////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					Input Manager				  //////////////////////////////////////
///////////////////////////////////					for Mouse					  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <dinput.h>

class MouseMG
	{
	private:
		HRESULT hr;
	public:
		MouseMG();
		~MouseMG(void);

			//Variables, not protected because dont want to impose 
			//the way to work

		//Mouse device
		LPDIRECTINPUTDEVICE pDInputMouse;
		//Mouse state
		DIMOUSESTATE curState;
		DIMOUSESTATE prevState;

		//Absolute position
		float x;
		float y;

			//Basic Functionality, it is in the user to respect 
			//encapsulation or not

		//Initialize the Direct Input Device, before this 
		//the Manager is unusable
		bool Initialize(const LPDIRECTINPUT pDInput, HWND hWnd);

		//Update the mouse state with the current one,
		//and re-acquire the device if needed
		bool UpdateState();

		//Says if the Left Click was Pressed
		bool MouseMG::PressedLeftClick();
		//Says if the Left Click was Release
		bool MouseMG::ReleaseLeftClick();
		//Says if the Right Click was Pressed
		bool MouseMG::PressedRightClick();
		//Says if the Right Click was Release
		bool MouseMG::ReleaseRightClick();
		//Returns the current movement of the middle wheel
		float MouseMG::RelativeZMove();


	};

