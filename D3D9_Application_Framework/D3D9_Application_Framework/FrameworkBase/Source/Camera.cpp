#pragma once

#include "stdafx.h"
#include "..\\Header\\Camera.h"

CameraMG::~CameraMG(void)
	{
	if( pD3dDev )
		pD3dDev->Release();
	return;
	}

void CameraMG::SetDevice(LPDIRECT3DDEVICE9 pDev)
	{
	pDev->AddRef();//Incrementing the references
	pD3dDev = pDev;
	return;
	}

void CameraMG::SetPresentParameters(D3DPRESENT_PARAMETERS params)
	{
	presentParameters = params;
	return;
	}

bool CameraMG::SetProjection(float FoV, float zN, float zF)
	{
	D3DXMatrixPerspectiveFovLH(&mProj, FoV, presentParameters.BackBufferWidth/(float)presentParameters.BackBufferHeight,
		zN, zF);
	hr = pD3dDev->SetTransform( D3DTS_PROJECTION , &mProj );
	if( FAILED(hr) )
		return false;
	return true;
	}

void CameraMG::LookAt(D3DXVECTOR3 target)
	{
	Target = target;
	return;
	}

void CameraMG::Position(D3DXVECTOR3 position)
	{
	Eye = position;
	return;
	}

void CameraMG::Upper(D3DXVECTOR3 up)
	{
	Up = up;
	return;
	}

bool CameraMG::UpdateView()
	{
	//D3DXVec3Cross(&Up, &Target, &Eye);
	D3DXMatrixLookAtLH( &mView , &Eye , &Target,
		&Up );
	hr = pD3dDev->SetTransform( D3DTS_PROJECTION , &mProj );
	hr = pD3dDev->SetTransform( D3DTS_VIEW , &mView );
	if( FAILED(hr) )
		return false;
	return true;
	}

void CameraMG::GetTotalTransformation(D3DXMATRIX& ts)
{
	ts = mProj*mView;
	return;
}

void CameraMG::GetPosition(D3DXVECTOR3** vec)
{
	*vec = &Eye;
	return;
}

void CameraMG::GetTarget(D3DXVECTOR3** vec)
{
	*vec = &Target;
	return;
}