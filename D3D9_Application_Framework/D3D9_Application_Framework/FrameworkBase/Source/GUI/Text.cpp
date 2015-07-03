#include "stdafx.h"

#include "..\\..\\Header\\GUI\\Text.h"
#include "..\\..\\Header\\GUI\\GUIImage.h"
#include "..\\..\\..\\MyApp.h"
#include "..\\..\\Header\\GUI\\GUI.h"

namespace GUIFramework
{

	extern GUIMaterialsLibrary MaterialsLibrary;

	Text::Text(): Width(0), Height(0), Color(0xFFFFFFFF)
		{
			pDevice = MyApp::GetSingleton()->GetD3DDevice();
			Material = GUIFramework::MaterialsLibrary.BasicEffect;
			
			D3DXCreateSprite(pDevice, &pSprite);
			D3DXCreateFontIndirect(pDevice, &GUIFramework::MaterialsLibrary.DefaultFont, &pFont);
			
			Alignment = DT_LEFT|DT_NOCLIP;
		}

	Text::~Text()
	{
		txt.clear();
		pFont->Release();
		pSprite->Release();
	}

	void Text::SetFontDesc(D3DXFONT_DESC fontDesc)
	{
		D3DXCreateFontIndirect(pDevice, &GUIFramework::MaterialsLibrary.DefaultFont, &pFont);
		return;
	}

	unsigned int Text::SetAlignment(unsigned int format)
	{
		unsigned int aux = Alignment;
		Alignment = format;

		return aux;
	}

	std::wstring Text::SetText(std::wstring T)
	{
		std::wstring aux = txt;
		txt = T;
		return aux;
	}

	AppPoint Text::SetPosition(AppPoint& P)
	{
		AppPoint aux = Position;
		Position = P;

		return aux;
	}

	int Text::SetWidth(int W)
	{
		int aux = Width;
		Width = W;

		return aux;
	}

	int Text::SetHeight(int H)
	{
		int aux = Height;
		Height = H;

		return aux;
	}

	void Text::Draw(const double elapsedTime)
	{
		RECT fontRect;
		SetRect(&fontRect, Position.X, Position.Y, Position.X+Width, Position.Y+Height);
		pFont->DrawText(NULL, txt.c_str(), -1, &fontRect, Alignment, Color);
	}

}