////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					Pro Primitive			      //////////////////////////////////////
///////////////////////////////////					Quad						  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

//It initialize Quad with an Effect from the GUIMaterialLibrary
//#include "..\\GUI\\GUI.h"
#include "..\\..\\Header\\Utility.h"

class MyApp;

namespace ProPrimitive
{
		////The Quad VertexElement structure Declaration
	D3DVERTEXELEMENT9 QuadVertexElement[];

	//This is a very primitive structure, we say it can't even draw itself
	//because it don't store the DX Device, it ask for it every time it Draws
	//and it is not encapsulated
	//
	//It's responsibility is to represent a quad, being able to draw itself
	//using some specific Effect
	//It can be draw in the plane or in the space
	//
	//Important: the instance draw is intended for vertexs that only change position
	//by a transformation, and the static draw for vertexs already transformed
	class Quad
	{
	public:

		#pragma region VertexDefinition
		struct QuadVertex
		{
			struct XYZ
			{
				float x;
				float y;
				float z;
			};
			struct UV
			{
				float u;
				float v;
			};
		
			XYZ Position;
			UV Texture;
		};
		#pragma endregion
		
		Quad()
		{}

		Quad(LPDIRECT3DDEVICE9 pDev);

		Quad(const D3DXVECTOR2& A, const D3DXVECTOR2& B, LPDIRECT3DDEVICE9 pDev);

		Quad(float Ax, float Ay, float Bx, float By, LPDIRECT3DDEVICE9 pDev);

		Quad(const D3DXVECTOR3& A, const D3DXVECTOR3& B, const D3DXVECTOR3& C,
				const D3DXVECTOR3& D, LPDIRECT3DDEVICE9 pDevice);

		Quad(const D3DXVECTOR2& A, const D3DXVECTOR2& B, LPDIRECT3DDEVICE9 pDev, LPD3DXEFFECT pEff);

		Quad(float Ax, float Ay, float Bx, float By, LPDIRECT3DDEVICE9 pDev, LPD3DXEFFECT pEff);

		Quad(const D3DXVECTOR3& A, const D3DXVECTOR3& B, const D3DXVECTOR3& C,
				const D3DXVECTOR3& D, LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT pEff);

		Quad(const Quad &B);

		~Quad();

			//The 4 vertexs
		QuadVertex Vertexs[4];

			//The Effect to render, it needs to RESPECT some GLOBAL VARIABLES NAMES!!!
		LPD3DXEFFECT pEffect;

			//The Buffers
		LPDIRECT3DVERTEXBUFFER9 pVBuffer;
		LPDIRECT3DINDEXBUFFER9  pIBuffer;

		void ReCreateVertexBuffer(LPDIRECT3DDEVICE9 pDev);

			//Draws the quad using the passed Device and texture
			//Params:
			//- pDev		The DX Device to render
			//- mPV			The projXview transformation
			//- mW			The World transformation
			//- pTexture	The texture to render the quad
		void Draw(LPDIRECT3DDEVICE9 pDev, const D3DXMATRIX& mPV, const D3DXMATRIX& mW, LPDIRECT3DTEXTURE9 pTexture);

			//Draws the quad using the passed Device and texture, 
			//it is render directly in the homogenious space
			//Params:
			//- pDev		The DX Device to render
			//- pTexture	The texture to render the quad
		void Draw(LPDIRECT3DDEVICE9 pDev, LPDIRECT3DTEXTURE9 pTexture);

			//Statics Draw methods
			//Intended to be used with a quad that you have the translated vertexs, and not the translation
			//Params:
			//- V			The vertexs to render
			//- pDev		The DX Device to render
			//- pEff		The effect to render
			//- pTexture	The texture to render the quad
		static void Draw(QuadVertex* V, LPDIRECT3DDEVICE9 pDev, LPD3DXEFFECT pEff, LPDIRECT3DTEXTURE9 pTexture);

	};

}

