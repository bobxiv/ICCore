#include "stdafx.h"

#include "Grid.h"
#include "FrameworkBase\\Header\\Utility.h"


//================================================
/////////////Constructors/Destructors/////////////
//================================================

Grid::Grid(float delta, LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXDECLARATION9 pGridDeclaration): mDelta(delta), pD3dDev(pDevice), mIndexs(NULL), mpVertexGridDeclaration(pGridDeclaration), hTechnique(NULL), PassCount(0)
{
	pD3dDev->AddRef();
	mpVertexGridDeclaration->AddRef();

	mGridBroad = ceil(1/(double)delta)+1;

	mIndexCount  = ((mGridBroad-1)*(mGridBroad-1)*2)*3;
	mVertexCount = mGridBroad*mGridBroad;

	
	VertexColor* Grid   = new VertexColor[mVertexCount];
	mIndexs = new int[mIndexCount];

	//Equaly grid Parallelized
	Concurrency::parallel_for(size_t(0),size_t(mVertexCount), [&](int value)
	{
		int i = int(value / mGridBroad);
		int j = value % mGridBroad;
		int index = i*mGridBroad+j;
		Grid[index].SetPosition( min(mDelta*i,1.0), min(mDelta*j,1.0), 0.0f);

		//ClockWishe ordering
		if( j < (mGridBroad-1) )
		{
			int pos = 0;
			if( i == 0 )
			{
				pos = 3*j;
			}else if( i == (mGridBroad-1))
			{
				pos = (mGridBroad-1)*6*(i-1)+3*j;
				pos += (mGridBroad-1)*3;
			}else
			{
				pos = (mGridBroad-1)*6*(i-1)+6*j;
				pos += (mGridBroad-1)*3;
			}

			//First
			if( i != (mGridBroad-1) )
			{
					

				mIndexs[pos] = index;
				pos++;
				mIndexs[pos] = index + 1;
				pos++;
				mIndexs[pos] = index + mGridBroad + 1;
				pos++;
			}

			//Second
			if( i != 0 )
			{
				mIndexs[pos] = index;
				pos++;
				mIndexs[pos] = index - mGridBroad;
				pos++;
				mIndexs[pos] = index + 1;
				pos++;
			}
		}

	});

	try
	{
		_CreateGridIndexBuffers();
		_CreateGridVertexBuffers(Grid);
	}catch(std::exception* e)
	{
		throw e;
	}

	delete[] Grid;
}

Grid::~Grid()
{
	delete[] this->mIndexs;
	mpGridVertexsBuffer->Release();
	mpGridAdjacencyBuffer->Release();
	pD3dDev->Release();
	mpVertexGridDeclaration->Release();
}

//================================================
/////////////     Private Methods    /////////////
//================================================

void Grid::_CreateGridVertexBuffers( VertexColor* GridVertexs )
{
	hr = CreateFillBuffer<VertexColor>(pD3dDev, &mpGridVertexsBuffer, mVertexCount, GridVertexs, NULL);

	if( FAILED(hr) )
		throw(new std::exception("Grid Vertex Buffer imposible to create"));
}

void Grid::_CreateGridIndexBuffers()
{
	hr = CreateFillIndexBuffer<int>(pD3dDev, &mpGridAdjacencyBuffer, mIndexCount, mIndexs, D3DFMT_INDEX32);

	if( FAILED(hr) )
		throw(new std::exception("Grid Vertex Buffer imposible to create"));
}

//================================================
/////////////     Public Methods     /////////////
//================================================

int Grid::GetGridBroad()
{
	return mGridBroad;
}

int Grid::GetVertexCount()
{
	return mVertexCount;
}

float Grid::GetDelta()
{
	return mDelta;
}

VertexColor* Grid::LockGridVertexs()
{
	void* Data = NULL;
	hr = mpGridVertexsBuffer->Lock(0, mVertexCount*sizeof(VertexColor), &Data, NULL);
	if( FAILED(hr) )
		throw(new std::exception("Grid: LockGridVertexs -> Fail to lock the Vertex buffer"));
	return (VertexColor*)Data;
}

void Grid::UnlockGridVertexs()
{
	hr = mpGridVertexsBuffer->Unlock();
	if( FAILED(hr) )
		throw(new std::exception("Grid: UnlockGridVertexs -> Fail to unlock the Vertex buffer"));
}

void Grid::Draw( GUIFramework::GUIBasicEffect& BasicEffect )
{
	hTechnique = BasicEffect.pEffect->GetTechniqueByName("SolidEffect");
		
	if( hTechnique == NULL )
		throw(new std::exception("Grid: Draw -> The effect passed don't have the SolidEffect technique"));
	BasicEffect.pEffect->SetTechnique(hTechnique);

	D3DXMATRIX proj;
	D3DXMatrixOrthoOffCenterLH(&proj, 0.0f, 1.0f, 0.0f, 1.0f, 0.1f, 100.0f);
	D3DXMATRIX view;
	D3DXVECTOR3 eye(0.0f,0.0f, -1.0);
	D3DXVECTOR3 at(0.0f,0.0f, 0.0);
	D3DXVECTOR3 up(0.0f,1.0f, 0.0);
	D3DXMatrixLookAtLH(&view, &eye, &at, &up);
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	//Camera.GetTotalTransformation(proj);
	hr = BasicEffect.pEffect->SetMatrix( "mPV", &(proj*view));
	if( FAILED(hr) )
		throw(new std::exception("Grid: Draw -> Fail to set a parameter to the effect"));
	hr = BasicEffect.pEffect->SetMatrix( "mW", &world);
	if( FAILED(hr) )
		throw(new std::exception("Grid: Draw -> Fail to set a parameter to the effect"));
	D3DXVECTOR4 color(1.0f, 1.0f, 1.0f, 1.0f);
		hr = BasicEffect.pEffect->SetVector( "modulateColor", &color);
	if( FAILED(hr) )
		throw(new std::exception("Grid: Draw -> Fail to set a parameter to the effect"));
	hr = BasicEffect.pEffect->SetFloat( "Opacity", 1.0f);
	if( FAILED(hr) )
		throw(new std::exception("Grid: Draw -> Fail to set a parameter to the effect"));
		
	hr = pD3dDev->SetStreamSource(0, mpGridVertexsBuffer, 0, sizeof(VertexColor)); 
	if( FAILED(hr) )
		throw(new std::exception("Grid: Draw -> Fail to set a parameter to the effect"));

	hr = pD3dDev->SetIndices(mpGridAdjacencyBuffer);
	if( FAILED(hr) )
		throw(new std::exception("Grid: Draw -> Fail to set a parameter to the effect"));

	BasicEffect.pEffect->Begin(&PassCount,NULL);

		for(UINT i=0;i < PassCount; ++i)
		{
			BasicEffect.pEffect->BeginPass(i);

			hr = pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mVertexCount, 0, mIndexCount/3);
			if( FAILED(hr) )
				throw(new std::exception("Grid: Draw -> Fail to render"));

			BasicEffect.pEffect->EndPass();
		}

		BasicEffect.pEffect->EndPass();

	BasicEffect.pEffect->End();
}