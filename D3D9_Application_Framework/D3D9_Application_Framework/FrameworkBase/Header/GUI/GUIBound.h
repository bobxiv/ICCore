////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					GUI							  //////////////////////////////////////
///////////////////////////////////					Bounds						  //////////////////////////////////////
///////////////////////////////////					For Collisions Detection	  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <d3dx9.h>

#include "..\\ProPrimitives\\Point.h"

namespace ProPrimitive
{
	class AppPoint;
}

using namespace ProPrimitive;

namespace GUIFramework
{

	class GUIObject;

	//Abstract Class
	//This class is the one that responds if there were collision with the mouse
	//(one point) and the control.
	//Could be any shape, but all will need to derive from this
	class GUIBound
		{
		protected:
	
				//The collision area inherits the position and width/height
				//from his parent. This is beacause collision coordinates are in
				//local space(control space)
			GUIObject* Owner;

		public:

			GUIBound()
			{}

			~GUIBound()
			{}

			//Responds if the point P falls inside of this shape, every collision shape 
			//should when a point is inside of itself
			//
			//- P the point to test collision against
			virtual bool IsInside(AppPoint P) = 0;

		};

	
	class GUIRectangle: public GUIBound
		{
		private:

				//Local Coordinates in the area ([-0.5,0.5],[-0.5,0.5])
			D3DXVECTOR2 A;

			D3DXVECTOR2 B;

		public:

			GUIRectangle(GUIObject* obj);
			  
			GUIRectangle(GUIObject* obj, D3DXVECTOR2 UpperLeft, D3DXVECTOR2 LowerRight);

			D3DXVECTOR2 SetB(D3DXVECTOR2& P2);

			D3DXVECTOR2 SetA(D3DXVECTOR2& P1);

			virtual bool IsInside(AppPoint P);

		};
}



