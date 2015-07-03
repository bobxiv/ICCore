#pragma once

#include "stdafx.h"
#include "..\\Header\\ConfigDialogProc.h"

INT_PTR CALLBACK App::DialogProc(HWND hDlg,UINT uMessage,WPARAM wParam,LPARAM lParam)
{
switch(uMessage)
 {
 case WM_INITDIALOG:
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
			   MyApp* pMyApp = MyApp::GetSingleton();
			   pMyApp->presentParameters= ::presentParameters;
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
			   delete[] txt;
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