#include "stdafx.h"

#include "CameraCenterOverlook.h"

CameraCenterOverlook::CameraCenterOverlook(XFileModel::SkinnedModel model, float MaxRadiusScalar = 1.0f)
	{
		float Radius;
		model.GetMeshBoundingSphere(m_Center, Radius);

		m_CenterMinDistance = Radius;
		m_CenterMaxDistance = Radius*MaxRadiusScalar;
		m_CenterCurDistance = (m_CenterMinDistance+m_CenterMaxDistance)/(float)2;

		m_Position = D3DXVECTOR3(0.0f,0.0f,-1.0f) * m_CenterCurDistance;

		m_Target = D3DXVECTOR3(0.0f,0.0f,0.0f) - m_Position;
		D3DXVec3Normalize(&m_Target, &m_Target);
	}

#ifdef _DEBUG
CameraCenterOverlook::CameraCenterOverlook(XFileModel::SkinnedModel model, float MaxRadiusScalar, LPDIRECT3DDEVICE9 pDevice):
	CameraMtx(pDevice)
	{
		float Radius;
		model.GetMeshBoundingSphere(m_Center, Radius);

		m_CenterMinDistance = Radius;
		m_CenterMaxDistance = Radius*MaxRadiusScalar;
		m_CenterCurDistance = (m_CenterMinDistance+m_CenterMaxDistance)/(float)2;

		m_Position = D3DXVECTOR3(0.0f,0.0f,-1.0f) * m_CenterCurDistance;

		m_Target = D3DXVECTOR3(0.0f,0.0f,0.0f) - m_Position;
		D3DXVec3Normalize(&m_Target, &m_Target);
	}
#endif

const D3DXMATRIX& CameraCenterOverlook::GetViewMatrix	()
{
	if( m_ViewDirty )//If Spatial Definition change then re-create View TS
	{

		if( m_ViewUpdateCount == 50 )
		{
			//With many Rotations, the sum of the floating point errors can make the this vectors
			//stop being ortogonal and unitary, so every 50 Updates we fix them
			D3DXVec3Normalize(&m_Target, &m_Target);
			D3DXVec3Cross(&m_Right, &m_Up, &m_Target);
			D3DXVec3Normalize(&m_Right, &m_Right);
			D3DXVec3Cross(&m_Up, &m_Target, &m_Right);
			D3DXVec3Normalize(&m_Up, &m_Up);

			m_ViewUpdateCount=0;
		}

		//Make the View Transformation

		//Could be:
		//D3DXMatrixLookAtLH(&m_View, &m_Position, &m_Target, &m_Up);

		//				 Rx		 Ux		 Tx		0
		//				 Ry		 Uy		 Ty		0
		//				 Rz		 Uz		 Tz		0
		//				<P,R>	<P,U>	<P,T>	1

		//Inverse camera rotation
		m_View._11 =  m_Right.x; m_View._12 = m_Up.x; m_View._13 = m_Target.x;
		m_View._21 =  m_Right.y; m_View._22 = m_Up.y; m_View._23 = m_Target.y;
		m_View._31 =  m_Right.z; m_View._32 = m_Up.z; m_View._33 = m_Target.z;

		//Translation after rotation
		//Also converting to World
		D3DXVECTOR3 PositionTrans = m_Position+m_Center;
		m_View._41 = -D3DXVec3Dot( &PositionTrans, &m_Right );//Right  = x
		m_View._42 = -D3DXVec3Dot( &PositionTrans, &m_Up	 );//Up	    = y
		m_View._43 = -D3DXVec3Dot( &PositionTrans, &m_Target);//Target = z

		//Updated
		m_ViewUpdateCount ++;
		//not dirty any more
		m_ViewDirty = false;
	}

	//Return the cached View TS
	return m_View;
}

void CameraCenterOverlook::Rotate	( float x, float y, float z )   
{
	D3DXMATRIX Rotation;

	if( x )//if any Pitch
	{
		//Build Rotation MTX
		//Around Roght, so we can never turn around the camera(annoying)		
		D3DXMatrixRotationAxis(&Rotation, &m_Right, D3DXToRadian(x));

		//we never turn the y axis upward beacause it will turn the camera(annoying)
		D3DXVECTOR3 m_UpAux = m_Up;
		D3DXVec3TransformNormal(&m_UpAux	 , &m_UpAux	, &Rotation);
		if( m_UpAux.y > 0 )
		{
			//Transform Axis
			D3DXVec3TransformNormal(&m_Up	 , &m_Up	, &Rotation);
			D3DXVec3TransformNormal(&m_Right , &m_Right	, &Rotation);
			D3DXVec3TransformNormal(&m_Target, &m_Target, &Rotation);

			D3DXVec3TransformNormal(&m_Position, &m_Position, &Rotation);
		}
	}

	if( y )//if any Yaw
	{
		//Build Rotation MTX
		D3DXMatrixRotationY(&Rotation, D3DXToRadian(y));
		
		//Transform Axis
		D3DXVec3TransformNormal(&m_Up	 , &m_Up	, &Rotation);
		D3DXVec3TransformNormal(&m_Right , &m_Right	, &Rotation);
		D3DXVec3TransformNormal(&m_Target, &m_Target, &Rotation);

		D3DXVec3TransformNormal(&m_Position, &m_Position, &Rotation);
	}

	/*(annoying)
	if( z )//if any Roll
	{
		//Build Rotation MTX
		D3DXMatrixRotationAxis(&Rotation, &m_Target, D3DXToRadian(z));
		
		//we never turn the y axis upward beacause it will turn the camera(annoying)
		D3DXVECTOR3 m_UpAux = m_Up;
		D3DXVec3TransformNormal(&m_UpAux	 , &m_UpAux	, &Rotation);
		if( m_UpAux.y > 0 )
		{
			//Transform Axis
			D3DXVec3TransformNormal(&m_Up	 , &m_Up	, &Rotation);
			D3DXVec3TransformNormal(&m_Right , &m_Right	, &Rotation);
			D3DXVec3TransformNormal(&m_Target, &m_Target, &Rotation);

			D3DXVec3TransformNormal(&m_Position, &m_Position, &Rotation);
		}
	}
	*/

	m_ViewDirty = true;

	return;
}

void CameraCenterOverlook::SetCenter(D3DXVECTOR3& Center)
{
	m_Center = Center;

	m_ViewDirty = true;
}

void CameraCenterOverlook::Forward		( float elapsedTime )
{
	if( m_CenterMaxDistance < (m_CenterCurDistance+elapsedTime) )
		elapsedTime = m_CenterMaxDistance - m_CenterCurDistance;

	m_CenterCurDistance += elapsedTime;

	m_Position += m_Target*elapsedTime;
	m_ViewDirty = true;

	return;
}

void CameraCenterOverlook::Backward		( float elapsedTime )
{
	if( m_CenterMinDistance > (m_CenterCurDistance-elapsedTime) )
		elapsedTime = m_CenterCurDistance - m_CenterMinDistance;

	m_CenterCurDistance -= elapsedTime;

	m_Position -= m_Target*elapsedTime;
	m_ViewDirty = true;

	return;
}
