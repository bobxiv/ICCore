#pragma once

#include "CameraMtx.h"

#include "..\\Mesh\\SkinnedModel.h"

//Specilization of CameraMtx
//Camera centered on a point
class CameraCenterOverlook: public CameraMtx
{
protected:
	//Center where to look
	D3DXVECTOR3 m_Center;

	// Current/Max/Min possible distance form the center
	float m_CenterCurDistance;
	float m_CenterMaxDistance;
	float m_CenterMinDistance;

public:
	
	//Constructor
		//Parameter:
		//- model				centered over the model calculating the bounding sphere [IN]
		//- MaxRadiusScalar		{1,inf} max distance possible meter in min distances [OPCIONAL] [IN]
	CameraCenterOverlook(XFileModel::SkinnedModel model, float MaxRadiusScalar);

	const D3DXVECTOR3& GetCenter		  () const { return m_Center;		}

	#ifdef _DEBUG
	CameraCenterOverlook(XFileModel::SkinnedModel model, float MaxRadiusScalar, LPDIRECT3DDEVICE9 pDevice);
	#endif 

	virtual const D3DXMATRIX& GetViewMatrix	();

	virtual void Rotate	( float x, float y, float z );

	void SetCenter(D3DXVECTOR3& Center);

	virtual void Forward		( float elapsedTime );
	virtual void Backward		( float elapsedTime );

};