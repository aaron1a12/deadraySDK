#ifndef   DEADRAY_DEBUGTEXT_H
#define   DEADRAY_DEBUGTEXT_H

#include "Deadray/Core.h"

namespace Deadray {

	class Engine;


	class DEADRAY_API DebugText : public Node {
		ID3DXFont* g_pFont;

		float fps;
		float fpsMs;
		float timeSinceFpsUpdate;

	public:
		DebugText();
		~DebugText();

		void OnDeviceStart() override;
		void OnDeviceShutdown()  override;
		void Destroy() override;

		void Render();
	};

}

#endif