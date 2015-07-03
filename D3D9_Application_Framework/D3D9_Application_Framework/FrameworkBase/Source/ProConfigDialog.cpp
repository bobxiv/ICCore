#include "stdafx.h"

#include "..\\Header\\ProConfigDialog.h"

//The CommandHandlers that will be use in CConfigDialogApplication to respond to User Actions
#include "..\\Header\\CommandHandlers.h"

//The ATL Module of this application
CComModule _Module;

ConfigDialog::CConfigDialogApplication* ConfigDialog::CConfigDialogApplication::Singleton;

namespace ConfigDialog
{
	//The procedure variables(All the window/control Events can access & change them)
	static DEVMODE devMode;                  //Pretended Display Mode
	static LPDIRECT3D9 pD3d;				 //To check the device capabilities
	static D3DDEVTYPE	devType;			 //The type of device wanted
	static D3DDISPLAYMODE currentDisplayMode;//The display mode at the time of start, 
	D3DPRESENT_PARAMETERS presentParameters; //The present parameters selected to create the device

	IUIFramework* pRibbonFramework;

	bool InitRibbon(HWND hDialogWnd)
	{
		//Initialize the COM Library
		HRESULT hr = CoInitialize(NULL);
		if( FAILED(hr) )
			return false;

		//Creates the Ribbon Framework
		hr = ::CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pRibbonFramework));
		if( FAILED(hr) )
			return false;

		//Creates the singleton RibbonApplication, known as ConfigApplication
		IUIApplication* pRibbonApplication;
		pRibbonApplication = CConfigDialogApplication::GetSingleton();

		//Binds the Dialog Wnd with the ConfigDialogApplication(RibbonApplication)
		hr = pRibbonFramework->Initialize(hDialogWnd, pRibbonApplication);
		if( FAILED(hr) )
			return false;

		//Loads the compiled Ribbon xml structure
		hr = pRibbonFramework->LoadUI(::GetModuleHandle(NULL), L"APPLICATION_RIBBON");
		if( FAILED(hr) )
			return false;

		return true;
	}

	bool DestroyRibbon()
	{
		if( pRibbonFramework != NULL )
		{
			pRibbonFramework->Destroy();
			pRibbonFramework->Release();
			pRibbonFramework = NULL;
		}

		return true;
	}

	CConfigDialogApplication::CConfigDialogApplication(): m_cRef(1), m_pRPCommandHandler(NULL)
	{
		m_VSyncEnable = false;
		m_FTSEnable = false;
	}

	__checkReturn CConfigDialogApplication* CConfigDialogApplication::GetSingleton()
	{
		if( Singleton == NULL )
			Singleton = new CConfigDialogApplication();

		return Singleton;
	}

	// IUnknown method implementations.
	STDMETHODIMP_(ULONG) CConfigDialogApplication::AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHODIMP_(ULONG) CConfigDialogApplication::Release()
	{
		LONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0)
		{
			delete this;
		}

		return cRef;
	}

	STDMETHODIMP CConfigDialogApplication::QueryInterface(REFIID iid, void** ppv)
	{
		if (iid == __uuidof(IUnknown))
		{
			//we have to do this because we have 2 Unknown parents from IUIApplication and IUICommandHandler
			//so we chose the IUNKNOWN from IUIApplication
			*ppv = static_cast<IUnknown*>(static_cast<Ribbon::IUIApplication*>(this));
		}else if (iid == __uuidof(Ribbon::IUIApplication))
		{
			*ppv = static_cast<Ribbon::IUIApplication*>(this);
		}
		else if(iid == __uuidof(Ribbon::IUICommandHandler))
		{
			*ppv = static_cast<Ribbon::IUICommandHandler*>(this);
		}else
		{
			*ppv = NULL;
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	// IUIApplication Implementation
	COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE CConfigDialogApplication::OnViewChanged(UINT32 nViewID, __in UI_VIEWTYPE typeID, __in IUnknown* pView, UI_VIEWVERB verb, INT32 uReasonCode)  
	{ 
		UNREFERENCED_PARAMETER(nViewID);
		UNREFERENCED_PARAMETER(typeID);
		UNREFERENCED_PARAMETER(pView);
		UNREFERENCED_PARAMETER(verb);
		UNREFERENCED_PARAMETER(uReasonCode);
		return E_NOTIMPL; 
	}

	COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE CConfigDialogApplication::OnCreateUICommand(
														   UINT32 nCmdID, 
														   __in UI_COMMANDTYPE typeID,
														   __deref_out IUICommandHandler** ppCommandHandler) 
	{ 
		UNREFERENCED_PARAMETER(typeID);

		switch(nCmdID)
		{
		case cmdHAL:
		case cmdREF:
		case cmdWARP:
			{
				if( m_pRPCommandHandler == NULL )
					CRPCommandHandler::CreateInstance(&m_pRPCommandHandler);
				return m_pRPCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
			}
		}

			return this->QueryInterface(IID_PPV_ARGS(ppCommandHandler));//the app is the default command handler for the controls
	}

	COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE CConfigDialogApplication::OnDestroyUICommand(
														    UINT32 commandId,
															__in UI_COMMANDTYPE typeID,
															__in_opt IUICommandHandler* pCommandHandler) 
	{ 
		UNREFERENCED_PARAMETER(commandId);
		UNREFERENCED_PARAMETER(typeID);
		UNREFERENCED_PARAMETER(pCommandHandler);
		return E_NOTIMPL; 
	}

	// IUICommandhandler Implementation
	STDMETHODIMP CConfigDialogApplication::Execute(UINT nCmdID,												/*Control ID*/
						UI_EXECUTIONVERB verb,										/*Action or Event*/
						__in_opt const PROPERTYKEY* key,							/*Property Changing*/
						__in_opt const PROPVARIANT* ppropvarValue,					/*Value of the propery changing*/
						__in_opt IUISimplePropertySet* pCommandExecutionProperties)
	{
		UNREFERENCED_PARAMETER(pCommandExecutionProperties);
		m_hr = S_OK;
		
		switch (verb)//Action switch
		{
		case UI_EXECUTIONVERB_EXECUTE:
		case UI_EXECUTIONVERB_PREVIEW:
		case UI_EXECUTIONVERB_CANCELPREVIEW:
			{
			switch( nCmdID )//Control switch
			{
			case cmdVSync:
				{
					PROPVARIANT var, varNew;

					m_hr = pRibbonFramework->GetUICommandProperty(cmdVSync, UI_PKEY_BooleanValue, &var);
					if (FAILED(m_hr))
					   return m_hr;

					m_hr = PropVariantToBoolean(var, &m_VSyncEnable);
					if (FAILED(m_hr))
					   return m_hr;

					m_VSyncEnable = !m_VSyncEnable;
					m_hr = UIInitPropertyFromBoolean(UI_PKEY_Enabled, m_VSyncEnable, &varNew);
					if (FAILED(m_hr))
					   return m_hr;

					m_hr = pRibbonFramework->InvalidateUICommand(cmdVSync, UI_INVALIDATIONS_PROPERTY, &UI_PKEY_Label);
					if (FAILED(m_hr))
						return m_hr;
				}
				break;
			case cmdFTS:
				{
					m_FTSEnable = !m_FTSEnable;
					
					m_hr = pRibbonFramework->InvalidateUICommand(cmdFTS, UI_INVALIDATIONS_PROPERTY, &UI_PKEY_Label);
					m_hr = pRibbonFramework->InvalidateUICommand(cmdFT,  UI_INVALIDATIONS_PROPERTY, &UI_PKEY_Enabled);
					if (FAILED(m_hr))
						return m_hr;
				}
				break;

			}//End Control switch

			}
			break;

		}//End Action Switch

		return m_hr;
	}

	STDMETHODIMP CConfigDialogApplication::UpdateProperty(UINT nCmdID,									 /*Control ID*/
								__in REFPROPERTYKEY key,						 /*Property Key Affected*/
								__in_opt const PROPVARIANT* ppropvarCurrentValue,/*Current Value*/
								__out PROPVARIANT* ppropvarNewValue)			 /*New Value*/
	{

		UNREFERENCED_PARAMETER(ppropvarCurrentValue);

		m_hr = E_FAIL;

		if( key == UI_PKEY_Label )
		{
			// Update the Label of FTS control, to show if it is enabled or not
			if (nCmdID == cmdFTS)
			{
				if (m_FTSEnable)
					m_hr = UIInitPropertyFromString(UI_PKEY_Label, L"FTS Enabled", ppropvarNewValue);
				else
					m_hr = UIInitPropertyFromString(UI_PKEY_Label, L"FTS Disabled", ppropvarNewValue);
			}

			// Update the Label of VSync control, to show if it is enabled or not
			if (nCmdID == cmdVSync)
			{
				if (m_VSyncEnable)
					m_hr = UIInitPropertyFromString(UI_PKEY_Label, L"VSync\nEnabled", ppropvarNewValue);
				else
					m_hr = UIInitPropertyFromString(UI_PKEY_Label, L"VSync\nDisabled", ppropvarNewValue);
			}
		}

		if( key == UI_PKEY_Enabled )
		{
			// Update the Enabability of FT depending on FTS
			if (nCmdID == cmdFT)
			{
				if(m_FTSEnable)
					m_hr = UIInitPropertyFromBoolean(UI_PKEY_Enabled, true, ppropvarNewValue);
				else
					m_hr = UIInitPropertyFromBoolean(UI_PKEY_Enabled, false, ppropvarNewValue);
			}
		}

		return m_hr;

	}

	//Strictly the Dialog Implementation
	INT_PTR CALLBACK DialogProc(HWND hDlg,UINT uMessage,WPARAM wParam,LPARAM lParam)
	{
	switch(uMessage)
	 {
	 case WM_INITDIALOG:
		InitRibbon(hDlg);
 		return DE_InitializeDialog(hDlg);
		break;

	 case WM_COMMAND:
	  {
		  int wmID    = LOWORD(wParam);
		  int wmEvent = HIWORD(wParam);
		  switch(wmID)
		   {
				case IDC_RESOLUTION:
					{
					switch( wmEvent )
						{
						case CBN_SELCHANGE:
							CE_ResolutionSelectionChange(hDlg,(HWND)lParam);
							break;
						}
					}
					break;
				case IDC_SETDISPLAYMODE:
					{
					switch( wmEvent )
						{
						case BN_CLICKED:
							CE_SetDisplayModeClick(hDlg);
							break;
						}
					}
				case IDC_BACKBUFFER:
					{
					switch( wmEvent )
						{
						case CBN_SELCHANGE:
							CE_BackBufferSelectionChange(hDlg,(HWND)lParam);
							break;
						}
					}
					break;
				case IDC_DEPTHSTENCIL:
					{
					switch( wmEvent )
						{
						case CBN_SELCHANGE:
							CE_DepthStencilSelectionChange(hDlg,(HWND)lParam);
							break;
						}
					}
					break;
				case IDC_HAL:
					{
					switch( wmEvent )
						{
						case BN_CLICKED:
							devType = D3DDEVTYPE_HAL;
							UpdateLabelCompatibility(hDlg);
							break;
						}
					}
					break;
				case IDC_REF:
					{
					switch( wmEvent )
						{
						case BN_CLICKED:
							devType = D3DDEVTYPE_REF;
							UpdateLabelCompatibility(hDlg);
							break;
						}
					}
					break;
				//Cancel button pressed
				case IDC_CANCEL:
					DestroyWindow(hDlg);
					PostQuitMessage(0);
					break;
				//Accept button pressed
				case IDC_ACCEPT:
				  /* MyApp* pMyApp = MyApp::GetSingleton();
				   pMyApp->presentParameters = presentParameters;
				   pMyApp->deviceType = devType;
				   wchar_t* txt= new wchar_t[101];
				   GetDlgItemText(hDlg, IDC_WIDTH, txt, 100);
				   pMyApp->presentParameters.BackBufferWidth = _wtoi(txt);
				   GetDlgItemText(hDlg, IDC_HEIGHT, txt, 100);
				   pMyApp->presentParameters.BackBufferHeight = _wtoi(txt);
				   GetDlgItemText(hDlg, IDC_WWIDTH, txt, 100);
				   pMyApp->wndWidth = _wtoi(txt);
				   GetDlgItemText(hDlg, IDC_WHEIGHT, txt, 100);
				   pMyApp->wndHeight = _wtoi(txt);
				   delete[] txt;*/
				   DestroyWindow(hDlg);
				   PostQuitMessage(1);
				   break;
		  }//End of Control ID switch

		  return 0;
	  }//End of Command Message

	 case WM_KEYDOWN:
	  {
		  switch(wParam)
		   {
		   case VK_ESCAPE:	
			DestroyWindow(hDlg);
			PostQuitMessage(0);
			break;
			}
	  }
	 case WM_DESTROY:
		  pD3d->Release();
		  break;
	 default:
		 return false;
	}

	return true;
	}




	bool DE_InitializeDialog(HWND hDlg)
	{
			//Loads the Dx9 Icon
		HANDLE hDxLogo = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DX9LOGO), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		SendDlgItemMessage( hDlg, IDC_LOGO, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hDxLogo);

			//The default buffers width/height
		presentParameters.BackBufferWidth = 640;
		SendDlgItemMessage( hDlg, IDC_WIDTH, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"640");
		presentParameters.BackBufferHeight = 480;
		SendDlgItemMessage( hDlg, IDC_HEIGHT, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"480");

			//The default buffers width/height
		presentParameters.BackBufferWidth = 640;
		SendDlgItemMessage( hDlg, IDC_WWIDTH, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"640");
		presentParameters.BackBufferHeight = 480;
		SendDlgItemMessage( hDlg, IDC_WHEIGHT, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"480");
			
		
			//Sets the combo contents and Display properties
		HWND hWndCBBackBufer      = GetDlgItem(hDlg, IDC_BACKBUFFER);
		HWND hWndCBResolution     = GetDlgItem(hDlg, IDC_RESOLUTION);
		HWND hWndSTAspectRatio    = GetDlgItem(hDlg, IDC_ASPECTRATIO);
		HWND hWndSTDisplayFormat  = GetDlgItem(hDlg, IDC_DISPLAYFORMAT);
		HWND hWndCBDS			  = GetDlgItem(hDlg, IDC_DEPTHSTENCIL);
		HWND hWndTBHAL			  = GetDlgItem(hDlg, IDC_HAL);


			//By default activates the HAL device
		devType = D3DDEVTYPE_HAL;
		SendMessage(hWndTBHAL, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)NULL);
			
		pD3d = Direct3DCreate9(D3D9b_SDK_VERSION);

		pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &currentDisplayMode);

		D3DDISPLAYMODE displayMode;
		wchar_t buf[100];
		wchar_t* format= new wchar_t[30];

		//Gets the Current Display Mode
		pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
		UpdateLabelCurrentDisplayMode(currentDisplayMode, hDlg);
		
		#pragma region Store the current display format in static variable devMode
			switch( displayMode.Format )
				{	
				case D3DFMT_X8R8G8B8: 
					devMode.dmBitsPerPel = 32;//bits
					break;
				case D3DFMT_A8R8G8B8:
					devMode.dmBitsPerPel = 32;//bits
					break;
				case D3DFMT_R8G8B8:
					devMode.dmBitsPerPel = 24;//bits
					break;
				default:
				devMode.dmBitsPerPel = 0;//Unknown
				}
		#pragma endregion
		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | 
			DM_BITSPERPEL | DM_DISPLAYFREQUENCY;//Initialize, the properties that could be change later
		devMode.dmSize= sizeof(DEVMODE);

		UpdateLabelCurrentDisplayMode(displayMode, hDlg);
		
		D3DFORMAT currentDisplatFormat = displayMode.Format;

			//Checks and loads Display Mode Suported 
		unsigned int maxModes = pD3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, currentDisplatFormat );
		for(unsigned int mode=0;mode < maxModes; ++mode)//Sets the avaible Diaplay Modes with the Current Display Format
			{
			pD3d->EnumAdapterModes(D3DADAPTER_DEFAULT, currentDisplatFormat , mode, &displayMode);

			wsprintf(buf, L"%u x %u   fq: %u", displayMode.Width, displayMode.Height, displayMode.RefreshRate);
			SendMessage((HWND)hWndCBResolution , CB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
			}
		delete[] format;

			//Checks and loads BackBuffer Suported
		if( pD3d->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, D3DFMT_R8G8B8, true) == D3D_OK )
			{
			SendMessage((HWND)hWndCBBackBufer , CB_ADDSTRING, (WPARAM)0, (LPARAM)L"R8G8B8");
			}
		if( pD3d->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, D3DFMT_A8B8G8R8, true) == D3D_OK )
			{
			SendMessage((HWND)hWndCBBackBufer , CB_ADDSTRING, (WPARAM)0, (LPARAM)L"A8B8G8R8");
			}
		if( pD3d->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, D3DFMT_X8R8G8B8, true) == D3D_OK )
			{
			SendMessage((HWND)hWndCBBackBufer , CB_ADDSTRING, (WPARAM)0, (LPARAM)L"X8R8G8B8");
			}

			//Order of default format to show
		if( (presentParameters.BackBufferFormat = D3DFMT_X8R8G8B8 , SendMessage((HWND)hWndCBBackBufer ,
			CB_SELECTSTRING, (WPARAM)-1, (LPARAM)L"X8R8G8B8") == CB_ERR) )
			if( (presentParameters.BackBufferFormat = D3DFMT_A8B8G8R8 , 
				SendMessage((HWND)hWndCBBackBufer , CB_SELECTSTRING, (WPARAM)-1, (LPARAM)L"A8B8G8R8") == CB_ERR) )
				if( (presentParameters.BackBufferFormat = D3DFMT_R8G8B8 , 
					SendMessage((HWND)hWndCBBackBufer , CB_SELECTSTRING, (WPARAM)-1, (LPARAM)L"R8G8B8") == CB_ERR) )
					{
					//If there is no format suported then say no format, the suported format will not change
					//again
					SendMessage((HWND)hWndCBBackBufer , CB_ADDSTRING, (WPARAM)0, (LPARAM)L"No Format");
					SendMessage((HWND)hWndCBBackBufer , CB_SELECTSTRING, (WPARAM)-1, (LPARAM)L"No Format");
					}

		
			//Checks and loads Depth/Stencil Suported
		if( SUCCEEDED(pD3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, D3DUSAGE_DEPTHSTENCIL, 
			D3DRTYPE_SURFACE, D3DFMT_D24S8)) )
			SendMessage((HWND)hWndCBDS , CB_ADDSTRING, (WPARAM)0, (LPARAM)L"D24S8");

		if( SUCCEEDED(pD3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, D3DUSAGE_DEPTHSTENCIL, 
			D3DRTYPE_SURFACE, D3DFMT_D24X8)) )
			SendMessage((HWND)hWndCBDS , CB_ADDSTRING, (WPARAM)0, (LPARAM)L"D24X8");

		if( SUCCEEDED(pD3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, D3DUSAGE_DEPTHSTENCIL, 
			D3DRTYPE_SURFACE, D3DFMT_D16_LOCKABLE)) )
			SendMessage((HWND)hWndCBDS , CB_ADDSTRING, (WPARAM)0, (LPARAM)L"D16 Lockable");

			//Order of default format to show
		if( (presentParameters.AutoDepthStencilFormat = D3DFMT_D24S8 ,
			SendMessage((HWND)hWndCBDS , CB_SELECTSTRING, (WPARAM)-1, (LPARAM)L"D24S8") == CB_ERR) )
			if( (presentParameters.AutoDepthStencilFormat = D3DFMT_D24X8 ,
				SendMessage((HWND)hWndCBDS , CB_SELECTSTRING, (WPARAM)-1, (LPARAM)L"D24X8") == CB_ERR) )
				if( (presentParameters.AutoDepthStencilFormat = D3DFMT_D16_LOCKABLE ,
					SendMessage((HWND)hWndCBDS , CB_SELECTSTRING, (WPARAM)-1, (LPARAM)L"D16 Lockable") == CB_ERR) )
					{
					//If there is no format suported then say no format, the suported format will not change
					//again
					SendMessage((HWND)hWndCBDS , CB_ADDSTRING, (WPARAM)0, (LPARAM)L"No Format");
					SendMessage((HWND)hWndCBDS , CB_SELECTSTRING, (WPARAM)-1, (LPARAM)L"No Format");
					}
		//Update the compatibility label
		UpdateLabelCompatibility(hDlg);
		
		return true;
	}


	void CE_SetDisplayModeClick(HWND hDlg)
	{
		if( ChangeDisplaySettings( &devMode, 0 ) != DISP_CHANGE_SUCCESSFUL )
			MessageBox(hDlg, L"The video mode could not be changed", L"Display Mode", MB_ICONEXCLAMATION | MB_OK);

		pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &currentDisplayMode);
		UpdateLabelCurrentDisplayMode(currentDisplayMode, hDlg);
		return;
	}


	void CE_BackBufferSelectionChange(HWND hDlg, HWND control)
	{
		int index = SendMessage(control, CB_GETCURSEL, NULL, NULL);
		int len = SendMessage(control, CB_GETLBTEXTLEN, (WPARAM)index, NULL);
		wchar_t* format = new wchar_t[len];
		SendMessage(control, CB_GETLBTEXT, (WPARAM)index, (LPARAM)format);

		//Parsing the BackBuffer format
		if( wcscmp(format,L"X8R8G8B8") == 0 )
			presentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
		else
			presentParameters.BackBufferFormat = (D3DFORMAT)NULL;//Unknown


			//Checks if the selections are compatible
		UpdateLabelCompatibility(hDlg);

		//delete[] format;
		return;
	}

	void CE_ResolutionSelectionChange(HWND hDlg, HWND control)
	{
		HWND hSetDisplayMode = GetDlgItem(hDlg,IDC_SETDISPLAYMODE);
		EnableWindow(hSetDisplayMode, true);
						
		int index = SendMessage(control, CB_GETCURSEL, NULL, NULL);
		int len = SendMessage(control, CB_GETLBTEXTLEN, (WPARAM)index, NULL);
		wchar_t* format = new wchar_t[len];
		SendMessage(control, CB_GETLBTEXT, (WPARAM)index, (LPARAM)format);
		
		//Parsing the Display format
		swscanf_s(format, L"%u x %u   fq: %u", &devMode.dmPelsWidth, &devMode.dmPelsHeight, &devMode.dmDisplayFrequency);

		//delete[] format;
		return;
	}

	void CE_DepthStencilSelectionChange(HWND hDlg, HWND control)
	{
		int index = SendMessage(control, CB_GETCURSEL, NULL, NULL);
		int len = SendMessage(control, CB_GETLBTEXTLEN, (WPARAM)index, NULL);
		wchar_t* format = new wchar_t[len];
		SendMessage(control, CB_GETLBTEXT, (WPARAM)index, (LPARAM)format);

		//Parsing the BackBuffer format
		if( wcscmp(format,L"D24S8") == 0 )
			presentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
		else
			if( wcscmp(format,L"D24X8") == 0 )
				presentParameters.AutoDepthStencilFormat = D3DFMT_D24X8;
			else
				if( wcscmp(format,L"D16 Lockable") == 0 )
					presentParameters.AutoDepthStencilFormat = D3DFMT_D16_LOCKABLE;
				else
					presentParameters.AutoDepthStencilFormat = (D3DFORMAT)NULL;//Unknown


			//Checks if the selections are compatible
		UpdateLabelCompatibility(hDlg);
		
		//delete[] format;
	}

	void UpdateLabelCompatibility(HWND hDlg)
	{
		pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &currentDisplayMode);
		UpdateLabelCurrentDisplayMode(currentDisplayMode, hDlg);

		if( pD3d->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, devType, currentDisplayMode.Format, 
			presentParameters.BackBufferFormat, presentParameters.AutoDepthStencilFormat) == D3D_OK )
			SendDlgItemMessage(hDlg, IDC_COMPATIBLE , WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"Compatible");
		else
			SendDlgItemMessage(hDlg, IDC_COMPATIBLE , WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"Not Compatible");
		return;
	}

	void UpdateLabelCurrentDisplayMode(D3DDISPLAYMODE currentDisplayMode, HWND hDlg)
	{
			//Shows the current format and resolution
		wchar_t buf[100];
		wchar_t* format= new wchar_t[30];
		#pragma region Get format string
			switch( currentDisplayMode.Format )
				{	
				case D3DFMT_X8R8G8B8: 
					wcscpy_s(format, sizeof(L"X8R8G8B8"), L"X8R8G8B8");
					break;
				case D3DFMT_A8R8G8B8:
					wcscpy_s(format, sizeof(L"R8G8B8"), L"R8G8B8");
					break;
				case D3DFMT_R8G8B8:
					wcscpy_s(format, sizeof(L"R8G8B8"), L"R8G8B8");
					break;
				default:
				wcscpy_s(format, sizeof(L"Unknown"), L"Unknown");
				}
		#pragma endregion
		wsprintf(buf, L"%u x %u     fmt: %s", currentDisplayMode.Width, currentDisplayMode.Height, format);
		SendDlgItemMessage(hDlg, IDC_DISPLAYFORMAT, WM_SETTEXT, (WPARAM)NULL, (LPARAM)buf);

			//Shows the current Aspect Ratio
		float ratio = ((float)currentDisplayMode.Width/currentDisplayMode.Height);
		if( ( (1.33333333333333-EPI) < ratio ) && ( ratio < (1.33333333333333+EPI) ) )
			SendDlgItemMessage(hDlg, IDC_ASPECTRATIO, WM_SETTEXT, (WPARAM)0, (LPARAM)L"4/3");
		else
			if( ( (1.77777777777777-EPI) < ratio ) && ( ratio < (1.77777777777777+EPI) ) )
				SendDlgItemMessage(hDlg, IDC_ASPECTRATIO, WM_SETTEXT, (WPARAM)0, (LPARAM)L"16/9");
			else
				if( ( (1.6-EPI) < ratio ) && ( ratio < (1.6+EPI) ) )
					SendDlgItemMessage(hDlg, IDC_ASPECTRATIO, WM_SETTEXT, (WPARAM)0, (LPARAM)L"16/10");
					
		delete[] format;
		return;
	}

}