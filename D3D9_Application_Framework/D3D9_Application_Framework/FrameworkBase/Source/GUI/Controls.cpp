#include "stdafx.h"

#include "..\\..\\..\\MyApp.h"
//#include "..\\..\\Header\\GUI\\GUIBound.h"
#include "..\\..\\Header\\GUI\\GUIObject.h"
#include "..\\..\\Header\\GUI\\Controls.h"
#include "..\\..\\Header\\GUI\\Text.h"
//#include "..\\..\\Header\\GUI\\GUIBound.h"

namespace GUIFramework
{

	#pragma region Button
	Button::Button()
		{
		Width = 136;
		Height = 53/2;

		OnMouseClickHandler = NULL;
		OnMouseClickDownHandler = NULL;
		OnMouseOverHandler  = NULL;
		OnMouseAwayHandler  = NULL;

			// Visual Representation
			//----------------------

			//The back of the button
		spriteBack.SetPosition( this->Position );
		spriteBack.SetTexture(::MyApp::GetSingleton()->pControlsTexture);//this button default sprite
		AppRectangle Rect( AppPoint(0,1), AppPoint(136,54) );
		spriteBack.SetRenderRectangle(Rect);
		spriteBack.SetWidth(136);
		spriteBack.SetHeight(53/2);//flatter

		spriteBack.Opacity = 0.60f;
		spriteBack.Visible = true;
		Representation.push_back(&spriteBack);
			
			//The front of the button
		spriteFront.SetPosition( AppPoint(this->Position.X-1, this->Position.Y-(this->Height*0.1f)) );
		spriteFront.SetTexture(::MyApp::GetSingleton()->pControlsTexture);//this button default sprite
		AppRectangle Rect2( AppPoint(137,1), AppPoint(250,54) );
		spriteFront.SetRenderRectangle(Rect2);
		spriteFront.SetWidth(133);
		spriteFront.SetHeight(51/2);//flatter

		spriteFront.Opacity = 1.0f;
		spriteFront.Visible = false;
		Representation.push_back(&spriteFront);

			//The Caption Text
		CaptionText.SetPosition(this->Position);
		CaptionText.SetAlignment(DT_CENTER|DT_VCENTER|DT_NOCLIP);
		CaptionText.SetWidth(136);
		CaptionText.SetHeight(53/2);
		Representation.push_back(&CaptionText);

			// Collision Representation
			//-------------------------

			//coordinates in local space, defined from (-0.5,0.5) to (0.5,-0.5)
		D3DXVECTOR2 A(-0.5,0.5);
		D3DXVECTOR2 B(0.5,-0.5);
		GUIRectangle* bound = new GUIFramework::GUIRectangle(dynamic_cast<GUIObject*>(this),A,B);
		CollisionShape = bound;

		}

	bool Button::AddHandler(HandlerType HT, bool (*Handler)() )
	{
	switch(HT)
		{
		case HandlerType::OnMouseClickHType:
			{
			OnMouseClickHandler = (bool(*)(int,int))Handler;
			}
			break;
		case HandlerType::OnMouseClickDownHType:
			{
			OnMouseClickDownHandler = (bool(*)())Handler;
			}
			break;
		case HandlerType::OnMouseOverHandlerHType:
			{
			OnMouseOverHandler = Handler;
			}
			break;
		case HandlerType::OnMouseAwayHandlerHType:
			{
			OnMouseAwayHandler = Handler;
			}
			break;
		default:
			return false;
		}
	
	return true;
	}

	int Button::SetWidth(int W)
		{
			int aux = Width;
			Width = W;

			spriteBack.SetWidth(W);
			spriteFront.SetWidth(W);
			CaptionText.SetWidth(W);
			
			return aux;
		}

	int Button::SetHeight(int H)
		{
			int aux = Height;
			Height = H;

			spriteBack.SetHeight(H);
			spriteFront.SetHeight(H);
			CaptionText.SetHeight(H);

			return aux;
		}

	std::wstring Button::SetCaption(std::wstring txt)
	{
		return CaptionText.SetText(txt);
	}

	AppPoint Button::SetPosition(AppPoint& P)
		{
			AppPoint aux = Position;
			Position = P;
			
			spriteBack.SetPosition( Position );
				//The front sprite is always draw when mouse is over, so it is always upper
			spriteFront.SetPosition( AppPoint(Position.X-(this->Width*0.005f), Position.Y-(this->Height*0.025f)) );

			CaptionText.SetPosition(Position);

			return aux;
		}

	#pragma region Control Event Responses & Users methods calls

	void Button::OnMouseLeftClickDown()
	{
			//Click Displacement
		spriteBack.SetPosition( AppPoint(Position.X+(this->Width*0.02f), Position.Y+(this->Height*0.04f)) );
		CaptionText.SetPosition( spriteBack.GetPosition() );

			//Only back visible
		spriteFront.Visible = false;

			//User Handler
		if( OnMouseClickDownHandler != NULL )
			(*OnMouseClickDownHandler)();

		MyApp::GetSingleton()->Error(L"OnMouseLeftClick");
		return;
	}

	void Button::OnMouseLeftClickUp(AppPoint P)
	{
			//User Handler
		if( OnMouseClickHandler != NULL )
		{
			(*OnMouseClickHandler)(P.X, P.Y);
		}
		return;
	}

	void Button::OnMouseOver()
	{
			//Upper Over Position
		spriteBack.SetPosition( spriteFront.GetPosition() );
		CaptionText.SetPosition( spriteFront.GetPosition() );

			//Quick FadeIn of both
		spriteBack.FadeIn(0.075f);
		spriteFront.FadeIn(0.075f);
		spriteFront.Visible = true;

		CaptionText.Color = 0xFF000000;

			//User Handler
		if( OnMouseOverHandler != NULL )
			(*OnMouseOverHandler)();

		MyApp::GetSingleton()->Error(L"OnMouseOver");
		return;
	}

	void Button::OnMouseAway()
	{
			//Normal Position
		spriteBack.SetPosition(this->Position);
		CaptionText.SetPosition( this->Position );

			//Slow FadeOut
		spriteFront.FadeOut(0.15f);

		CaptionText.Color = 0xFFFFFFFF;

			//User Handler
		if( OnMouseAwayHandler != NULL )
			(*OnMouseAwayHandler)();

		MyApp::GetSingleton()->Error(L"OnMouseAway");
		return;
	}

	void Button::OnChar(int Code, short Count, bool Realease)
	{


		return;
	}

	#pragma endregion

	#pragma endregion


	#pragma region Slider

	Slider::Slider(): Minimum(0), Maximum(1), Value(0.5)
		{
		Width = 136;
		Height = 53/2;

		OnMouseClickHandler = NULL;
		OnMouseOverHandler  = NULL;
		OnMouseAwayHandler  = NULL;

			// Visual Representation
			//----------------------

			//The Line of the slider
		spriteLine.SetPosition( AppPoint(this->Position.X,this->Position.Y) );
		spriteLine.SetTexture(::MyApp::GetSingleton()->pControlsTexture);//this button default sprite
		AppRectangle Rect( AppPoint(0,192), AppPoint(93,234-18) );
		spriteLine.SetRenderRectangle(Rect);
		spriteLine.SetWidth(136);
		spriteLine.SetHeight(53/2.5);//flatter

		spriteLine.Opacity = 0.60f;
		spriteLine.Visible = true;
		Representation.push_back(&spriteLine);
			
			//The pivot of the slider
		spritePivot.SetPosition( AppPoint(this->Position.X, this->Position.Y));
		spritePivot.SetTexture(::MyApp::GetSingleton()->pControlsTexture);//this button default sprite
		AppRectangle Rect2( AppPoint(152,192), AppPoint(192,234) );
		spritePivot.SetRenderRectangle(Rect2);
		spritePivot.SetWidth(26);
		spritePivot.SetHeight(53/2);//flatter

		spritePivot.Opacity = 1.0f;
		spritePivot.Visible = true;
		Representation.push_back(&spritePivot);

			// Collision Representation
			//-------------------------

			//coordinates in local space, defined from (-0.5,0.5) to (0.5,-0.5)
		D3DXVECTOR2 A(-0.5,0.5);
		D3DXVECTOR2 B(0.5,-0.5);
		GUIRectangle* bound = new GUIFramework::GUIRectangle(dynamic_cast<GUIObject*>(this),A,B);
		CollisionShape = bound;

		}

	bool Slider::AddHandler(HandlerType HT, bool (*Handler)() )
	{
	switch(HT)
		{
		case HandlerType::OnMouseClickHType:
			{
			OnMouseClickHandler = (bool(*)(int,int))Handler;
			}
			break;
		case HandlerType::OnMouseOverHandlerHType:
			{
			OnMouseOverHandler = Handler;
			}
			break;
		case HandlerType::OnMouseAwayHandlerHType:
			{
			OnMouseAwayHandler = Handler;
			}
			break;
		default:
			return false;
		}
	
	return true;
	}

	int Slider::SetWidth(int W)
		{
			int aux = Width;
			Width = W;

			spriteLine.SetWidth(W);
			spritePivot.SetWidth(W);
			
			return aux;
		}

	int Slider::SetHeight(int H)
		{
			int aux = Height;
			Height = H;

			spriteLine.SetHeight(H);
			spritePivot.SetHeight(H);

			return aux;
		}

	AppPoint Slider::SetPosition(AppPoint& P)
		{
			AppPoint aux = Position;
			Position = P;
			
			spriteLine.SetPosition( Position );
				//The front sprite is always draw when mouse is over, so it is always upper
			spritePivot.SetPosition( Position );
			//spritePivot.SetPosition( AppPoint(Position.X+(Value-Minimum)/(float)(Maximum-Minimum), Position.Y+(Height/2)) );

			return aux;
		}

	int Slider::SetMinimum(int min)
	{	
		int aux = Minimum;
		Minimum = min;

		return aux;
	}

	int Slider::SetMaximum(int max)
	{
		int aux = Maximum;
		Maximum = max;

		return aux;
	}

	int Slider::SetValue(int cur)
	{
		if( cur < Minimum || cur > Maximum )//not possible
			return -1;
		int aux = Value;
		Value = cur;

		return aux;
	}

	#pragma region Control Event Responses & Users methods calls

	void Slider::OnMouseLeftClickDown()
	{
		//	//Click Displacement
		//spriteBack.SetPosition( AppPoint(Position.X+(this->Width*0.02f), Position.Y+(this->Height*0.04f)) );
		//CaptionText.SetPosition( spriteBack.GetPosition() );

		//	//Only back visible
		//spriteFront.Visible = false;

		MyApp::GetSingleton()->Error(L"OnMouseLeftClick");
		return;
	}

	void Slider::OnMouseLeftClickUp(AppPoint P)
	{
			//User Handler
		if( OnMouseClickHandler != NULL )
			(*OnMouseClickHandler)(P.X, P.Y);
		return;
	}

	void Slider::OnMouseOver()
	{
			//Quick FadeIn of both
		spriteLine.FadeIn(0.075f);

			//User Handler
		if( OnMouseOverHandler != NULL )
			(*OnMouseOverHandler)();

		MyApp::GetSingleton()->Error(L"OnMouseOver");
		return;
	}

	void Slider::OnMouseAway()
	{
			//Slow FadeOut
		//spriteLine.FadeOut(0.15f);
		spriteLine.Opacity=0.60f;

			//User Handler
		if( OnMouseAwayHandler != NULL )
			(*OnMouseAwayHandler)();

		MyApp::GetSingleton()->Error(L"OnMouseAway");
		return;
	}

	void Slider::OnChar(int Code, short Count, bool Realease)
	{


		return;
	}

	#pragma endregion

}
