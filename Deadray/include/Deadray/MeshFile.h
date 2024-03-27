#ifndef   DEADRAY_MESHFILE_H
#define   DEADRAY_MESHFILE_H

#include "Deadray/Core.h"
#include "Deadray/Types.h"
#include <vector>

namespace Deadray {

	//struct ;

	class DEADRAY_API DMesh {
	public:
		static struct Header {
			//const char signature[] = "DMSH";
		};


		static bool ReadFromFile(LPCWSTR meshFile);

	};

}

#endif