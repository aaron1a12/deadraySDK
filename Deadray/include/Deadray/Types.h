#ifndef   DEADRAY_TYPES_H
#define   DEADRAY_TYPES_H

namespace Deadray {

	typedef uint16 NodeType;
	

	namespace Types {
		enum NodeTypes
		{
			Invalid = 0,
			Node = 1,
			Scene = 2,
			SceneNode = 3,
			Primitive = 4,
			MeshNode = 5,
			StaticMesh = 6,
			GameManager = 7,
			Camera = 8,
			Light = 9,
			Agent = 10
		};
	}
}

#endif