#ifndef   DEADRAY_EVENT_H
#define   DEADRAY_EVENT_H

#include "Deadray/Core.h"
#include <deque>

namespace Deadray {

	class Node;

	class DEADRAY_API Event {
	private:
		std::deque<Node*> listeners;

	public:
		uint16 Type;

		void AddListener(Node* node);
		void Broadcast();

		Event();
		~Event();
	};

}
#endif