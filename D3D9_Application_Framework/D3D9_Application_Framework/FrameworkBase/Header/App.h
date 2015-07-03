////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					Unique Application			  //////////////////////////////////////
///////////////////////////////////					------------------			  //////////////////////////////////////
///////////////////////////////////					Composes of differents		  //////////////////////////////////////
///////////////////////////////////					Modules:					  //////////////////////////////////////
///////////////////////////////////					KeyboardMG					  //////////////////////////////////////
///////////////////////////////////					MouseMG						  //////////////////////////////////////
///////////////////////////////////					CameraMG					  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "..\\..\\resource.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SoundBuffers.h"
#include "GUI\\GUI.h"

struct AppCreationSetup
{
	AppCreationSetup(): HasDirectSoundManager(false), dxConfigWindow(true)
		{}

	bool HasDirectSoundManager;

	bool dxConfigWindow;

};

	//Epsilon value
#define EPI 0.0000001f

//Abstract Class
//Derive form this to get its base functionality
//
//SEE MyApp(derived from this)
class App
	{
	public:
		//Used to store Window style
		typedef UINT WndClassStyle, WndInstanceStyle, WndInstanceExStyle;
		//Specifies the input device to be initialized and aquired
		enum InputDevice{KeyboardInput = 1, MouseInput = 2, JoystickInput = 4};

	protected:
		//variables accesible form the derived class

		bool WindowInitialized;//some properties change differently if 
							   //the window is already initialized
	
		HINSTANCE hInstance;
			//Dx Window Variables
		std::wstring className;
		WndClassStyle wcs;
		WndInstanceStyle wis;
		WndInstanceExStyle wiexs;
		HWND hWnd;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//Dx window procedure
		int idIcon;		//Customizable Application Icon
		int idSmallIcon;//Customizable Application Icon small
		int wndWidth;   //Current Dx window width
		int wndHeight;  //Current Dx window height
		int wndPosX;    //Current Dx window position X
		int wndPosY;    //Current Dx window position Y

			//Config Window Procedure
		static INT_PTR CALLBACK DialogProc(HWND hDlg,UINT uMessage,WPARAM wparam,LPARAM lparam);

			//Application Properties
		std::wstring title;

		

			//Helper Functions
		inline ATOM RegisterWindow() const;

		inline BOOL InitializeWindow(const int nCmdShow);


		//Dx & DInput Variables
		D3DDEVTYPE deviceType;
		D3DPRESENT_PARAMETERS presentParameters;

			//Application Properties
		bool hasConsole;

			//Input and Graphic Devices or Managers
		//Keyboard device
		KeyboardMG Keyboard;
		//Mouse device Manager
		MouseMG Mouse;
		//Joystick device
		LPDIRECTINPUTDEVICE pDInputJoystick;
		//camera Manager
		CameraMG Camera;
	public:
		//DirectSound Manager 
		SoundBuffersMG* pSoundBuffers;
	protected:
		//Device de Direct3d
		LPDIRECT3DDEVICE9 pD3dDev;
		
		::GUIFramework::GUI* pGUI;

		virtual bool LaunchDxCofigWindow();

		virtual BOOL InitializeD3dDevice();

		virtual BOOL InitializeDInputDevice(InputDevice type);

		App(void): title(L"Generic DirectX Application"), className(L"MyDxApp"),
			wcs(CS_HREDRAW | CS_VREDRAW), wis(WS_OVERLAPPEDWINDOW), wiexs(NULL),
			idIcon(NULL), idSmallIcon(NULL), wndPosX(0), wndPosY(0), wndWidth(640), wndHeight(480),
			hasConsole(false), pSoundBuffers(NULL), pD3dDev(NULL), pDInputJoystick(NULL), deviceType(D3DDEVTYPE_HAL),
			WindowInitialized(false), pGUI(NULL)
			{}
	public:

		friend class GUIFramework::GUI;

		LPDIRECT3DTEXTURE9 pControlsTexture;//default controls textures for the GUI

		~App(void)
			{
			if( hasConsole ) FreeConsole();
			if( pDInputJoystick )
				{
				pDInputJoystick->Unacquire();
				pDInputJoystick->Release();
				}
			if( pD3dDev )
				pD3dDev->Release();
			}

		//Execute the main loop
		//
		//hInst        the current handle to your instance, just pass the main parameter
		//nCmdShow     the window instantiation show state
		//inputDevices the input devices you wish to initialize, use App::InputDevice
		//											(if you want another make it youself!)
		int Run(HINSTANCE hInst,int nCmdShow, InputDevice inputDevices);

			//-----------------------
			// Pure Virtual Functions
			//-----------------------

		//Execute after getting the window and the devices,
		//makes the user app initializations
		virtual void Initialize() = 0;

		//Execute after Initialize, loads all the nesesary 
		//DirectX and user resources
		virtual void LoadContent() = 0;

		//Execute before quitting the app, it just unloads
		//the resources loaded in LoadContent(NO OTHER)
		virtual void UnloadContent() = 0;

		//Execute 1 time per loop, it updates all
		//
		//- elapsedTime the elapsed time between this frame and the previous
		virtual void Update(const double elapsedTime) = 0;

		//Execute 1 time per loop, it renders the scene
		//
		//- elapsedTime the elapsed time between this frame and the previous
		virtual void Render(const double elapsedTime) = 0;

			//-----------------------
			// Basic Functionalities
			//-----------------------

		AppCreationSetup CreationSetup;

		//Obtaines/Release a console linked with cerr
		bool ToggleConsole();

		//Ask if the App has a console
		bool HasConsole();

			// Getters & Setters

		//Sets the style of window to be register
		//Must be set before the app is created or it will be unefective
		//
		//classStyle		 is the Window class style
		void SetWindowClassStyle(const App::WndClassStyle classStyle);

		//Gets the style of window to be/was register
		App::WndClassStyle GetWindowClassStyle() const;

		//Sets the styles the window will be/was obtained
		//
		//instanceStyle      is the window creation style
		//WndInstanceExStyle is the window extended creation style
		void SetWindowStyle(const WndInstanceStyle instanceStyle, 
			const WndInstanceExStyle instanceExStyle);

		//Gets the style the window will be/was obtained,
		//
		//instanceStyle      is the window creation style
		//WndInstanceExStyle is the window extended creation style
		void GetWindowStyle(WndInstanceStyle& instanceStyle, 
			WndInstanceExStyle& instanceExStyle) const;

		//Sets the main window title and the assosiated console title
		std::wstring SetTitle(const std::wstring str);

		//Gets the main window title
		std::wstring GetTitle() const;

		//Sets the window icons
		void SetIcons(const int idLarge, const int idSmall);

		//Gets the window icons
		void GetIcons(int& idLarge, int& idSmall) const;

		//Sets the window dimensions
		void SetWindowDimension(const int w, const int h);

		//Gets the window dimensions
		void GetWindowDimension(int& w, int& h) const;
	
		//Sets the window position in desktop coordinates
		void SetWindowPosition(const int x,const  int y);

		//Gets the window position in desktop coordinates
		void GetWindowPosition(int& x, int& y) const;

		//Gets the BackBuffer dimensions
		void GetBackBufferDimension(int& w, int& h) const;

		//Gets the application D3D Device
		LPDIRECT3DDEVICE9 GetD3DDevice();

			//Error Functions, promps the string in the console 
			//or the output window
		void Error(char* strError);

		void Error(wchar_t* strError);
				
		void Error(std::string strError);
		
		void Error(std::wstring strError);

			//Fatal Functions, promps the string in the console 
			//or the output window and then Exits the App
		void Fatal(char* strError, int nExitCode);

		void Fatal(wchar_t* strError, int nExitCode);
				
		void Fatal(std::string strError,int nExitCode);
		
		void Fatal(std::wstring strError, int nExitCode);

		//Exits the App with the specified code 
		//
		//- nExitCode	The exit code
		void Exit(int nExitCode);
		
	};



