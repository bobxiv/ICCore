////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					GUI							  //////////////////////////////////////
///////////////////////////////////					Objects Base Class	 		  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <string>

#include "..\\ProPrimitives\\Point.h"

#include "GUIBound.h"
#include "GUIImage.h"

using namespace ProPrimitive;

namespace GUIFramework
{

	//Abstract Class
	//The Base of all the controls of the GUI, exposed the nesesary functionality for the GUI to 
	//render and direct events.
	//And have the basic elements of any control, visual representation, collision representation, etc
	class GUIObject
	{
	protected:

			//Name of the object
		std::string Name;

		int EventType;

		AppPoint Position;

			//Controls the Representations and Collision Shape area
			//Note: it is not nessesary a bounding box for the representation but yes a GOOD aproximation
		int Width;
		int Height;
		
			//List of visible representations of the object, they can be visible or not at a given time
		std::list<GUIImage*> Representation;

			//The collision shape of the object
		GUIBound* CollisionShape;

	public:

		GUIObject();

		~GUIObject();

			//Basic functionality exposed for the GUI, to Render and manage direct Events
		void Render(const double elapsedTime);
		bool IsInside(AppPoint P);

			//Events fot the control, to manage the FSM that is the control,
			//and call the users defined Event Handlers
		virtual void ResolveEvents() = 0;
		virtual void OnMouseOver() = 0;
		virtual void OnMouseAway() = 0;
		virtual void OnMouseLeftClickDown() = 0;
		virtual void OnMouseLeftClickUp(AppPoint P) = 0;
		virtual void OnChar(int Code, short Count, bool Realease) = 0;

			//Basic Getters
		std::string GetName();
		AppPoint GetPosition();
		int GetWidth();
		int GetHeight();
		
	};

}

	