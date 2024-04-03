#pragma once

#include "Deadray/Core.h"
#include "Deadray/DMap.h"

namespace Deadray {

	class DEADRAY_API Node;

	typedef void* (*NodeCreator)(Node*);

	// Registers engine-wide node type (makes it spawnable when loading levels and allows class name lookup)
	#define REGISTER_NODE_TYPE(CLASSID, CLASSNAME) \
	static bool CLASSNAME##Registered = TypeManager.RegisterNewType(NodeClassInfo(CLASSID, #CLASSNAME, (NodeCreator)&CLASSNAME::CreateObject<CLASSNAME>));

	struct NodeClassInfo {
		uint32 Id;
		const char* Name;
		NodeCreator Creator;

		NodeClassInfo() {
			Id = 0;
			Name = "Unknown Node";
			Creator = 0;
		}

		NodeClassInfo(uint32 inClassId, const char* inClassName, NodeCreator inCreator) {
			Id = inClassId;
			Name = inClassName;
			Creator = inCreator;
		}

	};

	class DEADRAY_API TypeManagerClass {
	public:
		TypeManagerClass();
		~TypeManagerClass();
	private:

		// Force initialization of the map by placing it statically inside a method.
		// Otherwise, the different translation units could interact with it before proper initialization.
		static DMap<uint32, NodeClassInfo>& GetRegistry() {
			static DMap<uint32, NodeClassInfo> registeredNodes;
			return registeredNodes;
		}

	public:

		static const bool RegisterNewType(NodeClassInfo nodeClass);
		static const char* GetNodeTypeName(uint32 type);
		static Node* CreateNewObjectByType(uint32 type, Node* parent);
	};

	// Singleton
	extern TypeManagerClass TypeManager;
}