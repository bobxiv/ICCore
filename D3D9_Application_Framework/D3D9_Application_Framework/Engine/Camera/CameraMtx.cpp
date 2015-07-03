#include "stdafx.h"

#include "CameraMtx.h"

CameraMtx::CameraMtx(): m_ViewUpdateCount(0)
{
	// Reset / Clear all required values
	//m_pPlayer         = NULL;
	m_Right        = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	m_Up           = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_Target       = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	m_Position     = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	m_FOV             = 60.0f;
	m_NearClipPlane   = 1.0f;
	m_FarClipPlane    = 1000.0f;
	m_ViewPort.X      = 0;
	m_ViewPort.Y      = 0;
	m_ViewPort.Width  = 640;
	m_ViewPort.Height = 480;
	m_ViewPort.MinZ   = 0.0f;
	m_ViewPort.MaxZ   = 1.0f;

	// Set matrices to identity
	D3DXMatrixIdentity( &m_View );
	D3DXMatrixIdentity( &m_Projection );
}

#ifdef _DEBUG
	CameraMtx::CameraMtx(LPDIRECT3DDEVICE9 pDevice): m_ViewUpdateCount(0), m_UpArrow("arrow.x",pDevice),
																	   m_RightArrow("arrow.x",pDevice),
																	   m_TargetArrow("arrow.x",pDevice)
																	   //m_Camera("Camera.obj",pDevice, true)
		{
			// Reset / Clear all required values
			//m_pPlayer         = NULL;
			m_Right        = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
			m_Up           = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
			m_Target       = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
			m_Position     = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

			m_FOV             = 60.0f;
			m_NearClipPlane   = 1.0f;
			m_FarClipPlane    = 1000.0f;
			m_ViewPort.X      = 0;
			m_ViewPort.Y      = 0;
			m_ViewPort.Width  = 640;
			m_ViewPort.Height = 480;
			m_ViewPort.MinZ   = 0.0f;
			m_ViewPort.MaxZ   = 1.0f;

			// Set matrices to identity
			D3DXMatrixIdentity( &m_View );
			D3DXMatrixIdentity( &m_Projection );




			//Debug render frustrum matrix inverses
			D3DXMatrixIdentity( &m_InvView );
			D3DXMatrixIdentity( &m_InvProjection );

			//Debug render frustrum Vertex Declaration
			pDevice->CreateVertexDeclaration(Vertex::VertexElement, &pVDeclFrustrum);

			m_Camera.Load("Camera.obj",pDevice);
			D3DVERTEXELEMENT9 pElem[15];
			HRESULT hr = m_Camera.m_RootMeshContainer->MeshData.pMesh->GetDeclaration(pElem);
			assert( !FAILED(hr) );
			hr = pDevice->CreateVertexDeclaration(pElem, &pVDeclCamera);
			assert( !FAILED(hr) );

			D3DXVECTOR4 FrustrumV[8];
			WORD FrustrumI[12*3];
			float D = 0.000001f;
			FrustrumV[0] = D3DXVECTOR4(-1.0f+D,1.0f-D,0.0f+D,1.0f);
			FrustrumV[1] = D3DXVECTOR4(1.0f-D,1.0f-D,0.0f+D,1.0f);
			FrustrumV[2] = D3DXVECTOR4(1.0f-D,-1.0f+D,0.0f+D,1.0f);
			FrustrumV[3] = D3DXVECTOR4(-1.0f+D,-1.0f+D,0.0f+D,1.0f);

			FrustrumV[4] = D3DXVECTOR4(-1.0f+D,1.0f-D,1.0f-D,1.0f);
			FrustrumV[5] = D3DXVECTOR4(1.0f-D,1.0f-D,1.0f-D,1.0f);
			FrustrumV[6] = D3DXVECTOR4(1.0f-D,-1.0f+D,1.0f-D,1.0f);
			FrustrumV[7] = D3DXVECTOR4(-1.0f+D,-1.0f+D,1.0f-D,1.0f);

			hr = pDevice->CreateVertexBuffer(sizeof(D3DXVECTOR4)*8,D3DUSAGE_WRITEONLY, D3DFVF_XYZW, D3DPOOL_DEFAULT, &pFrustrumVB, NULL);
			assert( !FAILED(hr) );
			
			void* data = NULL;
			pFrustrumVB->Lock(0, sizeof(D3DXVECTOR4)*8, &data, NULL);

				memcpy(data, FrustrumV, sizeof(D3DXVECTOR4)*8);

			pFrustrumVB->Unlock();

			//front
			FrustrumI[0]  = 0;
			FrustrumI[1]  = 1;
			FrustrumI[2]  = 2;
			FrustrumI[3]  = 2;
			FrustrumI[4]  = 3;
			FrustrumI[5]  = 0;

			//Back (note that the normal for cull is the reverse from front)
			FrustrumI[6]  = 4;
			FrustrumI[7]  = 7;
			FrustrumI[8]  = 6;
			FrustrumI[9]  = 6;
			FrustrumI[10] = 5;
			FrustrumI[11] = 4;

			//Right
			FrustrumI[12] = 1;
			FrustrumI[13] = 5;
			FrustrumI[14] = 6;
			FrustrumI[15] = 6;
			FrustrumI[16] = 2;
			FrustrumI[17] = 1;

			//Left (note that the normal for cull is the reverse from right)
			FrustrumI[18] = 0;
			FrustrumI[19] = 3;
			FrustrumI[20] = 7;
			FrustrumI[21] = 7;
			FrustrumI[22] = 4;
			FrustrumI[23] = 0;

			//Top
			FrustrumI[24] = 0;
			FrustrumI[25] = 4;
			FrustrumI[26] = 5;
			FrustrumI[27] = 5;
			FrustrumI[28] = 1;
			FrustrumI[29] = 0;

			//Bottom (note that the normal for cull is the reverse from top)
			FrustrumI[30] = 3;
			FrustrumI[31] = 2;
			FrustrumI[32] = 6;
			FrustrumI[33] = 6;
			FrustrumI[34] = 7;
			FrustrumI[35] = 3;

			hr = pDevice->CreateIndexBuffer(sizeof(WORD)*12*3,D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pFrustrumIB, NULL);
			assert( !FAILED(hr) );

			data = NULL;
			pFrustrumIB->Lock(0, sizeof(WORD)*(12*3), &data, NULL);

				memcpy(data, FrustrumI, sizeof(WORD)*(12*3));

			pFrustrumIB->Unlock();
		}

	void CameraMtx::DebugRenderCamera(LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT pEffect)
		{
			//Camera state Rotation Matrix
			D3DXMATRIX Rot;
			D3DXMatrixIdentity(&Rot);
			Rot(0,0) = this->m_Right.x;  Rot(0,1) = this->m_Right.y;  Rot(0,2) = this->m_Right.z;
			Rot(1,0) = this->m_Up.x;     Rot(1,1) = this->m_Up.y;     Rot(1,2) = this->m_Up.z;
			Rot(2,0) = this->m_Target.x; Rot(2,1) = this->m_Target.y; Rot(2,2) = this->m_Target.z;
		
			D3DXMATRIX Scale;//Arrows scale factor
			D3DXMatrixScaling(&Scale, 55.0f, 55.0f, 55.0f);

			
			D3DXMATRIX YArrowRot;//Y Axis Arrow fix to point in that direction
			{
			D3DXMatrixRotationX(&YArrowRot, -D3DX_PI/2);
			D3DXMATRIX Center;
			D3DXMatrixTranslation(&Center,0.0f,-0.61884129f,0.0f);
			D3DXMATRIX PointFix;
			D3DXMatrixRotationX(&PointFix, D3DX_PI);
			D3DXMATRIX UnCenter;
			D3DXMatrixTranslation(&UnCenter,0.0f,0.61884129f,0.0f);

			YArrowRot = YArrowRot*Center*PointFix*UnCenter;
			}

			
			D3DXMATRIX XArrowRot;//X Axis Arrow fix to point in that direction
			{
			D3DXMatrixRotationY(&XArrowRot, D3DX_PI/2);

			D3DXMATRIX Center;
			D3DXMatrixTranslation(&Center,-0.61884129f,0.0f,0.0f);
			D3DXMATRIX PointFix;
			D3DXMatrixRotationY(&PointFix, D3DX_PI);
			D3DXMATRIX UnCenter;
			D3DXMatrixTranslation(&UnCenter,0.61884129f,0.0f,0.0f);

			XArrowRot = XArrowRot*Center*PointFix*UnCenter;
			}

			D3DXMATRIX ZArrowRot;//Z Axis Arrow fix to point in that direction
			{
			D3DXMATRIX Center;
			D3DXMatrixTranslation(&Center,0.0f,0.0f,-0.61884129f);
			D3DXMATRIX PointFix;
			D3DXMatrixRotationX(&PointFix, D3DX_PI);
			D3DXMATRIX UnCenter;
			D3DXMatrixTranslation(&UnCenter,0.0f,0.0f,0.61884129f);

			ZArrowRot = Center*PointFix*UnCenter;
			}

			//Moves the arrows & camera model to the camera position
			D3DXMATRIX CenterCameraTranslation;
			D3DXMatrixTranslation(&CenterCameraTranslation, this->m_Position.x, this->m_Position.y, this->m_Position.z);

			//Arrows Rendering
			m_UpArrow.Render	(pDevice, &(YArrowRot*Rot*Scale*CenterCameraTranslation) );
			m_RightArrow.Render	(pDevice, &(XArrowRot*Rot*Scale*CenterCameraTranslation) );
			m_TargetArrow.Render(pDevice, &(ZArrowRot*Rot*Scale*CenterCameraTranslation) );


			D3DXMatrixScaling(&Scale, 0.85f, 0.85f, 0.85f);//Camera model scale factor

			D3DXMATRIX Translation;//Translation to put camera model and arrow axis a little apart
			D3DXVECTOR3 trans = m_Right*(-60.0f)+m_Up*(-20.0f)+m_Target*(-25.0f);
			D3DXMatrixTranslation(&Translation, trans.x, trans.y, trans.z);
			D3DXMATRIX CameraFixRot;//Rotation of the camera model to make it point z axis(target)
			D3DXMatrixRotationY(&CameraFixRot, D3DX_PI/2);

			//m_Camera.Render(pDevice, NULL/*&(Scale)*/);
			pEffect->SetMatrix("World",					&(CameraFixRot*Rot*Scale*CenterCameraTranslation*Translation) );
			pEffect->SetMatrix("WorldViewProjection",	&(CameraFixRot*Rot*Scale*CenterCameraTranslation*Translation*View*Proyection) );
			pDevice->SetVertexDeclaration(pVDeclCamera);
			
			//Camera model Rendering
			pDevice->BeginScene();
				pEffect->Begin(0,NULL);
					pEffect->BeginPass(0);
						m_Camera.m_RootMeshContainer->MeshData.pMesh->DrawSubset(0);
					pEffect->EndPass();
				pEffect->End();
			pDevice->EndScene();

			return;
		}

	void CameraMtx::DebugRenderFrustrum(LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT pEffect)
		{
			//Inverses => from this camera device space to world space
			if( m_ProjectionDirty )
				D3DXMatrixInverse(&m_InvProjection, NULL, &this->GetProjectionMatrix());
			if( this->m_ViewDirty )
				D3DXMatrixInverse(&m_InvView, NULL, &this->GetViewMatrix());

			pEffect->SetMatrix("WorldViewProjection", &((m_InvProjection*m_InvView)*View*Proyection) );

			pEffect->SetBool("UseNormal", false);
			pEffect->SetVector("Color", &D3DXVECTOR4(0.5f,0.5f,0.5f,1.0f));
			pDevice->SetVertexDeclaration(pVDeclFrustrum);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			pDevice->SetStreamSource(0,pFrustrumVB, 0, sizeof(Vertex));
			pDevice->SetIndices(pFrustrumIB);

			//Render Occluded Frustrum sides(grey)
			pDevice->BeginScene();
				pEffect->Begin(0,NULL);
					pEffect->BeginPass(0);
						pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);			
					pEffect->EndPass();
				pEffect->End();
			pDevice->EndScene();

			pEffect->SetVector("Color", &D3DXVECTOR4(0.0f,0.0f,0.0f,1.0f));

			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			//Render Visible Frustrum sides(black)
			pDevice->BeginScene();
				pEffect->Begin(0,NULL);
					pEffect->BeginPass(0);
						pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);			
					pEffect->EndPass();
				pEffect->End();
			pDevice->EndScene();

			pEffect->SetBool("UseNormal", true);
			pEffect->SetVector("Color", &D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f));
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			
			return;
		}
#endif


void CameraMtx::SetFOV(float FOV)
{
	m_FOV = FOV;

	m_ProjectionDirty = true;
	return;
}

void CameraMtx::SetViewPort(long Left, long Top, long Width, long Height, float NearClip, float FarClip, LPDIRECT3DDEVICE9 pDevice = NULL)
{
	m_ViewPort.X = Left;
	m_ViewPort.Y = Top;
	m_ViewPort.Width  = Width;
	m_ViewPort.Height = Height;
	m_ViewPort.MinZ = 0.0f;
	m_ViewPort.MaxZ = 1.0f;
	m_NearClipPlane = NearClip;
	m_FarClipPlane  = FarClip;

	m_ProjectionDirty = true;
	
	if( pDevice ) pDevice->SetViewport( &m_ViewPort );
	return;
}

const D3DXMATRIX& CameraMtx::GetProjectionMatrix()
{
	if( m_ProjectionDirty )//If Perspective Parameters change then re-create Projective TS
	{
		float AspectRatio = (float)m_ViewPort.Width / (float)m_ViewPort.Height;

		D3DXMatrixPerspectiveFovLH(&m_Projection, D3DXToRadian(m_FOV), AspectRatio, m_NearClipPlane, m_FarClipPlane);

		//not dirty any more
		m_ProjectionDirty = false;
	
	}

	//Return the cached Projetion TS
	return m_Projection;
}

const D3DXMATRIX& CameraMtx::GetViewMatrix()
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
		m_View._41 = -D3DXVec3Dot( &m_Position, &m_Right );//Right  = x
		m_View._42 = -D3DXVec3Dot( &m_Position, &m_Up    );//Up	    = y
		m_View._43 = -D3DXVec3Dot( &m_Position, &m_Target);//Target = z

		//Updated
		m_ViewUpdateCount ++;
		//not dirty any more
		m_ViewDirty = false;
	}

	//Return the cached View TS
	return m_View;
}

void CameraMtx::SetCopyDetails ( const CameraMtx* pCamera )
{
	// Validate Parameters
	if (!pCamera) return;

	// Reset / Clear all required values
	m_Position		= pCamera->GetPosition();
	m_Right			= pCamera->GetRight();
	m_Target		= pCamera->GetTarget();
	m_Up			= pCamera->GetUp();
	m_FOV			= pCamera->GetFOV();
	m_NearClipPlane = pCamera->GetNearClipPlane();
	m_FarClipPlane  = pCamera->GetFarClipPlane();
	m_ViewPort		= pCamera->GetViewPort();

	// Rebuild both matrices
	m_ViewDirty		  = true;
	m_ProjectionDirty = true;
	m_ViewUpdateCount = 0;

	return;
}

//-------------------------------------------------------------------------
// Camera Translation/Rotation Functionality
//-------------------------------------------------------------------------

void CameraMtx::Pitch(float angle)//arround the x axis
{
	D3DXMATRIX Rotation;

	//Build Rotation MTX
	D3DXMatrixRotationAxis(&Rotation, &m_Right, D3DXToRadian(angle));
	
	//Transform Axis
	D3DXVECTOR4 aux;
	D3DXVec3Transform(&aux, &m_Up, &Rotation);
	m_Up = D3DXVECTOR3(aux);
	D3DXVec3Transform(&aux, &m_Right, &Rotation);
	m_Right = D3DXVECTOR3(aux);
	D3DXVec3Transform(&aux, &m_Target, &Rotation);
	m_Target = D3DXVECTOR3(aux);

	m_ViewDirty = true;

	return;
}

void CameraMtx::Yaw(float angle)//arround the y axis
{
	D3DXMATRIX Rotation;

	//Build Rotation MTX
	D3DXMatrixRotationAxis(&Rotation, &m_Up, D3DXToRadian(angle));
	
	//Transform Axis
	D3DXVECTOR4 aux;
	D3DXVec3Transform(&aux, &m_Up, &Rotation);
	m_Up = D3DXVECTOR3(aux);
	D3DXVec3Transform(&aux, &m_Right, &Rotation);
	m_Right = D3DXVECTOR3(aux);
	D3DXVec3Transform(&aux, &m_Target, &Rotation);
	m_Target = D3DXVECTOR3(aux);

	m_ViewDirty = true;

	return;
}

void CameraMtx::Roll(float angle)//arround the z axis
{
	D3DXMATRIX Rotation;

	//Build Rotation MTX
	D3DXMatrixRotationAxis(&Rotation, &m_Target, D3DXToRadian(angle));
	
	//Transform Axis
	D3DXVECTOR4 aux;
	D3DXVec3Transform(&aux, &m_Up, &Rotation);
	m_Up = D3DXVECTOR3(aux);
	D3DXVec3Transform(&aux, &m_Right, &Rotation);
	m_Right = D3DXVECTOR3(aux);
	D3DXVec3Transform(&aux, &m_Target, &Rotation);
	m_Target = D3DXVECTOR3(aux);

	m_ViewDirty = true;

	return;
}

void CameraMtx::Forward( float elapsedTime )
{
	m_Position += m_Target*elapsedTime;
	m_ViewDirty = true;

	return;
}

void CameraMtx::Backward( float elapsedTime )
{
	m_Position -= m_Target*elapsedTime;
	m_ViewDirty = true;

	return;
}

void CameraMtx::SetPosition( const D3DXVECTOR3& Position ) 
{ 
	m_Position = Position;
	m_ViewDirty = true;

	return;
}

void CameraMtx::Move( const D3DXVECTOR3& Shift )
{ 
	m_Position += Shift;
	m_ViewDirty = true;

	return;
}


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////	  Centered Overlook		 ////////////////////////////////////////
		////////////////////////////////////////			Camera			 ////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
//void CameraCenterOverlook::Pitch(float angle)//arround the x axis
//{
//	D3DXMATRIX Rotation;
//
//	//Build Rotation MTX
//	D3DXMatrixRotationAxis(&Rotation, &m_Right, D3DXToRadian(angle));
//	
//	//Transform Axis
//	D3DXVec3TransformNormal(&m_Up, &m_Up, &Rotation);
//	D3DXVec3TransformNormal(&m_Right, &m_Right, &Rotation);
//	D3DXVec3TransformNormal(&m_Target, &m_Target, &Rotation);
//
//	m_ViewDirty = true;
//
//	return;
//}
//
//void CameraCenterOverlook::Yaw(float angle)//arround the y axis
//{
//	D3DXMATRIX Rotation;
//
//	//Build Rotation MTX
//	D3DXMatrixRotationAxis(&Rotation, &m_Up, D3DXToRadian(angle));
//	
//	//Transform Axis
//	D3DXVec3TransformNormal(&m_Up, &m_Up, &Rotation);
//	D3DXVec3TransformNormal(&m_Right, &m_Right, &Rotation);
//	D3DXVec3TransformNormal(&m_Target, &m_Target, &Rotation);
//
//	m_ViewDirty = true;
//
//	return;
//}
//
//void CameraCenterOverlook::Roll(float angle)//arround the z axis
//{
//	D3DXMATRIX Rotation;
//
//	//Build Rotation MTX
//	D3DXMatrixRotationAxis(&Rotation, &m_Target, D3DXToRadian(angle));
//	
//	//Transform Axis
//	D3DXVec3TransformNormal(&m_Up, &m_Up, &Rotation);
//	D3DXVec3TransformNormal(&m_Right, &m_Right, &Rotation);
//	D3DXVec3TransformNormal(&m_Target, &m_Target, &Rotation);
//
//	m_ViewDirty = true;
//
//	return;
//}
//
//void CameraCenterOverlook::Forward( float elapsedTime )
//{
//	m_Position += m_Target*elapsedTime;
//	m_ViewDirty = true;
//
//	return;
//}
//
//void CameraCenterOverlook::Backward( float elapsedTime )
//{
//	m_Position -= m_Target*elapsedTime;
//	m_ViewDirty = true;
//
//	return;
//}