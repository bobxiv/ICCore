////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					GUI							  //////////////////////////////////////
///////////////////////////////////					Sprites						  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "..\\ProPrimitives\\Quad.h"
#include "GUIImage.h"

class MyApp;
using namespace ProPrimitive;

namespace GUIFramework
{

	class Sprite: public GUIImage
		{
		private:
				//The DirectX Device to render the sprite
			LPDIRECT3DDEVICE9 pDevice;
				
				//The texture of the sprite
			LPDIRECT3DTEXTURE9 pTexture;
			D3DSURFACE_DESC TextureDesc;
			
			AppRectangle RenderArea;
			
				//The quad(frame) to draw the sprite
			Quad* pspriteQuad;

				//The sprite position in pixels
			AppPoint Position;

			D3DXMATRIX Translation;
			D3DXMATRIX Sizing;
			D3DXMATRIX Rotation;

				//The sprite dimensions in pixels
			int Width;
			int Height;

			float deltax;

			float FadePeriod;
			float FadeTime;
			bool fadingOut;
			bool fadingIn;

				//The sprite clockwise rotation in degrees
			int RotationDegree;

				//If the sprite is to be draw flipped
			bool flipX;
			bool flipY;
	

		public:

			Sprite();

			Sprite(LPDIRECT3DTEXTURE9& pTex);

			Sprite(LPDIRECT3DTEXTURE9& pTex, AppPoint P);

			Sprite(AppPoint P, int W, int H);

			Sprite(AppPoint P, int W, int H, int Degree);

			Sprite(LPDIRECT3DTEXTURE9& pTex, AppPoint P, int W, int H);
			
			~Sprite();

			float Opacity;
			
			float ModulateColor[3];

			virtual void Draw(const double elapsedTime);

			bool FlipX();
			bool FlipY();

			void FadeOut(float period);
			void FadeIn(float period);

				//Setters
			virtual AppPoint SetPosition(AppPoint& P);
			virtual int SetWidth(int W);
			virtual int SetHeight(int H);
			int SetRotation(int Degree);
			LPDIRECT3DTEXTURE9 SetTexture(LPDIRECT3DTEXTURE9 pTex);
			AppRectangle SetRenderRectangle(AppRectangle& R);
			GUIMaterial SetMaterial(GUIMaterial& M);

				//Getters
			AppPoint GetPosition() const;
			int GetWidth() const;
			int GetHeight() const;
			int GetRotation() const;
			LPDIRECT3DTEXTURE9 GetTexture() const; 
			AppRectangle GetRenderRectangle() const;
			GUIMaterial GetMaterial() const;
			
		};


		class Animation: public GUIImage
		{
		private:
				
			
		};

}