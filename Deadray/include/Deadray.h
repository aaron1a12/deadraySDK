/*////////////////////////////////////////////////////////////////

DEADRAY

////////////////////////////////////////////////////////////////*/

#ifndef   DEADRAY_H
#define   DEADRAY_H

#ifdef DEADRAY_DLL
#define DEADRAY_API __declspec(dllexport)
#else
#define DEADRAY_API __declspec(dllimport)
#endif

#include "Deadray/Core.h"
#include "Deadray/Engine.h"

#endif