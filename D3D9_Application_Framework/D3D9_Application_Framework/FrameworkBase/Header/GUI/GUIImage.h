////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					GUI							  //////////////////////////////////////
///////////////////////////////////					Sprites						  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "..\\ProPrimitives\\Point.h"
#include "GUIMaterial.h"

namespace ProPrimitive
{
	class AppPoint;
}

//namespace std
//{
//	class wstring;
//}

using namespace ProPrimitive;


namespace GUIFramework
{

	//Abstract Class
	//This class is the Drawing of any control visual representation,
	//being this ones sprites, animations, videos, etc.
	//It exposes the functionality for GUIObject to render all the parts
	class GUIImage 
	{
	protected:
		
			//The GUI material, it have the effect and any other data
		GUIMaterial Material;

	public:

		GUIImage(): Visible(true)
		{}

		~GUIImage()
		{}

			//Says if the representation should currently be drawn 
		bool Visible;

		//Draws the representation, every representation should know of to be Drawn
		//
		//- elapsedTime the elapsed time between this frame and the previous
		virtual void Draw(const double elapsedTime) = 0;

			//Basic Setters that should be implemented
		virtual AppPoint SetPosition(AppPoint& P) = 0;
		virtual int SetWidth(int W) = 0;
		virtual int SetHeight(int H) = 0;

	};

}