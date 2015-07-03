#pragma once

#include "stdafx.h"

#include "..\\..\\Header\\GUI\\GUIObject.h"

//extern MyApp* pMyApp;//YOUR APP(ALREADY DECLEARE AND DEFINE IN App.cpp)

namespace GUIFramework
{
	
	GUIObject::GUIObject(): EventType(0), Width(0), Height(0), Representation(NULL), CollisionShape(NULL)
	{}

	GUIObject::~GUIObject()
	{
		Representation.clear();
		if( CollisionShape )
			delete CollisionShape;
	}

	void GUIObject::Render(const double elapsedTime)
	{
		std::list<GUIImage*>::iterator it = Representation.begin();
		while( it != Representation.end() )
		{
			GUIImage* image = *it;
			if( image->Visible == true )
				image->Draw(elapsedTime);
			++it;
		}

		return;
	}

	bool GUIObject::IsInside(AppPoint P)
	{
		return CollisionShape->IsInside(P);
	}

	std::string GUIObject::GetName()
	{
		return Name;
	}

	AppPoint GUIObject::GetPosition()
	{
		return Position;
	}

	int GUIObject::GetWidth()
	{
		return Width;
	}

	int GUIObject::GetHeight()
	{
		return Height;
	}
	

}