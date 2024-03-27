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
			Camera = 6,
			Light = 7,
			Agent = 8
		};
	}
}

#endif