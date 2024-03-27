#ifndef   DEADRAY_MESHACTOR_H
#define   DEADRAY_MESHACTOR_H

#include "Deadray/Core.h"
#include "Deadray/SceneNode.h"

namespace Deadray {

	// Forward declarations
	class MeshNode;

	class DEADRAY_API StaticMesh : public SceneNode {

	private:
		MeshNode* mesh;

	public:
		StaticMesh(Node* parent);
		~StaticMesh();

		virtual NodeType GetNodeType() override {
			return Types::SceneNode;
		}
	};

}

#endif