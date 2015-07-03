#pragma once

#include <Windows.h>

#include "..\\..\\MyApp.h"

	//The procedure variables(All the window/control Events can access & change them)
static DEVMODE devMode;                  //Pretended Display Mode
static LPDIRECT3D9 pD3d;				 //To check the device capabilities
static D3DDEVTYPE	devType;			 //The type of device wanted
static D3DDISPLAYMODE currentDisplayMode;//The display mode at the time of start, 
D3DPRESENT_PARAMETERS presentParameters; //The present parameters selected to create the device

	//DE = Dialog Event 
bool DE_InitializeDialog(HWND hDlg);

	//CE = Control Event	|	 One per (Control,Event) pair
void CE_SetDisplayModeClick(HWND hDlg);

void CE_ResolutionSelectionChange(HWND hDlg, HWND control);

void CE_BackBufferSelectionChange(HWND hDlg, HWND control);

void CE_DepthStencilSelectionChange(HWND hDlg, HWND control);

	//Auxiliary Function
//Checks and updates the current display mode
void UpdateLabelCurrentDisplayMode(D3DDISPLAYMODE currentDisplayMode, HWND hDlg);
//Checks and updates the buffers/display mode compatibility
void UpdateLabelCompatibility(HWND hDlg);
