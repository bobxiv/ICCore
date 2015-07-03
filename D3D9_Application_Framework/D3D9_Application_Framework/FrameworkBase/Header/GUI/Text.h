////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					GUI							  //////////////////////////////////////
///////////////////////////////////					Sprites						  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "..\\GUI\\GUIImage.h"
#include "..\\ProPrimitives\\Point.h"

namespace GUIFramework
{

	class Text: public GUIImage
	{
	private:

			//The DirectX Device to render the sprite
		LPDIRECT3DDEVICE9 pDevice;

		LPD3DXFONT pFont;
		LPD3DXSPRITE pSprite;

			//The sprite position in pixels
		AppPoint Position;

			//The sprite dimensions in pixels
		int Width;
		int Height;
			
			//Alignement of the text
		unsigned int Alignment;

		std::wstring txt;

	public:

		Text();

		~Text();

		int Color;

		void SetFontDesc(D3DXFONT_DESC fontDesc);

		unsigned int SetAlignment(unsigned int format);

		std::wstring SetText(std::wstring pT);

		virtual AppPoint SetPosition(AppPoint& P);

		virtual int SetWidth(int W);

		virtual int SetHeight(int H);

		virtual void Draw(const double elapsedTime);

	};

}