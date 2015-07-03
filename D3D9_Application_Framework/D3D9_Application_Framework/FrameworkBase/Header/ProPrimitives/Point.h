////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					Pro Primitive			      //////////////////////////////////////
///////////////////////////////////					Point						  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace ProPrimitive
{

	//Basic structure for the GUI
	//It is used to represent a point in the screen, so it is expressed
	//in integers
	class AppPoint
		{
		public:
			AppPoint(): X(0), Y(0){}

			AppPoint(int a, int b): X(a), Y(b){}

			AppPoint& operator=(const AppPoint& B);

			bool operator==(const AppPoint& B) const;

			int X;

			int Y;
		};

	class AppRectangle
	{
	public:
			AppRectangle(){}

			AppRectangle(AppPoint& a, AppPoint& b): P1(a), P2(b){}

			AppRectangle& operator=(const AppRectangle& B);

			bool operator==(const AppRectangle& B) const;

			AppPoint P1;

			AppPoint P2;
	};

}