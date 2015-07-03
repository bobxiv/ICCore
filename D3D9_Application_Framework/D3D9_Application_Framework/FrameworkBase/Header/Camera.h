////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					Camera Manager				  //////////////////////////////////////
///////////////////////////////////						Basic					  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <d3dx9.h>

class CameraMG
	{
	private:
		HRESULT hr;

			//The Camera Resources
		LPDIRECT3DDEVICE9 pD3dDev;
		D3DPRESENT_PARAMETERS presentParameters;
			
			//The Camera Transformations
		D3DXMATRIX mProj;
		D3DXMATRIX mView;

			//The coordinates sistem of the Camera Space
		D3DXVECTOR3 Eye;
		D3DXVECTOR3 Target;
		D3DXVECTOR3 Up;

	public:

		CameraMG(): pD3dDev(NULL){}
		CameraMG(LPDIRECT3DDEVICE9 pDev, D3DPRESENT_PARAMETERS params): pD3dDev(pDev), presentParameters(params){}

		~CameraMG(void);

		void SetDevice(LPDIRECT3DDEVICE9 pDev);

		void SetPresentParameters(D3DPRESENT_PARAMETERS params);

			//Changes and sets the projection of the camera
			//Uses a perspective projection
		bool SetProjection(float FoV, float zN, float zF);
			//Changes the target of the camera
		void LookAt(D3DXVECTOR3 target);
			//Changes the position of the camera
		void Position(D3DXVECTOR3 position);
			//Changes the up coordinates of the camera
		void Upper(D3DXVECTOR3 position);
			//To be call every loop that one of the 3 camera vector are updated
		bool UpdateView();

		void GetTotalTransformation(D3DXMATRIX&);

		void GetPosition(D3DXVECTOR3**);

		void GetTarget(D3DXVECTOR3**);
	};

