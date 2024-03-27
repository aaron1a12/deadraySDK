#ifndef   DEADRAY_NODE_H
#define   DEADRAY_NODE_H

#include "Deadray/Core.h"
#include "Deadray/Types.h"
#include <vector>

//#define CreateChildNode(T, P) _CreateChildNode<T>(&T##Params(P))



namespace Deadray {

	class Engine;
	class Scene;
	class Event;

	class DEADRAY_API Node {

	private:// private
		
		Engine* pEngine;
		Node* pParentNode;
		std::vector<Node*> childNodes;
		std::vector<uint16> typeFamily;
		

	public:
		//typedef Node Super;
		Node(Engine* engine, void* params = nullptr);
		Node(Node* parent, void* params = nullptr);
		~Node();

		virtual void OnDeviceStart();
		virtual void OnDeviceShutdown();

		virtual void Destroy();

		Engine* GetEngine();
		Scene* GetScene();

		inline virtual NodeType GetNodeType() {
			return Types::Node;
		}

		inline virtual const char* GetNodeClassName() {
			return "Node";
		}


		// Attaches an existing node to this one
		void AttachNode(Node* childNode);

		// Associates this node with the scene.
		void RegisterSceneNode(Node* node);

		//T* _CreateChildNode (void* params = nullptr)  // ... = flexible constructor possible!?

		template <class T>
		T* CreateChildNode ()  // ... = flexible constructor possible!?
		{
			T* ptr = new T( this );

			AttachNode((Node*)ptr);
			RegisterSceneNode((Node*)ptr);


			return ptr;
		}

		
		bool bIsTickable;


		// 
		void AddEventListener(uint16 evtType, Node* listener);
		virtual void OnEvent(const Event& evt);

		//
		//virtual bool ShouldRegisterAsTickable();

		//void RegisterAsTickable();

		//void Tick(float dt);

		//void onEvent();

		/*static inline const bool RegisterType() {
			return true;
		}

		static const bool bRegistered = RegisterType();*/

	public:
		//virtual void OnGameStart();

		virtual void OnTick(float dt);

	};

}

#endif