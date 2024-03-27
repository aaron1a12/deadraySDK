#ifndef   DEADRAY_H
#define   DEADRAY_H

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DEADRAY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DEADRAY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DEADRAY_DLL
#define DEADRAY_API __declspec(dllexport)
#else
#define DEADRAY_API __declspec(dllimport)
#endif

#include "Deadray/Core.h"
#include "Deadray/Engine.h"

#endif