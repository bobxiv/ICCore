////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					GUI							  //////////////////////////////////////
///////////////////////////////////					Manager						  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <list>

class MyApp;

//To make the MaterialsLibrary
#include "GUIMaterial.h"

//#include "GUIBound.h"

//#include "GUIImage.h"

#include "Sprite.h"
#include "Controls.h"


namespace GUIFramework
{

		//Structure of the global library of material for the GUI Objects
	struct GUIMaterialsLibrary
	{
			//Global GUI Basic Effect
		GUIBasicEffect BasicEffect;

			//Global Font Description
		D3DXFONT_DESC DefaultFont;

		//Could be fill with others materials

		std::list<GUIMaterial> UserMaterials;

		void Initialize()
		{
			BasicEffect.Initialize();
			
			DefaultFont.Height = 15;
			DefaultFont.Width = 0;
			DefaultFont.Italic = false;
			DefaultFont.MipLevels = 1;
			DefaultFont.CharSet = DEFAULT_CHARSET;
			DefaultFont.OutputPrecision = OUT_DEFAULT_PRECIS;
			DefaultFont.PitchAndFamily =  DEFAULT_PITCH|FF_DONTCARE;
			DefaultFont.Quality = ANTIALIASED_QUALITY;
			//wcscpy( DefaultFont.FaceName, L"Arial");
			//wcscpy( DefaultFont.FaceName, L"Bauhaus 93");
			wcscpy( DefaultFont.FaceName, L"Goudy Stout");
			
			
			return;
		}

		~GUIMaterialsLibrary()
		{
			UserMaterials.clear();
		}

	};

	class MouseState 
	{
	public:
		MouseState(): LeftClickPressed(false), MouseOver(NULL){}

		AppPoint Point;

		bool LeftClickPressed;

		GUIObject* MouseOver;
	};
		
	//Abstract Class
	//This class is the responsible of administrating all the GUI controls,
	//they rendering and events. Suppling the basic library of GUI materials.
	//And giving the description of the screen.
	class GUI
		{
		protected:

				//The DirectX Device to render the sprite
			LPDIRECT3DDEVICE9 pDevice;
			
			std::vector<GUIObject*> Controls;

			MouseState MS;
			
			GUIObject* Focused;

				//Private contructors
			GUI();

			GUI(const GUI&){}

			GUI& operator= (const GUI& ){}

		public:

				//Public destructor
			~GUI();

			virtual void Load() = 0;

			virtual void UnLoad() = 0;

			virtual void RenderGUI(const double elapsedTime); //Base implementation, to be call from the derived GUI

			bool ResolveMousePosition(int x, int y);

			bool ResolveMouseLeftClickDown(AppPoint& P);

			bool ResolveMouseLeftClickUp(AppPoint& P);

			bool ResolveChar(int Code, short Count, bool Realease);

			void GetFrameDimensions(int& W,int& H);
			
		};


}