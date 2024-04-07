#ifndef   DEADRAY_GAMEMANAGER_H
#define   DEADRAY_GAMEMANAGER_H

#include "Deadray/Core.h"

namespace Deadray {

	class DEADRAY_API GameManager : public Node {
	public:
	public:
		GameManager();
		~GameManager();

		virtual uint32 GetNodeType() override {return Types::GameManager;}
	};

}

#endif