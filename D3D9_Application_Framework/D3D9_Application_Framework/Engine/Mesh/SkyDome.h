#pragma once
#include "stdafx.h"
#include "HierarchicalModel.h"
#include <string>

#include "CameraMtx.h"

//A skydome, to work needs a file "skydome.x" in the current directory
//or in specified directory, to load the dome
class SkyDome
{
	//Model mesh
	XFileModel::HierarchicalModel m_DomeModel;

	//Dx Device
	LPDIRECT3DDEVICE9 pDevice;

	//View & Projection TS
	D3DXMATRIX m_ViewTransformation;
	D3DXMATRIX m_ProjectionTransformation;

	//SkyDome world TS
	D3DXMATRIX m_FixTranslation;
	D3DXMATRIX m_CameraTranslation;//if pCamera != NULL then uses the pCamera position else need to be set with a function
	D3DXMATRIX m_ScaleTranslation;

	//The Camera in use
	CameraMtx* pCamera;
	
public:
	SkyDome(LPDIRECT3DDEVICE9 pD3dDev, CameraMtx* pCam = NULL): pDevice(pD3dDev), m_DomeModel( "skydome.x",pD3dDev,true), pCamera(pCam)
	{
		//So the dome is a little under the camera
		D3DXMatrixTranslation(&m_FixTranslation, 0.0f, -0.3f, 0.0f);

		//the position of the camera is needed every time it moves
		D3DXMatrixIdentity(&m_CameraTranslation);

		//originaly the skydome is [-1,1]
		D3DXMatrixScaling(&m_ScaleTranslation, 100.0f, 100.0f, 100.0f);
	}

	SkyDome(LPDIRECT3DDEVICE9 pD3dDev, std::string directory, CameraMtx* pCam = NULL): pDevice(pD3dDev), 
		m_DomeModel( directory + "\\" + "skydome.x",pD3dDev,true), pCamera(pCam)
	{
		//So the dome is a little under the camera
		D3DXMatrixTranslation(&m_FixTranslation, 0.0f, -0.3f, 0.0f);

		//the position of the camera is needed every time it moves
		D3DXMatrixIdentity(&m_CameraTranslation);

		//originaly the skydome is [-1,1]
		D3DXMatrixScaling(&m_ScaleTranslation, 100.0f, 100.0f, 100.0f);
	}

	void SetWind()
	{
	}

	void SetDayTime()
	{
	}

	CameraMtx* AttachCamera(CameraMtx* pCam)
	{
		CameraMtx* pAuxCamera = pCamera;
		pCamera = pCam;

		return pAuxCamera;
	}

	void DetachCamera()
	{
		pCamera = NULL;

		return;
	}

	//Sets the Position of the camera, used if no Camera Object is attached
	void SetCameraPosition(D3DXVECTOR3 Position)
	{
		D3DXMatrixTranslation(&m_CameraTranslation, Position[0], Position[1], Position[2]);
	}

	//Sets the View and Projection, they are used if no Camera Object is attached
	void SetViewProjection(D3DXMATRIX& View, D3DXMATRIX& Projection)
	{	
		m_ViewTransformation       = View;
		m_ProjectionTransformation = Projection;
	}

	void Render()
	{
		if( pCamera )
		{
			const D3DXVECTOR3& Position = pCamera->GetPosition();
			D3DXMatrixTranslation(&m_CameraTranslation, Position[0], Position[1], Position[2]);
			pDevice->SetTransform(D3DTS_WORLD,		&(m_FixTranslation*m_ScaleTranslation*m_CameraTranslation));
		}
		else
			pDevice->SetTransform(D3DTS_WORLD,		&(m_FixTranslation*m_ScaleTranslation*m_CameraTranslation));

		if( pCamera )
			pDevice->SetTransform(D3DTS_VIEW,		&pCamera->GetViewMatrix());
		else
			pDevice->SetTransform(D3DTS_VIEW,		&m_ViewTransformation);

		if( pCamera )
			pDevice->SetTransform(D3DTS_PROJECTION, &pCamera->GetProjectionMatrix());
		else
			pDevice->SetTransform(D3DTS_PROJECTION, &m_ProjectionTransformation);

		m_DomeModel.Render(pDevice, &(m_FixTranslation*m_ScaleTranslation*m_CameraTranslation));

		return;
	}

};
