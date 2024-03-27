#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/OBJReader.h"
#include "Deadray/Primitive.h"
#include <deque>

using namespace Deadray;

float scaleFactor = 0.01f;

bool OBJReader::ReadFromFile(LPCWSTR objFile, PrimitiveMeshData* meshData)
{	
	HANDLE hFile;
	DWORD fileSize;
	DWORD dwBytesRead;
	uint8* buf = nullptr;

	std::deque<PrimitiveVertex> vertices;
	std::deque<Vector2> uvs;
	std::deque<Triangle> faces;



	hFile = CreateFile(objFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
        goto exit;
    }

	fileSize = GetFileSize(hFile, 0);

	if (fileSize == 0 || fileSize == INVALID_FILE_SIZE)
	{
		goto exit;
	}

	buf = (uint8*)malloc(fileSize);

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	if(!ReadFile(hFile, buf, fileSize, &dwBytesRead, NULL))
    {
        goto exit;
    }

	CloseHandle(hFile);


	// Begin parsing read buffer

	uint32 totalLines = 0;

	meshData->vertexCount = 0;

	PTR n = 0;

	while (n < fileSize)
	{	
		// Figure out line buffer size (loop until we find a CR or LF)

		uint32 lineSize = 0;
		PTR ln = n;

		while (ln < fileSize && !(memcmp((void*)&buf[ln], "\r", 1) == 0 || memcmp((void*)&buf[ln], "\n", 1) == 0))
		{
			lineSize++;
			ln++;
		}

		////////////////////////////
		
		// Parse each line
	
		PTR off = 0;
		
		// If it starts with V (v, vt, vn)
		if (memcmp((void*)&buf[n+off], "v", 1)==0 || memcmp((void*)&buf[n+off], "f", 1)==0)
		{

			OBJ_VTYPE type = OBJ_VERTEX;

			if (memcmp((void*)&buf[n+off], "vt", 2)==0)
			{
				type = OBJ_UV;
				off += 3;
			}
			else if (memcmp((void*)&buf[n+off], "vn", 2)==0)
			{
				type = OBJ_NORMAL;
				off += 3;
			}
			else if (memcmp((void*)&buf[n+off], "f", 1)==0)
			{
				type = OBJ_FACE;
				off += 2;
			}
			else
			{
				off += 2;
			}

			PTR eol = lineSize-off;

			// End of Line Ptr. You should not read from this point and onwards.
			PTR eolPtr = (PTR)buf + n + lineSize;

			//
			// Figure out how many property values do we have (separated by spaces)
			//

			uint32 valCount = 0;
			uint8 delimiter = 0x20; // space
			uint8* tmp      = &buf[n];			

			// Go thru every char 'till the end.
			while ((PTR)tmp < eolPtr)
			{

				if (*tmp == delimiter)
				{
					valCount++;
				}
				tmp++;
			}
			
			//
			// Create an array of all values and calculate their sizes
			//

			TinyValBuffer* valuePTRs = new TinyValBuffer[valCount];
			
			// For each value

			uint8* valPtr = &buf[n+off];

			for(uint32 i=0; i != valCount; i++)
			{
				// Save the start of the buffer
				valuePTRs[i].buffer = valPtr;

				// Go thru every char 'till we find a delimiter (space).
				while ((PTR)valPtr <= eolPtr)
				{
					if (*valPtr == delimiter || ((PTR)valPtr == eolPtr))
					{
						// Size = current address - start address
						valuePTRs[i].size = valPtr - valuePTRs[i].buffer;

						//DEBUG("SIZE:%d\n", valuePTRs[i].size);

						valPtr++;
						break;
					}
					else
					{
						valPtr++;
					}
				}
			}

			//
			// Finally parse the values
			//

			// For properties with float values

			if (type == OBJ_VERTEX || type == OBJ_NORMAL || type == OBJ_UV)
			{		

				float* scalars = new float[valCount];

				for(uint32 i=0; i != valCount; i++)
				{
					char* string = new char[valuePTRs[i].size + 1]; // make space for null terminated
					memcpy((void*)string, valuePTRs[i].buffer, valuePTRs[i].size);
					string[valuePTRs[i].size] = 0; // null terminator

					scalars[i] = (float)atof(string); // Converts string to double.

					delete [] string;
				}


				if (type == OBJ_VERTEX)
				{
					PrimitiveVertex vertex;
					vertex.x = scalars[0] * scaleFactor;
					vertex.y = scalars[1] * scaleFactor;
					vertex.z = scalars[2] * scaleFactor;
					vertex.color = 0xffffffff;

					//float oldX = vertex.x;
					//float oldZ = vertex.z;

					//vertex.x = oldX * -1.0 -  oldZ * 0.0;
					//vertex.z = oldZ * -1.0 -  oldX * 0.0;

					// We'll set these as an improbable UV coord so we can test if it's valid later
					vertex.u = FLT_MAX;
					vertex.v = FLT_MAX;

					vertices.push_back(vertex);
				}

				if (type == OBJ_UV)
				{
					Vector2 uvCoords = Vector2(
						/*1.0 - scalars[0],  // Obj tex coords are upside down when exporting from maya
						1.0 - scalars[1]*/
						scalars[0], 
						1-scalars[1]
					);

					uvs.push_back(uvCoords);
				}

				if (type == OBJ_NORMAL)
				{
					// We currently don't use normals so let's skip for now.
				}

				delete [] scalars;
			}

			// Hopefully, all of the verts and UVs have been read by now,
			// so we can begin building the polygons.

			if (type == OBJ_FACE && valCount == 3) // only accept faces with 3 points. I.e., triangles.
			{
				Triangle tri;
				int* vertexIndices = new int[valCount]; // 3

				// For each point
				for(uint32 i=0; i != valCount; i++)
				{
					//
					// Get the indices by splitting by "/" and converting to an integer
					//
					
					int* integers = new int[3]; // v/t/n
					WORD integerIndex = 0;
						
					// working null-terminated copy of the value buffer
					char* string = new char[valuePTRs[i].size + 1]; // make space for null terminated
					memcpy((void*)string, valuePTRs[i].buffer, valuePTRs[i].size);
					string[valuePTRs[i].size] = 0; // null terminator
					
					const char splitter[2] = "/";
					char *token;

					// get the first token 
					token = strtok(string, splitter);

					// walk through other tokens 
					while( token != NULL ) {
						// Index
						integers[integerIndex] = atoi(token) - 1; // Wavefront OBJs use 1-based indices instead of classic 0.

						integerIndex++;
						token = strtok(NULL, splitter);
					}

					///////////////////////////////////

					int vertexIndex = integers[0];
					int uvIndex = integers[1];
					int normalIndex = integers[2];

					vertexIndices[i] = vertexIndex;

					// Fetch the previously saved UVs using the found UV index and
					// assign the uvs in our previously saved primitive vertex...

					PrimitiveVertex& primVert = vertices[vertexIndex];
					Vector2& uvCoords = uvs[uvIndex];

					// ...but only do so if we haven't already done so.
					if (primVert.u == FLT_MAX)
					{
						primVert.u = uvCoords.x;
						primVert.v = uvCoords.y;
					}
					else if (uvCoords.x != primVert.u || uvCoords.y != primVert.v)	// Case scenario: a shared vertex with
					{									// more than one uv unique coord.
						// Duplicate the vertex
						PrimitiveVertex vertCopy = primVert;

						vertCopy.u = uvCoords.x;
						vertCopy.v = uvCoords.y;

						vertices.push_back(vertCopy);

						int newIndex = vertices.size() - 1;
						vertexIndices[i] = newIndex;
					}

					delete [] string;
					delete [] integers;
				}

				// Setup the triangle face by using the found indices

				tri.vertexA = vertexIndices[0];
				tri.vertexB = vertexIndices[1];
				tri.vertexC = vertexIndices[2];

				faces.push_back(tri);

				delete [] vertexIndices;
				
			}

			
			// Don't forget to delete the value array

			delete [] valuePTRs;

		} // END OF LINE

		
		////////////////////////////

		n += lineSize;
		totalLines++;

		// Skip lines left in front
		while (memcmp((void*)&buf[n], "\r", 1)==0 || memcmp((void*)&buf[n], "\n", 1) == 0)
		{
			n++;
		}
	}

	


	//
	// Finished parsing, convert to PrimitiveMeshData
	//

	// First, the vertices

	meshData->polyCount = faces.size();
	meshData->vertexCount = vertices.size();
	meshData->vertexBuffSize = meshData->vertexCount * sizeof(PrimitiveVertex);
	meshData->vertices = (PrimitiveVertex*)malloc(meshData->vertexBuffSize);


	

	// Copy-paste all the vertices into meshData->vertices

	PrimitiveVertex* vP = meshData->vertices;
	for(uint32 i=0; i != meshData->vertexCount; i++)
	{
//		DEBUG("Copypaste: x:%f, y:%f, z:%f\n", vertices[i].x, vertices[i].y, vertices[i].z);

		memcpy((void*)vP, &vertices[i], sizeof(PrimitiveVertex));
		vP++;
	}


	// Finally, the indices

	meshData->indexCount = 3 * meshData->polyCount; // 3 = 3 vertices per triangle
	meshData->indexBuffsize = meshData->indexCount * sizeof(WORD);
	meshData->indices = (WORD*)malloc(meshData->indexBuffsize);

	memset(meshData->indices, 0, sizeof(meshData->indexBuffsize));



	//DEBUG("\nNUMBER OF FACES: %d, NUMBER OF INDICES: %d, vertexBuffSize:%d\n", meshData->polyCount, meshData->indexCount, sizeof(PrimitiveVertex));


	WORD* iP = meshData->indices;


	WORD* tmpIndices = new WORD[meshData->indexCount];

	for(uint32 i=0, j=0; i != faces.size(); i++, j+=3)
	{
		Triangle& face = faces[i];

		tmpIndices[j] = face.vertexA;
		tmpIndices[j + 1] = face.vertexB;
		tmpIndices[j + 2] = face.vertexC;

		

		/*Triangle& face = faces[i];
		WORD* triIndices = new WORD[3];		
		triIndices[0] = face.vertexA;
		triIndices[1] = face.vertexB;
		triIndices[2] = face.vertexC;

		DEBUG("\nFACE. Indices: %d, %d, %d\n", triIndices[0], triIndices[1], triIndices[2]);

		//meshData->indices[j] = *triIndices;
		memcpy((void*)iP, (void*)triIndices, sizeof(WORD) * 3);*/
		
		iP++;
		//iP++;
		//iP++;

		//delete [] triIndices;
	}

	memcpy((void*)meshData->indices, (void*)tmpIndices, meshData->indexBuffsize);

	/*PrimitiveVertex quadVerts[] =
    {
        { -1.0f, -1.0f, 0.0f, 0xffffffff, 0.0, 0.0 },
        {  1.0f, -1.0f, 0.0f, 0xffffffff, 1.0, 0.0},
        {  -1.0f, 1.0f, 0.0f, 0xffffffff, 0.0, 1.0},
		{  1.0f, 1.0f, 0.0f, 0xffffffff, 1.0, 1.0}
    };
	
	meshData->vertexBuffSize = sizeof(quadVerts);

	meshData->vertices = (PrimitiveVertex*)malloc(meshData->vertexBuffSize);
	memcpy((void*)meshData->vertices, &quadVerts, meshData->vertexBuffSize);*/

	/*WORD quadIndices[6] = {
		0, 1, 2, // first triangle
		2, 1, 3  // second
	};

	meshData->indexBuffsize = sizeof(quadIndices);
	meshData->indices = (WORD*)malloc(meshData->indexBuffsize);
	memcpy((void*)meshData->indices, &quadIndices, meshData->indexBuffsize);*/


	delete [] tmpIndices;



    /*PrimitiveVertex quadVerts[] =
    {
        { -1.0f, 1.0f, 0.0f, 0x80808080, 0.0, 0.0 },
        {  1.0f, 1.0f, 0.0f, 0x80808080, 1.0, 0.0},
        {  -1.0f, -1.0f, 0.0f, 0x80808080, 0.0, 1.0},
		{  1.0f, -1.0f, 0.0f, 0x80808080, 1.0, 1.0}
    };

	WORD quadIndices[6] = {
		0, 1, 2,
		2, 1, 3
	};*/

		

	if (buf!=nullptr) delete buf;

	return true;

exit: {
		DEBUG("DAFUK")
		CloseHandle(hFile);	

		if (buf!=nullptr) delete buf;
		return false;
	}
}