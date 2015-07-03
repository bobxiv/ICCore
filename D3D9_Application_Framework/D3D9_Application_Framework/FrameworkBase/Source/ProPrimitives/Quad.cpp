#pragma once

#include "stdafx.h"
#include "..\\..\\Header\\ProPrimitives\\Quad.h"

#include "..\\..\\..\\MyApp.h"

namespace GUIFramework
{
	extern GUIMaterialsLibrary MaterialsLibrary;
}

namespace ProPrimitive
{

		//The Quad VertexElement structure Definition
	D3DVERTEXELEMENT9 QuadVertexElement[] =
			{
				{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
				{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
				D3DDECL_END()
			};

	#pragma region Quad Contructors and Destructors

	Quad::Quad(const Quad &B)
	{
		this->pEffect  = B.pEffect;
		if( this->pEffect )
			pEffect->AddRef();
		this->pVBuffer = B.pVBuffer;
		if( this->pVBuffer)
			this->pVBuffer->AddRef();
		this->pIBuffer = B.pIBuffer;
		if( this->pIBuffer )
			this->pIBuffer->AddRef();
		std::copy(B.Vertexs, B.Vertexs+4, this->Vertexs);
	}

	Quad::Quad(LPDIRECT3DDEVICE9 pDev)
	{
		//	   (-0.5,0.5) ----- *
		//			|			|			Normalize sprite
		//			|			|
		//			|			|
		//			* ---- (0.5,-0.5)
		
			//uses the same effect that BasicEffect from GUIMaterials
		pEffect = GUIFramework::MaterialsLibrary.BasicEffect.pEffect;

		Vertexs[0].Position.x = -0.5f;
		Vertexs[0].Position.y = 0.5f;
		Vertexs[0].Position.z = 0.0f;

		Vertexs[1].Position.x = 0.5f;
		Vertexs[1].Position.y = 0.5f;
		Vertexs[1].Position.z = 0.0f;

		Vertexs[2].Position.x = 0.5f;
		Vertexs[2].Position.y = -0.5f;
		Vertexs[2].Position.z = 0.0f;

		Vertexs[3].Position.x = -0.5f;
		Vertexs[3].Position.y = -0.5f;
		Vertexs[3].Position.z = 0.0f;
		

		Vertexs[0].Texture.u = 0;
		Vertexs[0].Texture.v = 0;

		Vertexs[1].Texture.u = 1;
		Vertexs[1].Texture.v = 0;

		Vertexs[2].Texture.u = 1;
		Vertexs[2].Texture.v = 1;

		Vertexs[3].Texture.u = 0;
		Vertexs[3].Texture.v = 1;

		int Indexs[6];
		Indexs[0]=0;
		Indexs[1]=1;
		Indexs[2]=2;
		Indexs[3]=2;
		Indexs[4]=3;
		Indexs[5]=0;

		pVBuffer = NULL;
		pIBuffer = NULL;
		CreateFillBuffer<Quad::QuadVertex>(pDev, &pVBuffer, 4, Vertexs, NULL);
		CreateFillIndexBuffer<int>(pDev, &pIBuffer, 6, Indexs, D3DFMT_INDEX32);

	}

	Quad::Quad(const D3DXVECTOR2& A, const D3DXVECTOR2& B, LPDIRECT3DDEVICE9 pDev)
	{
		//			A --------- *
		//			|			|
		//			|			|
		//			|			|
		//			* --------- B
		
			//uses the same effect that BasicEffect from GUIMaterials
		pEffect = GUIFramework::MaterialsLibrary.BasicEffect.pEffect;

		Vertexs[0].Position.x = A.x;
		Vertexs[0].Position.y = A.y;
		Vertexs[0].Position.z = 0.0f;

		Vertexs[1].Position.x = B.x;
		Vertexs[1].Position.y = A.y;
		Vertexs[1].Position.z = 0.0f;

		Vertexs[2].Position.x = B.x;
		Vertexs[2].Position.y = B.y;
		Vertexs[2].Position.z = 0.0f;

		Vertexs[3].Position.x = A.x;
		Vertexs[3].Position.y = B.y;
		Vertexs[3].Position.z = 0.0f;
		

		Vertexs[0].Texture.u = 0;
		Vertexs[0].Texture.v = 0;

		Vertexs[1].Texture.u = 1;
		Vertexs[1].Texture.v = 0;

		Vertexs[2].Texture.u = 1;
		Vertexs[2].Texture.v = 1;

		Vertexs[3].Texture.u = 0;
		Vertexs[3].Texture.v = 1;

		int Indexs[6];
		Indexs[0]=0;
		Indexs[1]=1;
		Indexs[2]=2;
		Indexs[3]=2;
		Indexs[4]=3;
		Indexs[5]=0;

		pVBuffer = NULL;
		pIBuffer = NULL;
		CreateFillBuffer<Quad::QuadVertex>(pDev, &pVBuffer, 4, Vertexs, NULL);
		CreateFillIndexBuffer<int>(pDev, &pIBuffer, 6, Indexs, D3DFMT_INDEX32);
	}

	Quad::Quad(float Ax, float Ay, float Bx, float By, LPDIRECT3DDEVICE9 pDev)
	{
		//			A --------- *
		//			|			|
		//			|			|
		//			|			|
		//			* --------- B
		
			//uses the same effect that BasicEffect from GUIMaterials
		pEffect = GUIFramework::MaterialsLibrary.BasicEffect.pEffect;

		Vertexs[0].Position.x = Ax;
		Vertexs[0].Position.y = Ay;
		Vertexs[0].Position.z = 0.0f;

		Vertexs[1].Position.x = Bx;
		Vertexs[1].Position.y = Ay;
		Vertexs[1].Position.z = 0.0f;

		Vertexs[2].Position.x = Bx;
		Vertexs[2].Position.y = By;
		Vertexs[2].Position.z = 0.0f;

		Vertexs[3].Position.x = Ax;
		Vertexs[3].Position.y = By;
		Vertexs[3].Position.z = 0.0f;
		

		Vertexs[0].Texture.u = 0;
		Vertexs[0].Texture.v = 0;

		Vertexs[1].Texture.u = 1;
		Vertexs[1].Texture.v = 0;

		Vertexs[2].Texture.u = 1;
		Vertexs[2].Texture.v = 1;

		Vertexs[3].Texture.u = 0;
		Vertexs[3].Texture.v = 1;

		int Indexs[6];
		Indexs[0]=0;
		Indexs[1]=1;
		Indexs[2]=2;
		Indexs[3]=2;
		Indexs[4]=3;
		Indexs[5]=0;

		pVBuffer = NULL;
		pIBuffer = NULL;
		CreateFillBuffer<Quad::QuadVertex>(pDev, &pVBuffer, 4, Vertexs, NULL);
		CreateFillIndexBuffer<int>(pDev, &pIBuffer, 6, Indexs, D3DFMT_INDEX32);
	}

	Quad::Quad(const D3DXVECTOR3& A, const D3DXVECTOR3& B, const D3DXVECTOR3& C, 
		const D3DXVECTOR3& D, LPDIRECT3DDEVICE9 pDevice)
	{
		//			A --------- B
		//			|			|
		//			|			|
		//			|			|
		//			D --------- C
		
			//uses the same effect that BasicEffect from GUIMaterials
		pEffect = GUIFramework::MaterialsLibrary.BasicEffect.pEffect;
		
		Vertexs[0].Position.x = A.x;
		Vertexs[0].Position.y = A.y;
		Vertexs[0].Position.z = A.z;

		Vertexs[1].Position.x = B.x;
		Vertexs[1].Position.y = B.y;
		Vertexs[1].Position.z = B.z;

		Vertexs[2].Position.x = C.x;
		Vertexs[2].Position.y = C.y;
		Vertexs[2].Position.z = C.z;

		Vertexs[3].Position.x = D.x;
		Vertexs[3].Position.y = D.y;
		Vertexs[3].Position.z = D.z;

		Vertexs[0].Texture.u = 0;
		Vertexs[0].Texture.v = 0;

		Vertexs[1].Texture.u = 1;
		Vertexs[1].Texture.v = 0;

		Vertexs[2].Texture.u = 1;
		Vertexs[2].Texture.v = 1;

		Vertexs[3].Texture.u = 0;
		Vertexs[3].Texture.v = 1;

		int Indexs[6];
		Indexs[0]=0;
		Indexs[1]=1;
		Indexs[2]=2;
		Indexs[3]=2;
		Indexs[4]=3;
		Indexs[5]=0;

		pVBuffer = NULL;
		pIBuffer = NULL;
		CreateFillBuffer<Quad::QuadVertex>(pDevice, &pVBuffer, 4, Vertexs, NULL);
		CreateFillIndexBuffer<int>(pDevice, &pIBuffer, 6, Indexs, D3DFMT_INDEX32);
	}

	Quad::Quad(const D3DXVECTOR2& A, const D3DXVECTOR2& B, LPDIRECT3DDEVICE9 pDev, LPD3DXEFFECT pEff)
	{
		//			A --------- *
		//			|			|
		//			|			|
		//			|			|
		//			* --------- B
		
		pEffect = pEff;

		Vertexs[0].Position.x = A.x;
		Vertexs[0].Position.y = A.y;
		Vertexs[0].Position.z = 0.0f;

		Vertexs[1].Position.x = B.x;
		Vertexs[1].Position.y = A.y;
		Vertexs[1].Position.z = 0.0f;

		Vertexs[2].Position.x = B.x;
		Vertexs[2].Position.y = B.y;
		Vertexs[2].Position.z = 0.0f;

		Vertexs[3].Position.x = A.x;
		Vertexs[3].Position.y = B.y;
		Vertexs[3].Position.z = 0.0f;
		

		Vertexs[0].Texture.u = 0;
		Vertexs[0].Texture.v = 0;

		Vertexs[1].Texture.u = 1;
		Vertexs[1].Texture.v = 0;

		Vertexs[2].Texture.u = 1;
		Vertexs[2].Texture.v = 1;

		Vertexs[3].Texture.u = 0;
		Vertexs[3].Texture.v = 1;

		int Indexs[6];
		Indexs[0]=0;
		Indexs[1]=1;
		Indexs[2]=2;
		Indexs[3]=2;
		Indexs[4]=3;
		Indexs[5]=0;

		pVBuffer = NULL;
		pIBuffer = NULL;
		CreateFillBuffer<Quad::QuadVertex>(pDev, &pVBuffer, 4, Vertexs, NULL);
		CreateFillIndexBuffer<int>(pDev, &pIBuffer, 6, Indexs, D3DFMT_INDEX32);
	}

	Quad::Quad(float Ax, float Ay, float Bx, float By, LPDIRECT3DDEVICE9 pDev, LPD3DXEFFECT pEff)
		{
		//			A --------- *
		//			|			|
		//			|			|
		//			|			|
		//			* --------- B
		
		pEffect = pEff;

		Vertexs[0].Position.x = Ax;
		Vertexs[0].Position.y = Ay;
		Vertexs[0].Position.z = 0.0f;

		Vertexs[1].Position.x = Bx;
		Vertexs[1].Position.y = Ay;
		Vertexs[1].Position.z = 0.0f;

		Vertexs[2].Position.x = Bx;
		Vertexs[2].Position.y = By;
		Vertexs[2].Position.z = 0.0f;

		Vertexs[3].Position.x = Ax;
		Vertexs[3].Position.y = By;
		Vertexs[3].Position.z = 0.0f;
		

		Vertexs[0].Texture.u = 0;
		Vertexs[0].Texture.v = 0;

		Vertexs[1].Texture.u = 1;
		Vertexs[1].Texture.v = 0;

		Vertexs[2].Texture.u = 1;
		Vertexs[2].Texture.v = 1;

		Vertexs[3].Texture.u = 0;
		Vertexs[3].Texture.v = 1;

		int Indexs[6];
		Indexs[0]=0;
		Indexs[1]=1;
		Indexs[2]=2;
		Indexs[3]=2;
		Indexs[4]=3;
		Indexs[5]=0;

		pVBuffer = NULL;
		pIBuffer = NULL;
		CreateFillBuffer<Quad::QuadVertex>(pDev, &pVBuffer, 4, Vertexs, NULL);
		CreateFillIndexBuffer<int>(pDev, &pIBuffer, 6, Indexs, D3DFMT_INDEX32);
	}
	

	Quad::Quad(const D3DXVECTOR3& A, const D3DXVECTOR3& B, const D3DXVECTOR3& C,
				const D3DXVECTOR3& D, LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT pEff)
	{
		//			A --------- B
		//			|			|
		//			|			|
		//			|			|
		//			D --------- C
		
		pEffect = pEff;
		
		Vertexs[0].Position.x = A.x;
		Vertexs[0].Position.y = A.y;
		Vertexs[0].Position.z = A.z;

		Vertexs[1].Position.x = B.x;
		Vertexs[1].Position.y = B.y;
		Vertexs[1].Position.z = B.z;

		Vertexs[2].Position.x = C.x;
		Vertexs[2].Position.y = C.y;
		Vertexs[2].Position.z = C.z;

		Vertexs[3].Position.x = D.x;
		Vertexs[3].Position.y = D.y;
		Vertexs[3].Position.z = D.z;

		Vertexs[0].Texture.u = 0;
		Vertexs[0].Texture.v = 0;

		Vertexs[1].Texture.u = 1;
		Vertexs[1].Texture.v = 0;

		Vertexs[2].Texture.u = 1;
		Vertexs[2].Texture.v = 1;

		Vertexs[3].Texture.u = 0;
		Vertexs[3].Texture.v = 1;

		int Indexs[6];
		Indexs[0]=0;
		Indexs[1]=1;
		Indexs[2]=2;
		Indexs[3]=2;
		Indexs[4]=3;
		Indexs[5]=0;

		pVBuffer = NULL;
		pIBuffer = NULL;
		CreateFillBuffer<Quad::QuadVertex>(pDevice, &pVBuffer, 4, Vertexs, NULL);
		CreateFillIndexBuffer<int>(pDevice, &pIBuffer, 6, Indexs, D3DFMT_INDEX32);
	}

	Quad::~Quad()
	{
		if( pVBuffer )
			pVBuffer->Release();
		if( pIBuffer )
			pIBuffer->Release();
	}

	#pragma endregion 

	void Quad::ReCreateVertexBuffer(LPDIRECT3DDEVICE9 pDev)
	{
	CreateFillBuffer<Quad::QuadVertex>(pDev, &pVBuffer, 4, Vertexs, NULL);
	return;
	}	
	void Quad::Draw(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX& mPV, const D3DXMATRIX& mW, LPDIRECT3DTEXTURE9 pTexture)
	{
		D3DXHANDLE hTechnique = NULL;
		pEffect->FindNextValidTechnique(NULL, &hTechnique);
		
		#if defined( _DEBUG )
			if( hTechnique == NULL )
				::MyApp::GetSingleton()->Error("The effect don't have a valid technique in Quad class");
		#endif
			
		HRESULT hr = pEffect->SetTechnique("BasicEffect");
		//pEffect->SetTechnique(hTechnique);

		pEffect->SetMatrix( "mPV", &mPV);
		pEffect->SetMatrix( "mW", &mW);
		pEffect->SetTexture( "Image", pTexture);

		pDevice->SetStreamSource(0,pVBuffer,0,20);
		pDevice->SetIndices(pIBuffer);

		UINT PassCount = 0;
		pEffect->Begin(&PassCount,NULL);

			for(UINT i=0;i < PassCount; ++i)
			{
				pEffect->BeginPass(i);

				HRESULT hr = NULL;
				hr = pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);
				
				#if defined( _DEBUG )
					if( FAILED(hr) )
						::MyApp::GetSingleton()->Error("The DrawIndexedPrimitive Failed in Quad class");
				#endif

				pEffect->EndPass();
			}

		pEffect->End();
	}

	void Quad::Draw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 pTexture)
	{
		D3DXHANDLE hTechnique = NULL;
		pEffect->FindNextValidTechnique(NULL, &hTechnique);
		
		#if defined( _DEBUG )
			if( hTechnique == NULL )
				::MyApp::GetSingleton()->Error("The effect don't have a valid technique in Quad class");
		#endif
		pEffect->SetTechnique(hTechnique);

		D3DXMATRIX mI; D3DXMatrixIdentity(&mI);
		pEffect->SetMatrix( "mPV", &mI);
		pEffect->SetMatrix( "mW", &mI);
		pEffect->SetTexture( "Image", pTexture);

		pDevice->SetStreamSource(0,pVBuffer,0,20);
		pDevice->SetIndices(pIBuffer);

		UINT PassCount = 0;
		pEffect->Begin(&PassCount,NULL);

			for(UINT i=0;i < PassCount; ++i)
			{
				pEffect->BeginPass(i);

				HRESULT hr = NULL;
				hr = pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);
				
				#if defined( _DEBUG )
					if( FAILED(hr) )
						::MyApp::GetSingleton()->Error("The DrawIndexedPrimitive Failed in Quad class");
				#endif
					
				pEffect->EndPass();
			}

		pEffect->End();
	}

	void Quad::Draw(QuadVertex* V, LPDIRECT3DDEVICE9 pDev, LPD3DXEFFECT pEff, LPDIRECT3DTEXTURE9 pTexture)
	{
		D3DXHANDLE hTechnique = NULL;
		pEff->FindNextValidTechnique(NULL, &hTechnique);
		
		#if defined( _DEBUG )
			if( hTechnique == NULL )
				::MyApp::GetSingleton()->Error("The effect don't have a valid technique in Quad class");
		#endif
		pEff->SetTechnique(hTechnique);

		pEff->SetTexture( "Image", pTexture);

		UINT PassCount = 0;
		pEff->Begin(&PassCount,NULL);

			for(UINT i=0;i < PassCount; ++i)
			{
				pEff->BeginPass(i);

				HRESULT hr = NULL;
				hr = pDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST,2,V,sizeof(Quad::QuadVertex));
				
				#if defined( _DEBUG )
					if( FAILED(hr) )
						::MyApp::GetSingleton()->Error("The DrawPrimitiveUP Failed in Quad class");
				#endif

				pEff->EndPass();
			}

		pEff->End();
	}

}