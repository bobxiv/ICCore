#pragma once

#include "IC.h"

#include "FrameworkBase\\Header\\GUI\\GUI.h"
#include "FrameworkBase\\Header\\ProPrimitives\\Quad.h"

#include "FrameworkBase\\Header\\GUI\\GUIMaterial.h"

//Renders a SOM
//
//Responsibilities:
//				   - Renders a SOM that is assign on construction
class SOMRenderer
{
private:
	SOMRenderer(){};//NO DEFAULT CONSTRUCTOR

	//The reference to the SOM object
	SOM* pSOM;

	LPDIRECT3DVERTEXDECLARATION9 pQuadVertexDecl;

	//The quads to display the pSOM neurons
	std::vector<ProPrimitive::Quad> Neurons;
	
	std::vector<D3DXMATRIX> NeuronPositions;
	
	std::vector<D3DXVECTOR3> TopologyLines;

	LPDIRECT3DVERTEXBUFFER9 TopologyVB;

	//The Direct 3D Device to create vertex/index buffer and draw
	LPDIRECT3DDEVICE9 pD3dDev;

	//Width of a neuron representation
	float dx;
	//Height of a neuron representation
	float dy;

	//The texture of a Neuron representation
	LPDIRECT3DTEXTURE9 pNeuronTexture;

	//The texture of a Neuron that win this Seasson representation
	LPDIRECT3DTEXTURE9 pNeuronWinnerTexture;

public:
	SOMRenderer(SOM* SOMRef, float DX, float DY, GUIFramework::GUIBasicEffect& BasicEffect, LPDIRECT3DDEVICE9 pDevice) throw (std::exception);

	~SOMRenderer();

	void Update();

	void Draw();

};