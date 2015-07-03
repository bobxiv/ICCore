#pragma once

#include "stdafx.h"
#include "..\\..\\Header\\GUI\\GUIMaterial.h"

#include "..\\..\\..\\MyApp.h"

namespace GUIFramework
{

	GUIMaterial::~GUIMaterial()
	{
		//if( pEffect )
			//pEffect->Release();
	}

	GUIBasicEffect::GUIBasicEffect()
	{
		pEffect = NULL;
	}

	GUIBasicEffect::~GUIBasicEffect()
	{
	}

	void GUIBasicEffect::Initialize()
	{
		LPD3DXBUFFER pErrors = NULL;
		HRESULT hr = D3DXCreateEffectFromFile(MyApp::GetSingleton()->GetD3DDevice(), L"D3D9_Application_Framework\\Content\\Effects\\GUI\\BasicEffect.fx",
			NULL, NULL, NULL, NULL, &pEffect, &pErrors);

		#if defined( _DEBUG )
			if ( FAILED( hr ) && pErrors != NULL && pErrors->GetBufferPointer() != NULL )
			{
				char* aux = new char[pErrors->GetBufferSize()+sizeof("Effect Compilation Error: ")+1];
				aux[0] = '\0';
				aux = strcat(aux, "Effect Compilation Error: ");
				MyApp::GetSingleton()->Error(strcat(aux, (char *)pErrors->GetBufferPointer()));
				delete[] aux;
			}
		#endif 
		if( pErrors != NULL )
			pErrors->Release();

		return;
	}

}