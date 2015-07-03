#pragma once

#include "stdafx.h"
#include "..\\Header\\Mouse.h"

MouseMG::MouseMG(): pDInputMouse(NULL), x(0), y(0)
	{
		ZeroMemory(&curState,sizeof(DIMOUSESTATE));
		ZeroMemory(&prevState,sizeof(DIMOUSESTATE));
	}

MouseMG::~MouseMG(void)
	{
	if( pDInputMouse )
		{
		pDInputMouse->Unacquire();
		pDInputMouse->Release();
		}
	}

bool MouseMG::Initialize(const LPDIRECTINPUT pDInput, const HWND hWnd)
	{
	hr = pDInput->CreateDevice(GUID_SysMouse , &pDInputMouse, NULL);
	if( FAILED(hr) )
		return false;
	
	hr = pDInputMouse->SetDataFormat( &c_dfDIMouse );
	if( FAILED(hr) )
		return false;

	hr = pDInputMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
	if( FAILED(hr) )
		return false;

	hr = pDInputMouse->Acquire();
	if( FAILED(hr) )
		return false;
	return true;
	}

bool MouseMG::UpdateState()
	{
	prevState = curState;
	hr = pDInputMouse->GetDeviceState(sizeof(DIMOUSESTATE) ,(LPVOID)&curState);
	while( hr == DIERR_INPUTLOST ) 
		hr = pDInputMouse->Acquire();
	if( FAILED(hr) )
		return false;

	x += curState.lX; 
	y += curState.lY;
	return true;
	}

bool MouseMG::PressedLeftClick()
	{
	return ( curState.rgbButtons[0] & 0x80 && !(prevState.rgbButtons[0] & 0x80) );
	}

bool MouseMG::ReleaseLeftClick()
	{
	return ( !(curState.rgbButtons[0]) & 0x80 && prevState.rgbButtons[0] & 0x80 );
	}

bool MouseMG::PressedRightClick()
	{
	return ( curState.rgbButtons[1] & 0x80 && !(prevState.rgbButtons[1] & 0x80) );
	}

bool MouseMG::ReleaseRightClick()
	{
	return ( !(curState.rgbButtons[1] & 0x80) && prevState.rgbButtons[1] & 0x80 );
	}

float MouseMG::RelativeZMove()
	{
	return ( curState.lZ );
	}

