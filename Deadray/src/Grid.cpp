#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Engine.h"
#include "Deadray/Grid.h"

using namespace Deadray;



Grid::Grid(Engine* engine)
{
	inst = engine;
	
	//inst->log("Grid created.");
}

void Grid::Render()
{
	LPDIRECT3DDEVICE9 device = inst->GetD3dDevice();

	device->SetStreamSource(0, gridVertexBuffer, 0, sizeof(GridVertex));
	device->SetFVF(GRIDVERTEX_FVF);
	device->DrawPrimitive(D3DPT_LINELIST, 0, numLines);

	device->SetStreamSource(0, axisVertexBuffer, 0, sizeof(GridVertex));
	device->SetFVF(GRIDVERTEX_FVF);
	device->DrawPrimitive(D3DPT_LINELIST, 0, 3);
}

void Grid::OnDeviceStart()
{
	// Square unit
	const uint32 gridSize = 20;

	// How much distance between the lines (in centimetres)
	const float gridSpacing = 1.0f;
	
	// The length of one side of the entire grid
	const float length = gridSize * gridSpacing;

	// How offset the lines in the middle are relative to the world center.
	// If our length is odd, there will be no lines in the middle.
	float centerOffset = (gridSize % 2 == 1) ? gridSpacing * 0.5f : 0.0f;


	// Number of lines (x or z)
	numLines = (gridSize + 1) * 2;

	uint32 bufferSize = sizeof(LineSegment) * numLines;

	LineSegment* pLineSegments = (LineSegment*)malloc(bufferSize);

	uint32 end = (numLines / 2) + 1;

	inst->log("Total Lines: %u, Length: %f, center offset: %f", numLines, length, centerOffset);

	

	float beginOffset = 0.f - length / 2;
	float endOffset = 0.f + length / 2;


	// Start at the top left corner. (e.g., x: -1, z: -1)
	float x = beginOffset;
	float z = beginOffset;

	
	for (uint32 i=0; i != numLines; i += 2) 
	{
		float offset = i * (gridSpacing / 2);

		LineSegment& xline = *(pLineSegments + i);
		LineSegment& zline = *(pLineSegments + i + 1);

		DWORD color = 0x00666666;

		// hor
		xline.points[0] = GridVertex(beginOffset, 0.0, beginOffset + offset, color); // begin
		xline.points[1] = GridVertex(endOffset, 0.0, beginOffset + offset, color); // end

		zline.points[0] = GridVertex(beginOffset + offset, 0.0, beginOffset, color); // begin
		zline.points[1] = GridVertex(beginOffset + offset, 0.0, endOffset, color); // end
	}

	// Send to device

	// Microsoft: Buffers created with D3DPOOL_DEFAULT that do not specify D3DUSAGE_WRITEONLY may suffer a severe performance penalty. 
	HRESULT hr = inst->GetD3dDevice()->CreateVertexBuffer( bufferSize, D3DUSAGE_WRITEONLY, GRIDVERTEX_FVF, D3DPOOL_DEFAULT, &gridVertexBuffer, NULL );

	if (SUCCEEDED(hr))
	{
		OutputDebugString(L"\nSUCCEEDED!\n");
	}

	if (FAILED(hr))
	{
		OutputDebugString(L"\nFAILED!\n");
	}


	VOID* pVertices;
    gridVertexBuffer->Lock(0, bufferSize, (void**)&pVertices, 0 );
    memcpy(pVertices, pLineSegments, bufferSize);
    gridVertexBuffer->Unlock();

	delete pLineSegments;

	// Axis ref lines

	uint32 axisBufferSize = sizeof(LineSegment) * 3;

	LineSegment axes[3];
	
	axes[0].points[0] = GridVertex(0.0, 0.0, 0.0, 0x00ff0000);
	axes[0].points[1] = GridVertex(1.0, 0.0, 0.0, 0x00ff0000);

	axes[1].points[0] = GridVertex(0.0, 0.0, 0.0, 0x0000ff00);
	axes[1].points[1] = GridVertex(0.0, 1.0, 0.0, 0x0000ff00);

	axes[2].points[0] = GridVertex(0.0, 0.0, 0.0, 0x000000ff);
	axes[2].points[1] = GridVertex(0.0, 0.0, 1.0, 0x000000ff);

	// Send to device

	inst->GetD3dDevice()->CreateVertexBuffer( bufferSize, D3DUSAGE_WRITEONLY, GRIDVERTEX_FVF, D3DPOOL_DEFAULT, &axisVertexBuffer, NULL );

	VOID* pAxisVertices;
    axisVertexBuffer->Lock(0, axisBufferSize, (void**)&pAxisVertices, 0 );
    memcpy(pAxisVertices, axes, axisBufferSize);
    gridVertexBuffer->Unlock();
}

void Grid::OnDeviceShutdown()
{
	Destroy();
}


void Grid::Destroy()
{
	inst->log("Grid freed.");
    gridVertexBuffer->Release();
	gridVertexBuffer = NULL;

	axisVertexBuffer->Release();
	axisVertexBuffer = NULL;
}