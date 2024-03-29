#ifndef   DEADRAY_MESHNODE_H
#define   DEADRAY_MESHNODE_H

#include "Deadray/Core.h"

namespace Deadray {

	struct DEADRAY_API MeshNodeParams
	{
		MeshNodeParams() {}
		MeshNodeParams(LPCWSTR inFile) {pSrcFile = inFile;}

		LPCWSTR pSrcFile;
	};


	class DEADRAY_API MeshNode : public Node {

	public:
		MeshNode(Node* parent, void* params = nullptr);
		~MeshNode();

	public:
		static const bool bRegistered;
		virtual NodeType GetNodeType() override;
	};

}

#endif