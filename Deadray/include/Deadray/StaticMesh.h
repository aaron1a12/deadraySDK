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

	public:
		static const bool bRegistered;
		virtual NodeType GetNodeType() override;
	};

}

#endif