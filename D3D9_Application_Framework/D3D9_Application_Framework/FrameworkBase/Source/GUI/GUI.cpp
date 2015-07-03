#pragma once

#include "stdafx.h"
#include "..\\..\\Header\\GUI\\GUI.h"

#include "..\\..\\..\\MyApp.h"

namespace GUIFramework
{

		//The Global GUI materials library
	GUIMaterialsLibrary MaterialsLibrary;

	GUI::GUI(): Focused(NULL)
		{
		pDevice = MyApp::GetSingleton()->GetD3DDevice();
		}

	GUI::~GUI()
		{
			Controls.clear();
			return;
		}

	#pragma region Events Resolvers

	bool GUI::ResolveMousePosition(int x, int y)
	{
	if( MS.LeftClickPressed && MS.MouseOver != NULL )//if click donw in a control 
		return true;  		    				     //until down won't get new MouseOver
		

	if( MS.MouseOver != NULL && MS.MouseOver->IsInside( AppPoint(x,y) ) )
		return true;
	else
		{
		if( MS.MouseOver != NULL )
			MS.MouseOver->OnMouseAway();
		MS.MouseOver = NULL;
		
		for(int i=0;i < Controls.size(); ++i)
			{
				if(Controls[i]->IsInside( AppPoint(x,y) ))
				{
					Controls[i]->OnMouseOver();
					MS.MouseOver = Controls[i];
				}
			}
		}

	return true;
	}

	bool GUI::ResolveMouseLeftClickDown(AppPoint& P)
	{
		if( MS.MouseOver != NULL )
		{
			Focused = MS.MouseOver;
			MS.LeftClickPressed = true;
			MS.MouseOver->OnMouseLeftClickDown();
		}
		return true;
	}

	bool GUI::ResolveMouseLeftClickUp(AppPoint& P)
	{
		if( Focused != NULL && Focused->IsInside(P) )//if effectively click
		{											 //it means release on focused
			Focused->OnMouseLeftClickUp(P);
			Focused->OnMouseOver();
		}else
			if( MS.MouseOver != NULL )
			{
				MS.MouseOver->OnMouseAway();
				MS.MouseOver = NULL;
			}
		MS.LeftClickPressed = false;

		return true;
	}

	bool GUI::ResolveChar(int Code, short Count, bool Realease)
	{
		if( Focused != NULL )
			Focused->OnChar(Code, Count, Realease);
		return true;
	}
	
	#pragma endregion 

	void GUI::RenderGUI(const double elapsedTime)
	{
		pDevice->BeginScene();
			for(int i=0;i < Controls.size(); ++i)
			{
				Controls[i]->Render(elapsedTime);
			}
		pDevice->EndScene();

		pDevice->Present(NULL,NULL,NULL,NULL);
	}

	//bool GUI::injectMousePosition(int x, int y)
	//{
	//	MS.Point.X = x;
	//	MS.Point.Y = y;
	//	return true;
	//}

	void GUI::GetFrameDimensions(int& W,int& H)
	{
		MyApp::GetSingleton()->GetBackBufferDimension(W,H);
		return;
	}




}