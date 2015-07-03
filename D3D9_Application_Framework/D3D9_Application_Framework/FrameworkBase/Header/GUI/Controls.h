#pragma once

#include "GUIObject.h"
#include "Sprite.h"
#include "Text.h"


namespace GUIFramework
{

	class Button: public GUIObject
	{
	protected:

		Sprite spriteBack;
		Sprite spriteFront;
		Text   CaptionText;

		bool (*OnMouseClickHandler)(int x, int y);
		bool (*OnMouseClickDownHandler)();
		bool (*OnMouseOverHandler)();
		bool (*OnMouseAwayHandler)();
		
	public:

		Button();
		
		~Button(){}

		enum HandlerType{OnMouseClickHType, OnMouseClickDownHType, OnMouseOverHandlerHType, OnMouseAwayHandlerHType};

		bool AddHandler(HandlerType HT, bool (*Handler)());

		int SetWidth(int W);

		int SetHeight(int H);

		std::wstring SetCaption(std::wstring txt);

		AppPoint SetPosition(AppPoint& P);

		virtual void ResolveEvents()
		{
			//MyApp::GetSingleton()->Mouse.
			OnMouseOver();
			return;
		}


		//	Button Events Call-backs

		virtual void OnMouseLeftClickDown();

		virtual void OnMouseLeftClickUp(AppPoint P);

		virtual void OnMouseOver();

		virtual void OnMouseAway();

		virtual void OnChar(int Code, short Count, bool Realease);
		
	};

	class Slider: public GUIObject
	{
	protected:

		Sprite spriteLine;
		Sprite spritePivot;

		float Minimum;
		float Maximum;
		float Value;

		bool (*OnMouseClickHandler)(int x, int y);
		bool (*OnMouseOverHandler)();
		bool (*OnMouseAwayHandler)();
		
	public:

		Slider();
		
		~Slider(){}

		enum HandlerType{OnMouseClickHType, OnMouseOverHandlerHType, OnMouseAwayHandlerHType};

		bool AddHandler(HandlerType HT, bool (*Handler)());

		int SetWidth(int W);

		int SetHeight(int H);

		AppPoint SetPosition(AppPoint& P);

		int SetMinimum(int min);

		int SetMaximum(int max);

		int SetValue(int cur);

		virtual void ResolveEvents()
		{
			//MyApp::GetSingleton()->Mouse.
			OnMouseOver();
			return;
		}


		//	Button Events Call-backs

		virtual void OnMouseLeftClickDown();

		virtual void OnMouseLeftClickUp(AppPoint P);

		virtual void OnMouseOver();

		virtual void OnMouseAway();

		virtual void OnChar(int Code, short Count, bool Realease);
		
	};

}
