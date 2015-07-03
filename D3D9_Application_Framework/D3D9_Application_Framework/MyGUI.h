////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					GUI							  //////////////////////////////////////
///////////////////////////////////					Manager						  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>

#include "FrameworkBase\\Header\\GUI\\GUI.h"

namespace GUIFramework
{

	//Singleton Class
	//The App GUI inside DirectX, this is where you have to put your code for the GUI.
	//This clase derives from the base GUI class. The responsibility
	//from the base App is to create/initialize GUI controls, GUI events, etc
	//
	//The specifics of your App GUI are responsibility from this clase
	//
	//IMPORTANT: this class links with the App once the App has created a DirectX Device
	//and this GUI will render by default
	class MyGUI: public GUI
		{
		private:
				//Singleton Variable
			static MyGUI* Singleton;


			//-----------------------
			//	Your States
			//-----------------------

				//YOUR CODE HERE
				//THIS IS AN EXAMPLE
			//GUIFramework::Sprite mySprite;
			

			//-----------------------

				//Private contructors
			MyGUI(){}

			MyGUI(const MyGUI&){}

			MyGUI& operator= (const MyGUI& ){}

		public:

				//Public destructor
			~MyGUI(){}
			
				//Gets the unique instance of GUI
			static MyGUI* GetSingleton();


			//------------------------
			//	Your Public Methods
			//------------------------

			//virtual void ResolveEvents();

			virtual void Load();

			virtual void UnLoad();

			virtual void RenderGUI(const double elapsedTime);

			//------------------------
			//	Your Public Properties
			//------------------------

			LPDIRECT3DDEVICE9 pPublicDevice;//use to re-create the grid

			GUIFramework::Button mLeftButton;
			GUIFramework::Button mRightButton;

			GUIFramework::Button   mDetalle;

			GUIFramework::Button   mPlayToggle;

			GUIFramework::Button   mSave;
			GUIFramework::Button   mLoad;
			
		};


}