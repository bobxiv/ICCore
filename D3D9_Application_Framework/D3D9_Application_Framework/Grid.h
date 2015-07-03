#pragma once

#include "CommonVertexClases.h"//We use VertexColor for the grid

#include "FrameworkBase\\Header\\GUI\\GUIMaterial.h"

//This class is responsible of creating the vertex grid to take as
//samples of the Neural Network. It should:
//
//						-Creates the vertex Grid and indices
//						-Draws the Grid
//						-Gives a reference to the vertexs to recalculate the NN value at that vertex
//Considerations:
//						-If the direct 3d device is recreated this objects should be re-created

class Grid
{
private:

	//The number of vertexs in the grid
	int			 mVertexCount;
	//The number of indices in the grid mesh
	int			 mIndexCount;
	int*		 mIndexs;
	//Delta space in x and y to create the grid
	float		 mDelta;
	//Width and Heiht of the Grid
	int			 mGridBroad;

	//The Direct 3D Device to create vertex/index buffer and draw
	LPDIRECT3DDEVICE9 pD3dDev;

	//Vertex Buffer of the grid
	LPDIRECT3DVERTEXBUFFER9 mpGridVertexsBuffer;

	//Index Buffer of the grid
	LPDIRECT3DINDEXBUFFER9  mpGridAdjacencyBuffer;

	//The vertex declaration to use in the draw of the grid
	LPDIRECT3DVERTEXDECLARATION9 mpVertexGridDeclaration;

	//Auxiliar variables of the Draw Method
	D3DXHANDLE hTechnique;
	UINT PassCount;

	HRESULT hr;

	void _CreateGridVertexBuffers( VertexColor* GridVertexs ) throw(std::exception);

	void _CreateGridIndexBuffers() throw(std::exception);

public:

	Grid(float delta, LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXDECLARATION9 pGridDeclaration);

	~Grid();

	int GetGridBroad();

	int GetVertexCount();

	float GetDelta();

	VertexColor* LockGridVertexs();

	void UnlockGridVertexs();

	void Draw( GUIFramework::GUIBasicEffect& BasicEffect ) throw (std::exception);
	

};