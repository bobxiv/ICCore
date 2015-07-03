#pragma once

#include <d3dx9.h>

extern D3DXMATRIXA16 View;
extern D3DXMATRIXA16 Proyection;

#include "..\\Mesh\\HierarchicalModel.h"
#include "..\\Mesh\\SkinnedModel.h"

#include "..\\Mesh\\ObjParser.h"

//Base for implementations of a camera
//Uses Matrix and vectors to takes the position of the axis
class CameraMtx
{
protected:
	//-------------------------------------------------------------------------
    // Private & Protected Variables
    //-------------------------------------------------------------------------
	#pragma region Private & Protected Variables

	D3DXMATRIX m_View;		//Cached View		TS
	D3DXMATRIX m_Projection;//Cached Projection TS

	bool m_ViewDirty;		//If View		TS changed
	int  m_ViewUpdateCount;	//Count the amount of updates from last target/Up/Right Re-Ortogonalization
	bool m_ProjectionDirty; //If Projection TS changed

	//------------------------------------------------
	// Perspective parameters
	//------------------------------------------------
	float		 m_FOV;				//Fild of View in degrees;
	float		 m_NearClipPlane;	//Near Clip Plane Distance
	float		 m_FarClipPlane;	//Far  Clip Plane Distance
	D3DVIEWPORT9 m_ViewPort;
	
	//------------------------------------------------
	// Camera Spatial Definition
	//------------------------------------------------
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Target;
	D3DXVECTOR3 m_Up;
	D3DXVECTOR3 m_Right;
	
	#ifdef _DEBUG
	XFileModel::HierarchicalModel m_UpArrow;	//Model Up Axis
	XFileModel::HierarchicalModel m_RightArrow;	//Model Right Axis
	XFileModel::HierarchicalModel m_TargetArrow;//Model Target Axis

	ObjParser m_Camera;							//Model Camera 
	LPDIRECT3DVERTEXDECLARATION9 pVDeclCamera;	//Model Camera VD

	//Frustrum Mesh data
	LPDIRECT3DVERTEXBUFFER9 pFrustrumVB;		
	LPDIRECT3DINDEXBUFFER9  pFrustrumIB;
	LPDIRECT3DVERTEXDECLARATION9 pVDeclFrustrum;//Frustrum VD

	//Transformation to render the frustrum
	D3DXMATRIX m_InvView;
	D3DXMATRIX m_InvProjection;
	#endif

	#pragma endregion

public:

	//-------------------------------------------------------------------------
    // Enumerators
    //-------------------------------------------------------------------------
	enum CameraMode{ Base				 = 1 ,
					 Mode_CenterOverlook = 2 ,
					 Mode_FPS_Fly		 = 3 ,
					 Mode_ThirdPerson	 = 4 
					};

	//-------------------------------------------------------------------------
    // Constructors & Destructors 
    //-------------------------------------------------------------------------
	CameraMtx();
	#ifdef _DEBUG
	CameraMtx(LPDIRECT3DDEVICE9 pDevice);
	#endif
	~CameraMtx()
	{}

	//-------------------------------------------------------------------------
    // Public Functions 
    //-------------------------------------------------------------------------

	void SetFOV		 (float FOV);
	void SetViewPort (long Left, long Top, long Width, long Height, float NearClip, float FarClip, LPDIRECT3DDEVICE9 pDevice);

	const D3DXMATRIX& GetProjectionMatrix   ();
	virtual const D3DXMATRIX& GetViewMatrix	();

	const D3DXVECTOR3& GetPosition		  () const { return m_Position;		}
	const D3DXVECTOR3& GetRight			  () const { return m_Right;			}
	const D3DXVECTOR3& GetTarget		  () const { return m_Target;		}
	const D3DXVECTOR3& GetUp			  () const { return m_Up;			}
	const float& GetFOV					  () const { return m_FOV;			}
	const float& GetNearClipPlane		  () const { return m_NearClipPlane;	}
	const float& GetFarClipPlane		  () const { return m_FarClipPlane;	}
	const D3DVIEWPORT9& GetViewPort		  () const { return m_ViewPort;		}

	//-------------------------------------------------------------------------
    // Public Virtual Functions 
    //-------------------------------------------------------------------------
	//virtual void AttachToPlayer   ( /*CPlayer * pPlayer*/ ) = 0;
	//virtual void DetachFromPlayer ( )						= 0;

	virtual void SetCopyDetails ( const CameraMtx* pCamera );

    virtual void SetPosition    ( const D3DXVECTOR3& Position );
    virtual void Move           ( const D3DXVECTOR3& Shift );

	virtual void Forward		( float elapsedTime );
	virtual void Backward		( float elapsedTime );

	virtual void Rotate         ( float x, float y, float z )   {}
	//Local Axis Rotation
	virtual void Pitch			( float angle );
	virtual void Yaw			( float angle );
	virtual void Roll			( float angle );
	//Updates the Camera depending of its mode
	virtual void Update         ( float TimeScale, float Lag )	{}

	virtual CameraMode GetCameraMode    ( ) const	{ return CameraMode::Base;}

	#ifdef _DEBUG
		//Renders the camera axis and model
		//Parameter:
		//- pDevice		the DX device to render [IN]
		//- pEffect		the effect to render [IN]
		virtual void DebugRenderCamera(LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT pEffect);

		//Renders the frustrum of the camera
		//Parameter:
		//- pDevice		the DX device to render the frustrum [IN]
		//- pEffect		the effect to render the frustrum [IN]
		virtual void DebugRenderFrustrum(LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT pEffect);
	#endif


};
