#pragma once

#include "stdafx.h"

#include "..\\..\\MyApp.h"
#include "..\\..\\MyGUI.h"

namespace GUIFramework
{
	extern GUIMaterialsLibrary MaterialsLibrary;
}

int App::Run(HINSTANCE hInst, int nCmdShow, InputDevice inputDevices)
	{
	hInstance = hInst;
	MSG msg;
	
	if( CreationSetup.dxConfigWindow )
		if( !LaunchDxCofigWindow() )
			return false;

	if( !RegisterWindow() )
		return false;
	
	if( !InitializeWindow(nCmdShow) )
		return false;
	WindowInitialized = true;

	if( (inputDevices & App::InputDevice::MouseInput) != 0 )
		ShowCursor(true);
	else
		ShowCursor(false);

	if( CreationSetup.HasDirectSoundManager )
	{
		pSoundBuffers = new SoundBuffersMG();
		if( !pSoundBuffers->Initialize(hWnd) )
			return false;
	}

	if( !InitializeD3dDevice() )
		return false;

		//Sets the Camera
	Camera.SetDevice(pD3dDev);
	Camera.SetPresentParameters(presentParameters);
	
	if( !InitializeDInputDevice(inputDevices) )
		return false;

	Initialize();

	this->LoadContent();

	HRESULT hr = D3DXCreateTextureFromFile(pD3dDev, L"D3D9_Application_Framework\\Content\\Images\\controls.dds", &pControlsTexture);
	if( FAILED(hr) )
			this->Error("The default GUI controls texture not found");

		//Initialize the GUI Materials Library
	::GUIFramework::MaterialsLibrary.Initialize();

		//Sets the users GUI
	pGUI = ::GUIFramework::MyGUI::GetSingleton();
	pGUI->Load();

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER count;
	double elapsedTime;
	//LARGE_INTEGER elapsedTime;

	while( true )
		{
		if( PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE) )
			{
			if( msg.message == WM_QUIT )
				break;

			DispatchMessage(&msg);
			}

		QueryPerformanceCounter(&count);
		static LARGE_INTEGER prevCount = count;
		elapsedTime = (count.QuadPart - prevCount.QuadPart)/(double)freq.QuadPart;

		this->Update(elapsedTime);

		this->Render(elapsedTime);

		pGUI->RenderGUI(elapsedTime);

		prevCount = count;
		}

	pGUI->UnLoad();
	UnloadContent();
	UnregisterClass(className.c_str(), hInstance);
	return msg.wParam;
	}

inline ATOM App::RegisterWindow() const
		{
		WNDCLASSEX appClass;

		appClass.cbSize = sizeof(WNDCLASSEX);
		
		appClass.style			= wcs;
		appClass.lpszClassName  = className.c_str();
		appClass.lpfnWndProc    = this->WndProc;
		appClass.hInstance      = hInstance;
		appClass.cbClsExtra		= 0;
		appClass.cbWndExtra		= 0;
		appClass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(idIcon));
		appClass.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(idSmallIcon));
		appClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		appClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		appClass.lpszMenuName	= NULL;
				
		return RegisterClassEx(&appClass);
		}

inline BOOL App::InitializeWindow(const int nCmdShow)
	{
		hWnd = CreateWindowEx(wiexs, className.c_str(), title.c_str(), wis, 
			CW_USEDEFAULT, CW_USEDEFAULT, wndWidth, wndHeight, NULL, NULL, hInstance, NULL);

		if( !hWnd )
			return false;

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		return true;
		}

BOOL App::InitializeD3dDevice()
{
	LPDIRECT3D9 pD3d;
		
	#ifdef PIX_ENABLE
		pD3d = Direct3DCreate9(32);//PIX requires that you use headers of 
								   //version 32 with DX9
	#else
		pD3d = Direct3DCreate9(DIRECT3D_VERSION);
	#endif

	//ZeroMemory(&presentParameters,sizeof(D3DPRESENT_PARAMETERS));

	presentParameters.Windowed= true;
	presentParameters.hDeviceWindow= hWnd;
	presentParameters.SwapEffect= D3DSWAPEFFECT_DISCARD;
	presentParameters.EnableAutoDepthStencil= true;
	presentParameters.FullScreen_RefreshRateInHz= D3DPRESENT_RATE_DEFAULT;
	presentParameters.PresentationInterval= D3DPRESENT_INTERVAL_DEFAULT;

	if( !CreationSetup.dxConfigWindow )
		{
		D3DDISPLAYMODE DisplayMode;
		pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode);
		presentParameters.BackBufferFormat= DisplayMode.Format;
		presentParameters.AutoDepthStencilFormat= D3DFMT_D16;
		presentParameters.BackBufferWidth= wndWidth;  //By default uses the
		presentParameters.BackBufferHeight= wndHeight;//size of the window
		}

	HRESULT hr = NULL;
	hr = pD3d->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&presentParameters, &pD3dDev);

	pD3d->Release();

	if( FAILED(hr) )
		return false;

	return true;
}

BOOL App::InitializeDInputDevice(InputDevice type)
{
	LPDIRECTINPUT pDInput;
	HRESULT hr;
	hr = DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL);
	if( FAILED(hr) )
		return false;
	if( type & App::InputDevice::KeyboardInput )
		if( Keyboard.Initialize(pDInput,hWnd) == false )
			return false;
	if( type & App::InputDevice::MouseInput )
		{
		if( Mouse.Initialize(pDInput,hWnd) == false )
			return false;
		}

	if( type & App::InputDevice::JoystickInput )
		{
		#pragma region Get Joystick
		hr = pDInput->CreateDevice(GUID_Joystick, &pDInputJoystick, NULL);
		if( FAILED(hr) )
			return false;

		hr = pDInputJoystick->SetDataFormat( &c_dfDIJoystick );
		if( FAILED(hr) )
			return false;

		hr = pDInputJoystick->SetCooperativeLevel(hWnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
		if( FAILED(hr) )
			return false;

		hr = pDInputJoystick->Acquire();
		if( FAILED(hr) )
			return false;
		#pragma endregion
		}

	pDInput->Release();
	return true;
}

	// Basic Functionalities

bool App::ToggleConsole()
	{
	bool res;
	if( hasConsole )
		res = FreeConsole();
	else
		{
		res = AllocConsole();
		SetConsoleTitle( (title+L": Debug Console").c_str() );
		SetConsoleTextAttribute( GetStdHandle( STD_ERROR_HANDLE ), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		freopen("CONOUT$","w",stderr);// So the STD output from C & C++ is the MsWin Console
		freopen("CONIN$","r",stdin);  // So the STD input  from C & C++ is the MsWin Console
		}
	hasConsole = !hasConsole;
	return res;
	}

bool App::HasConsole()
{
	return hasConsole;
}

bool App::LaunchDxCofigWindow()
	{
	HWND hDlg = NULL;
		//Modeless Dialog
	if( !(hDlg = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DXCONFIG),NULL,DialogProc)) )
		return false;
	ShowWindow(hDlg, SW_SHOW);

	MSG msg;
	while( GetMessage(&msg,NULL,NULL,NULL) )
		{
		if( !IsDialogMessage(hDlg,&msg) )//Catchs & Pocess tab transversing
			{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			}
		}

	if( msg.wParam == false )//In case of cancel or Esc pressed
		return false;

	return true;
	}

	// Getters & Setters

void App::SetWindowClassStyle(const App::WndClassStyle classStyle)
	{
	wcs = classStyle;
	return;
	}

App::WndClassStyle App::GetWindowClassStyle() const
	{
	return wcs;
	}

void App::SetWindowStyle(const App::WndInstanceStyle instanceStyle, 
						 const App::WndInstanceExStyle instanceExStyle)
	{
	wis   = instanceStyle;
	wiexs = instanceExStyle;
	if( WindowInitialized )
		{
		SetWindowLongPtr(hWnd, GWL_STYLE  , wis  );
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, wiexs);
		SetWindowPos(hWnd,NULL,NULL,NULL,NULL,NULL,SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);
		}
	return;
	}

void App::GetWindowStyle(App::WndInstanceStyle& instanceStyle, 
						 App::WndInstanceExStyle& instanceExStyle) const
	{
	instanceStyle  = wis;
	instanceExStyle= wiexs;
	return;
	}

std::wstring App::SetTitle(const std::wstring str)
	{
	std::wstring aux = title;
	title = str;
	if( WindowInitialized )
		SendMessage(hWnd,WM_SETTEXT,NULL,(LPARAM)str.c_str());

	return aux;
	}

std::wstring App::GetTitle() const
	{
	return title;
	}

void App::SetIcons(const int idLarge, const int idSmall)
	{
	idIcon = idLarge;
	idSmallIcon = idSmall;
	if( WindowInitialized )
		{
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)MAKEINTRESOURCE(idIcon)); 
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)MAKEINTRESOURCE(idSmallIcon)); 
		}
	return;
	}

void App::GetIcons(int& idLarge, int& idSmall) const
	{
	idLarge = idIcon;
	idSmall = idSmallIcon;
	return;
	}

void App::SetWindowDimension(const int w,const  int h)
	{
	wndWidth  = w;
	wndHeight = h;
	RECT wndRect;
	wndRect.left = wndPosX; wndRect.right = wndPosX+w;
	wndRect.top = wndPosY; wndRect.bottom = wndPosY+h;
	if( WindowInitialized )
		SetWindowPos(hWnd,NULL,NULL,NULL,wndWidth,wndHeight,SWP_NOMOVE);
	return;
	}

void App::GetWindowDimension(int& w, int& h) const
	{
	w = wndWidth;
	h = wndHeight;
	return;
	}

void App::SetWindowPosition(const int x,const int y)
	{
	wndPosX  = x;
	wndPosY  = y;
	if( WindowInitialized )
		SetWindowPos(hWnd,NULL,wndPosX,wndPosY,NULL,NULL,SWP_NOSIZE);
	return;
	}

void App::GetWindowPosition(int& x, int& y) const
	{
	x = wndPosX;
	y = wndPosY;
	return;
	}

	//Main Window Procedure

LRESULT CALLBACK App::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	MyApp* pMyApp = MyApp::GetSingleton();

	GUIFramework::GUI* pGUI = NULL;
	
	switch (message)
	{
	case WM_SIZING:
		{
		RECT* window = (RECT*)lParam;
		pMyApp->wndWidth  = window->right  - window->left; 
		pMyApp->wndHeight = window->bottom - window->top; 
		return DefWindowProc(hWnd, message, wParam, lParam);
		}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CHAR:
		{
		pGUI = GUIFramework::MyGUI::GetSingleton();
		pGUI->ResolveChar(wParam, short LOWORD(lParam), lParam & 0x00000001);
		}
		break;
		// Mouse messages
    case WM_MOUSEMOVE:
	    {
		pGUI = GUIFramework::MyGUI::GetSingleton();
		pGUI->ResolveMousePosition(short( LOWORD( lParam )), short( HIWORD( lParam )));
		char* aux = new char[256];
		sprintf(aux, "X:%i Y:%i",short( LOWORD( lParam )), short( HIWORD( lParam )) );
		pMyApp->Error(aux);
		delete[] aux;
		}
		break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_XBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    case WM_XBUTTONDBLCLK:
    case WM_MOUSEWHEEL:
        {
		pGUI = GUIFramework::MyGUI::GetSingleton();
		pGUI->ResolveMouseLeftClickDown( AppPoint( (short)LOWORD( lParam ), (short)HIWORD( lParam ) ) );
		}
		break;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONUP:
			{
			pGUI = GUIFramework::MyGUI::GetSingleton();
			pGUI->ResolveMouseLeftClickUp( AppPoint( (short)LOWORD( lParam ), (short)HIWORD( lParam ) ) );
			}
		break;
	case WM_PAINT://Won't do anything special Direct3D controls the render to the window
		hdc = BeginPaint(hWnd, &ps);
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void App::GetBackBufferDimension(int& w, int& h) const
{
	w = presentParameters.BackBufferWidth;
	h = presentParameters.BackBufferHeight;
	return;
}

LPDIRECT3DDEVICE9 App::GetD3DDevice()
		{
			return pD3dDev;
		}

void App::Error(char* strError)
	{
		OutputDebugStringA( strError );
		if( this->hasConsole )
		{
			std::cerr<< strError << '\n';
		}

		return;
	}

void App::Error(wchar_t* strError)
	{
		OutputDebugStringW( strError );
		if( this->hasConsole )
		{
			std::wcerr<< strError << '\n';
		}

		return;
	}
	
void App::Error(std::string strError)
	{
		OutputDebugStringA( strError.c_str() );
		if( this->hasConsole )
		{
			std::cerr<< strError.c_str() << '\n';
		}

		return;
	}

void App::Error(std::wstring strError)
	{
		OutputDebugStringW( strError.c_str() );
		if( this->hasConsole )
		{
			std::cerr<< strError.c_str() << '\n';
		}

		return;
	}

void App::Fatal(char* strError, int nErrorCode)
	{
		OutputDebugStringA( strError );
		if( this->hasConsole )
		{
			std::cerr<< strError << '\n';
		}

		this->Exit(nErrorCode);

		return;
	}

void App::Fatal(wchar_t* strError, int nErrorCode)
	{
		OutputDebugStringW( strError );
		if( this->hasConsole )
		{
			std::wcerr<< strError << '\n';
		}

		this->Exit(nErrorCode);

		return;
	}
	
void App::Fatal(std::string strError, int nErrorCode)
	{
		OutputDebugStringA( strError.c_str() );
		if( this->hasConsole )
		{
			std::cerr<< strError.c_str() << '\n';
		}
		wchar_t* errorWideChars = new wchar_t[strError.length()];
		mbstowcs(errorWideChars, strError.c_str(), strError.length()+1);
		MessageBox(NULL, errorWideChars, L"Fatal Error", MB_ICONERROR|MB_OK);
		delete[] errorWideChars;

		this->Exit(nErrorCode);

		return;
	}

void App::Fatal(std::wstring strError, int nErrorCode)
	{
		OutputDebugStringW( strError.c_str() );
		if( this->hasConsole )
		{
			std::cerr<< strError.c_str() << '\n';
		}

		this->Exit(nErrorCode);

		return;
	}

void App::Exit(int nExitCode)
	{
		PostQuitMessage(nExitCode);
		return;
	}
