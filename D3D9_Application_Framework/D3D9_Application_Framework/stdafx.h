#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' "\
	"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <cmath>

// DX Libraries
#pragma comment(lib, "d3d9.lib")
#include <d3d9.h>						//Direct 3D 9
#if _DEBUG
	#pragma comment(lib, "d3dx9d.lib")
#else
	#pragma comment(lib, "d3dx9.lib")
#endif
#include <d3dx9.h>
#pragma comment(lib, "DxErr.lib")
#include <DxErr.h>						//Error Information
#pragma comment(lib, "dxguid.lib")
#include <rmxfguid.h>
//#include <rmxftmpl.h>
#pragma comment(lib, "dinput8.lib")
#include <dinput.h>						//Direct Input

//if want DirectSound
#pragma comment(lib, "Winmm.lib")
#include <mmsystem.h>//already inside windows.h
#pragma comment(lib, "dsound.lib")		//Direct Sound
#include <dsound.h>

// C++ STD libraries
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <utility>
#include <iostream>

#include <cassert>
#include <exception>

//Config Dialog Includes

#include <atlbase.h>
#include <atlcom.h>
#include <initguid.h>

namespace ConfigDialog
{
	namespace Ribbon
	{
		#include <uiribbon.h>

		#pragma comment(lib, "propsys.lib")
		#include <UIRibbonPropertyHelpers.h>
	}
}

#include <ppl.h>

#include <commctrl.h>

#define ARRAY_ACCESS_INLINING

//MatLab C++ RunTime

//namespace MathLab
//{
//	#include <mclcppclass.h>
//	//MatLab .m function link
//	#include "AnalizarPulsos\\AnalizePulses.h"
//}

//#define ARRAY_ACCESS_INLINING
//#include <matrix.h>


//To use PIX 
#define PIX_ENABLE



#define OUT
#define IN


