#include "stdafx.h"

#include "SOMRenderer.h"

//================================================
/////////////Constructors/Destructors/////////////
//================================================

SOMRenderer::SOMRenderer(SOM* SOMRef, float DX, float DY, GUIFramework::GUIBasicEffect& BasicEffect, LPDIRECT3DDEVICE9 pDevice): pSOM(SOMRef), dx(DX), dy(DY), pD3dDev(pDevice), pQuadVertexDecl(NULL)
	{
		pD3dDev->AddRef();

		for(int i=0; i < (pSOM->mMapHeight*pSOM->mMapWidth); ++i)
		{
			//double* weights = pSOM->mNeuronMap[i].mWeights;
			//ProPrimitive::Quad curNeuron(weights[0], weights[1], weights[0]+dx, weights[1]+dy, pD3dDev);
			ProPrimitive::Quad curNeuron(0.0f, 0.0f, dx, dy, pD3dDev, BasicEffect.pEffect);
			//ProPrimitive::Quad curNeuron(-dx/(float)2, -dy/(float)2, dx/(float)2, dy/(float)2, pD3dDev, BasicEffect.pEffect);
			Neurons.push_back( curNeuron );
		}

		if( FAILED(::D3DXCreateTextureFromFile(pD3dDev, L"D3D9_Application_Framework\\Content\\Images\\Neurona.png", &pNeuronTexture)) )
			throw (new std::exception("SOMRenderer : SOMRenderer -> La texture de la neurona es imposible de cargar"));

		if( FAILED(::D3DXCreateTextureFromFile(pD3dDev, L"D3D9_Application_Framework\\Content\\Images\\NeuronaGanadora.png", &pNeuronWinnerTexture)) )
			throw (new std::exception("SOMRenderer : SOMRenderer -> La texture de la neurona ganadora es imposible de cargar"));

		NeuronPositions.resize(pSOM->mMapHeight*pSOM->mMapWidth);

		if( FAILED(pD3dDev->CreateVertexDeclaration(ProPrimitive::QuadVertexElement, &pQuadVertexDecl)) )
			throw (new std::exception("SOMRenderer : SOMRenderer -> La declaracion de los vertices del quad no se puede crear"));

		TopologyLines.resize( ((pSOM->mMapWidth-1)*(pSOM->mMapHeight-1))*4*2 + (pSOM->mMapWidth-2)*3*2 + (pSOM->mMapHeight-2)*3*2 );

		CreateFillBuffer(this->pD3dDev, &TopologyVB, TopologyLines.size(), &TopologyLines[0], D3DFVF_XYZ);

		this->Update();
	}

SOMRenderer::~SOMRenderer()
{
	pD3dDev->Release();
	pNeuronTexture->Release();
	Neurons.clear();
}

//================================================
/////////////     Public Methods     /////////////
//================================================

void SOMRenderer::Update()
{
	int Xmin = 0;
	int Xmax = 1.0f;
	int Ymin = 0;
	int Ymax = 1.0f;

	int pos=0;

	for(int i=0; i < pSOM->mMapHeight; ++i)
		for(int j=0; j < pSOM->mMapWidth; ++j)
		{
			double* weights = pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights;
			//D3DXMatrixTranslation( &NeuronPositions[i*pSOM->mMapWidth+j], (weights[0]+1.0f)/2.0f, (weights[1]+1)/2.0f, 0.0f);
			//D3DXMatrixTranslation( &NeuronPositions[i], (weights[0]+1)*0.8f, (weights[1]+1)*0.8f, 0.0f);
		
			//Update Neuron Position
			D3DXMatrixTranslation( &NeuronPositions[i*pSOM->mMapWidth+j], (weights[0]+1)*0.9f, (weights[1]+1)*0.9f, 0.0f);
			//D3DXMatrixTranslation( &NeuronPositions[i*pSOM->mMapWidth+j], weights[0]*1.8, weights[1]*1.8, 0.0f);//Para concent

			//Update Topology lines
			if( (j-1) >= 0 )//West
			{
				TopologyLines[pos].x = (pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[0]+1)/2.0f;
				TopologyLines[pos].y = (pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[1]+1)/2.0f;
				TopologyLines[pos].z = 0;
				/*TopologyLines[pos].x = pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[0];//Para concent
				TopologyLines[pos].y = pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[1];
				TopologyLines[pos].z = 0;*/

				pos++;

				TopologyLines[pos].x = (pSOM->mNeuronMap[i*pSOM->mMapWidth+(j-1)].mWeights[0]+1)/2.0f;
				TopologyLines[pos].y = (pSOM->mNeuronMap[i*pSOM->mMapWidth+(j-1)].mWeights[1]+1)/2.0f;
				TopologyLines[pos].z = 0;
				/*TopologyLines[pos].x = pSOM->mNeuronMap[i*pSOM->mMapWidth+(j-1)].mWeights[0];//Para concent
				TopologyLines[pos].y = pSOM->mNeuronMap[i*pSOM->mMapWidth+(j-1)].mWeights[1];
				TopologyLines[pos].z = 0;*/

				pos++;
			}

			if( (j+1) < this->pSOM->mMapWidth )//East
			{
				TopologyLines[pos].x = (pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[0]+1)/2.0f;
				TopologyLines[pos].y = (pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[1]+1)/2.0f;
				TopologyLines[pos].z = 0;
				/*TopologyLines[pos].x = pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[0];//Para concent
				TopologyLines[pos].y = pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[1];
				TopologyLines[pos].z = 0;*/

				pos++;

				TopologyLines[pos].x = (pSOM->mNeuronMap[i*pSOM->mMapWidth+(j+1)].mWeights[0]+1)/2.0f;
				TopologyLines[pos].y = (pSOM->mNeuronMap[i*pSOM->mMapWidth+(j+1)].mWeights[1]+1)/2.0f;
				TopologyLines[pos].z = 0;
				/*TopologyLines[pos].x = pSOM->mNeuronMap[i*pSOM->mMapWidth+(j+1)].mWeights[0];//Para concent
				TopologyLines[pos].y = pSOM->mNeuronMap[i*pSOM->mMapWidth+(j+1)].mWeights[1];
				TopologyLines[pos].z = 0;*/

				pos++;
			}

			if( (i-1) >= 0 )//South
			{
				TopologyLines[pos].x = (pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[0]+1)/2.0f;
				TopologyLines[pos].y = (pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[1]+1)/2.0f;
				TopologyLines[pos].z = 0;
				/*TopologyLines[pos].x = pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[0];//Para concent
				TopologyLines[pos].y = pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[1];
				TopologyLines[pos].z = 0;*/

				pos++;

				TopologyLines[pos].x = (pSOM->mNeuronMap[(i-1)*pSOM->mMapWidth+j].mWeights[0]+1)/2.0f;
				TopologyLines[pos].y = (pSOM->mNeuronMap[(i-1)*pSOM->mMapWidth+j].mWeights[1]+1)/2.0f;
				TopologyLines[pos].z = 0;
				/*TopologyLines[pos].x = pSOM->mNeuronMap[(i-1)*pSOM->mMapWidth+j].mWeights[0];//Para concent
				TopologyLines[pos].y = pSOM->mNeuronMap[(i-1)*pSOM->mMapWidth+j].mWeights[1];
				TopologyLines[pos].z = 0;*/

				pos++;
			}

			if( (i+1) < this->pSOM->mMapHeight )//North
			{
				TopologyLines[pos].x = (pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[0]+1)/2.0f;
				TopologyLines[pos].y = (pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[1]+1)/2.0f;
				TopologyLines[pos].z = 0;
				/*TopologyLines[pos].x = pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[0];//Para concent
				TopologyLines[pos].y = pSOM->mNeuronMap[i*pSOM->mMapWidth+j].mWeights[1];
				TopologyLines[pos].z = 0;*/

				pos++;

				TopologyLines[pos].x = (pSOM->mNeuronMap[(i+1)*pSOM->mMapWidth+j].mWeights[0]+1)/2.0f;
				TopologyLines[pos].y = (pSOM->mNeuronMap[(i+1)*pSOM->mMapWidth+j].mWeights[1]+1)/2.0f;
				TopologyLines[pos].z = 0;
				/*TopologyLines[pos].x = pSOM->mNeuronMap[(i+1)*pSOM->mMapWidth+j].mWeights[0];//Para concent
				TopologyLines[pos].y = pSOM->mNeuronMap[(i+1)*pSOM->mMapWidth+j].mWeights[1];
				TopologyLines[pos].z = 0;*/

				pos++;
			}

		}

}

void SOMRenderer::Draw()
	{
		D3DXMATRIX proj;
		D3DXMatrixOrthoOffCenterLH(&proj, 0.0f, 1.0f, 0.0f, 1.0f, 0.1f, 100.0f);
		D3DXMATRIX view;
		D3DXVECTOR3 eye(0.0f,0.0f, -1.0);
		D3DXVECTOR3 at(0.0f,0.0f, 0.0);
		D3DXVECTOR3 up(0.0f,1.0f, 0.0);
		D3DXMatrixLookAtLH(&view, &eye, &at, &up);

		D3DXMATRIX world;
		::D3DXMatrixIdentity(&world);
		pD3dDev->SetTransform( D3DTS_WORLD, &world);
		pD3dDev->SetTransform( D3DTS_VIEW, &view);
		pD3dDev->SetTransform( D3DTS_PROJECTION, &proj);
		pD3dDev->SetFVF(D3DFVF_XYZ);

		//Draw Map Topology
		void* pData;
		TopologyVB->Lock(0,sizeof(D3DXVECTOR3)*TopologyLines.size(), &pData, NULL);
		std::copy(TopologyLines.begin(), TopologyLines.end(), (D3DXVECTOR3*)pData);
		TopologyVB->Unlock();
		pD3dDev->SetStreamSource(0, TopologyVB, 0, sizeof(D3DXVECTOR3));
		HRESULT hr = this->pD3dDev->DrawPrimitive(D3DPT_LINELIST, 0, TopologyLines.size()/2);
		if( FAILED(hr) )
			throw(new std::exception("SOMRenderer : Draw -> No se puede dibujar las lineas de topologia de las neuronas"));


		Neurons[0].pEffect->SetFloat( "Opacity", 1.0f);
		float ModulateAndFade[4]; ModulateAndFade[0] = 1.0f;ModulateAndFade[1] = 1.0f;ModulateAndFade[2] = 1.0f;ModulateAndFade[3] = 1.0f;
		Neurons[0].pEffect->SetFloatArray( "modulateColor", ModulateAndFade, 4);

		pD3dDev->SetVertexDeclaration(pQuadVertexDecl);

		for(int i=0; i < (pSOM->mMapHeight*pSOM->mMapWidth); ++i)
		{
			if( pSOM->CurContinuousLearnSetup.NeuronWinners[i] )//if winner draw as deserved
				Neurons[i].Draw( pD3dDev, proj*view, NeuronPositions[i], pNeuronWinnerTexture);
			else				  //else just draw the poor bastard
				Neurons[i].Draw( pD3dDev, proj*view, NeuronPositions[i], pNeuronTexture);
		}
	}