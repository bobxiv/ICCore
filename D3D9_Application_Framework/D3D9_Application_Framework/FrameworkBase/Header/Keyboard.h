////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					Input Manager				  //////////////////////////////////////
///////////////////////////////////					for Keyboard				  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <dinput.h>

class KeyboardMG
	{
	private:
		HRESULT hr;
	public:
		KeyboardMG(): pDInputKeyboard(NULL){}
		~KeyboardMG(void);

			//Variables, not protected because dont want to impose 
			//the way to work

		//Keyboard device
		LPDIRECTINPUTDEVICE pDInputKeyboard;
		//Key Buffer
		char curKeyBuffer[256];
		char prevKeyBuffer[256];

			//Basic Functionality, it is in the user to respect 
			//encapsulation or not

		//Initialize the Direct Input Device, before this 
		//the Manager is unusable
		bool Initialize(const LPDIRECTINPUT pDInput, HWND hWnd);

		//Update the keybuffer state with the current one,
		//and re-acquire the device if needed
		bool UpdateState();

		//Says if DIK was currently pressed
		bool Pressed(int DIK);

		//Says if DIK was release pressed
		bool Release(int DIK);
	};

