#pragma once

#include "stdafx.h"

#include "HierarchicalModel.h"


namespace XFileModel
{

	void HierarchicalModel::LoadTextures(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pBone)
		{
			LPD3DXMESHCONTAINER pMeshContainer = pBone->pMeshContainer;
			while( pMeshContainer )
			{
				DWORD& BoneMaterialCount = pMeshContainer->NumMaterials;

				//Materials
				LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)((LPD3DXBUFFER)pMeshContainer->pMaterials)->GetBufferPointer();

				//For each material loads its textures
				for( int m = 0; m < BoneMaterialCount ; ++m )
				{
					LPDIRECT3DTEXTURE9 pMeshTexture;
					if( !pMaterials[m].pTextureFilename )//if it have a texture
						continue;
					std::map<std::string,LPDIRECT3DTEXTURE9>::iterator it = TextureDictionary.find(pMaterials[m].pTextureFilename);
					if( it == TextureDictionary.end() )
					{
						hr = D3DXCreateTextureFromFileA(pDevice, pMaterials[m].pTextureFilename, &pMeshTexture);
						if( FAILED(hr) )
							MessageBox(NULL, L"No se encontro una textura del modelo", L"Error", MB_OK|MB_SYSTEMMODAL|MB_ICONERROR);
						TextureDictionary.insert(std::pair<std::string,LPDIRECT3DTEXTURE9>(pMaterials[m].pTextureFilename,pMeshTexture));
					}
				}
				
				pMeshContainer = pMeshContainer->pNextMeshContainer;//advance
			}

			//load sibling textures
			if( pBone->pFrameSibling )
				LoadTextures(pDevice, pBone->pFrameSibling);

			//load children textures
			if( pBone->pFrameFirstChild )
				LoadTextures(pDevice, pBone->pFrameFirstChild);

			return;
		}

	void HierarchicalModel::LoadVertexDeclaration(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pBone)
		{
			LPD3DXMESHCONTAINER pMeshContainer = pBone->pMeshContainer;
			while( pMeshContainer )
			{
				LPD3DXMESH& pMesh = pMeshContainer->MeshData.pMesh;

				D3DVERTEXELEMENT9 Elem[15];
				pMesh->GetDeclaration(Elem);
				LPDIRECT3DVERTEXDECLARATION9 pDecl;
				pDevice->CreateVertexDeclaration(Elem, &pDecl);

				VertexDeclarationDictionary.insert(std::pair<LPD3DXMESH,LPDIRECT3DVERTEXDECLARATION9>(pMesh,pDecl));

				pMeshContainer = pMeshContainer->pNextMeshContainer;//advance
			}
			
			//load sibling vertex declarations
			if( pBone->pFrameSibling )
				LoadVertexDeclaration(pDevice, pBone->pFrameSibling);

			//load children vertex declarations
			if( pBone->pFrameFirstChild )
				LoadVertexDeclaration(pDevice, pBone->pFrameFirstChild);

			return;
		}

	void HierarchicalModel::LoadEffects(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pBone, LPD3DXEFFECTPOOL pEffectPool)
	{
		LPD3DXMESHCONTAINER pMeshContainer = pBone->pMeshContainer;
		while( pMeshContainer )
		{
			DWORD& BoneMaterialCount = pMeshContainer->NumMaterials;

			//Effects Instance info
			LPD3DXEFFECTINSTANCE& pEffectsArray = pMeshContainer->pEffects;
			 
			for( int m = 0; m < BoneMaterialCount ; ++m )
			{
				D3DXEFFECTINSTANCE& EffectInstance = pEffectsArray[m];

				if( !EffectInstance.pEffectFilename )
					continue;
				std::map<std::string,EffectData>::iterator it = EffectDictionary.find(EffectInstance.pEffectFilename);
				if( it == EffectDictionary.end() )
				{
					LPD3DXEFFECT pMeshEffect = NULL;
					LPD3DXBUFFER pErrorBuffer = NULL;
					hr = D3DXCreateEffectFromFileA(pDevice, EffectInstance.pEffectFilename, NULL, NULL, NULL, 
						pEffectPool, &pMeshEffect, &pErrorBuffer);
					if( FAILED(hr) )
						MessageBox(NULL, L"No se encontro un efecto del modelo", L"Error", MB_OK|MB_SYSTEMMODAL|MB_ICONERROR);

					EffectData EData(pMeshEffect, EffectInstance.NumDefaults, EffectInstance.pDefaults);

					EffectDictionary.insert(std::pair<std::string,EffectData>(EffectInstance.pEffectFilename, EData));
				}
			}

			pMeshContainer = pMeshContainer->pNextMeshContainer;//advance
		}
		
		//load sibling effects
		if( pBone->pFrameSibling )
			LoadEffects(pDevice, pBone->pFrameSibling, pEffectPool);

		//load children effects
		if( pBone->pFrameFirstChild )
			LoadEffects(pDevice, pBone->pFrameFirstChild, pEffectPool);

		return;
	}

	void HierarchicalModel::ComputeBoundingSphere( LPD3DXFRAME pRoot )
	{
		void* data = NULL;
		DWORD dataVertexCount = 0;
		DWORD dataStride = -1;
		GetVertexs(pRoot, data, dataVertexCount, dataStride);

		D3DXVECTOR3 Center;
		float		Radius;
		hr = D3DXComputeBoundingSphere((D3DXVECTOR3*)data, dataVertexCount, dataStride, &Center, &Radius);
		assert( !FAILED(hr) );

		delete[] data;
		
		return;
	}

	void HierarchicalModel::ComputeBoundingBox( LPD3DXFRAME pRoot )
	{
		void* data = NULL;
		DWORD dataVertexCount = 0;
		DWORD dataStride = -1;
		GetVertexs(pRoot, data, dataVertexCount, dataStride);

		D3DXVECTOR3 Min;
		D3DXVECTOR3 Max;
		hr = D3DXComputeBoundingBox((D3DXVECTOR3*)data, dataVertexCount, dataStride, &Min, &Max);
		assert( !FAILED(hr) );

		if( data )
			delete[] data;
		
		return;
	}

	void HierarchicalModel::GetVertexs(LPD3DXFRAME pBone, void*& data, DWORD& dataVertexCount, DWORD& dataStride)
	{
		LPD3DXMESHCONTAINER pMeshContainer = pBone->pMeshContainer;

		DWORD CurBoneVertexs = 0;
		//Count all vertexs in this bone and check this bone equals stride 
		if( pMeshContainer )//get first mesh stride
		{
			DWORD aux = pMeshContainer->MeshData.pMesh->GetNumBytesPerVertex();
			(dataStride == DWORD(-1))? dataStride = aux : 0;
			assert( dataStride == aux);
		}

		while( pMeshContainer )
		{
			LPD3DXMESH& pMesh = pMeshContainer->MeshData.pMesh;
			CurBoneVertexs += pMesh->GetNumVertices();
			DWORD curStride = pMesh->GetNumBytesPerVertex();
			assert( curStride == dataStride );
			pMeshContainer = pMeshContainer->pNextMeshContainer;//advance
		}

		pMeshContainer = pBone->pMeshContainer;
		void* dataCur = (CurBoneVertexs>0)? new char[CurBoneVertexs*dataStride] : NULL;

		char* tmpDataPtr = (char*)dataCur;
		//Save all vertexs inside dataCur Array
		while( pMeshContainer )
		{
			LPD3DXMESH& pMesh = pMeshContainer->MeshData.pMesh;

			DWORD dataSize = pMesh->GetNumVertices();

			void* meshData = NULL;
			pMesh->LockVertexBuffer(D3DLOCK_READONLY, &meshData);

				memcpy( (void*)tmpDataPtr, meshData, dataStride*dataSize);

			pMesh->UnlockVertexBuffer();

			tmpDataPtr += dataStride*dataSize;//Move ptr to keep coping data
			
			pMeshContainer = pMeshContainer->pNextMeshContainer;//advance
		}
		
		//Gets Sibling Data
		DWORD SiblingVertexs = 0;
		DWORD AuxStride = dataStride;
		void* dataSibling = NULL;
		if( pBone->pFrameSibling )
			GetVertexs(pBone->pFrameSibling, dataSibling, SiblingVertexs, AuxStride);

		(dataStride == DWORD(-1))? dataStride = AuxStride : 0;	//sometime we will at first get the stride
		assert ( AuxStride == dataStride );//Equal vertex size

		//Gets Child Data
		DWORD ChildVertexs = 0;
		void* dataChild = NULL;
		if( pBone->pFrameFirstChild )
			GetVertexs(pBone->pFrameFirstChild, dataChild, ChildVertexs, AuxStride);

		(dataStride == DWORD(-1))? dataStride = AuxStride : 0;	//sometime we will at first get the stride
		assert ( AuxStride == dataStride );//Equal vertex size

		//Merge all data
		dataVertexCount += CurBoneVertexs+SiblingVertexs+ChildVertexs;

		//Reserve data array
		data = (dataVertexCount>0)? new char[(CurBoneVertexs+SiblingVertexs+ChildVertexs)*dataStride] : NULL;

		//Copy Current/Sibling/Child vertexs to the data array
		if( dataCur )
		{
			memcpy(data, dataCur, dataStride*CurBoneVertexs);
			delete[] dataCur;//Erase Current tmp arrays
		}

		char* dataNext = (char*)data+(dataStride*CurBoneVertexs);
		if( dataSibling )
		{
			memcpy((void*)dataNext, dataSibling, dataStride*SiblingVertexs);
			delete[] dataSibling;//Erase Sibling tmp arrays
		}

		dataNext = (char*)dataNext+(dataStride*SiblingVertexs);
		if( dataChild )
		{
			memcpy((void*)dataNext,	dataChild, dataStride*ChildVertexs);
			delete[] dataChild;//Erase Child tmp arrays
		}

		return;
	}

	void HierarchicalModel::FreeMeshHierarchy(LPD3DXFRAME pBone)
		{
			//Free sibling bones
			if( pBone->pFrameSibling )
				FreeMeshHierarchy(pBone->pFrameSibling);

			//Free child bones
			if( pBone->pFrameFirstChild )
				FreeMeshHierarchy(pBone->pFrameFirstChild);

			//Free bone Name
			delete[] pBone->Name;

			//Free all bone MeshContainers
			while( pBone->pMeshContainer != NULL )
			{
				LPD3DXMESHCONTAINER& pMeshContainer = pBone->pMeshContainer;

				//Note that pMeshContainer->Name refered to pBone->Name 
				//so we already free it

				if( pMeshContainer->pEffects )
					((LPD3DXBUFFER)pMeshContainer->pEffects)->Release();

				switch( pMeshContainer->MeshData.Type )
				{
				default:
				case D3DXMESHTYPE_MESH://in case of Mesh
					pMeshContainer->MeshData.pMesh->Release();
					break;
				case D3DXMESHTYPE_PMESH://in case of Progressive Mesh
					pMeshContainer->MeshData.pPMesh->Release();
					break;
				case D3DXMESHTYPE_PATCHMESH://in case of Patch Mesh
					pMeshContainer->MeshData.pPatchMesh->Release();
					break;
				}

				if( pMeshContainer->pAdjacency )
					((LPD3DXBUFFER)pMeshContainer->pAdjacency)->Release();

				if( pMeshContainer->pMaterials )
					((LPD3DXBUFFER)pMeshContainer->pMaterials)->Release();
				
				if( pMeshContainer->pSkinInfo )
					pMeshContainer->pSkinInfo->Release();

				pBone->pMeshContainer = pMeshContainer->pNextMeshContainer;//next mesh Container
			}
			
			return;
		}

	void HierarchicalModel::RecursiveRender(LPDIRECT3DDEVICE9 pDevice, LPD3DXMATRIX pWorldMatrix, LPD3DXFRAME pBone) throw(std::exception)
		{
			//if it haves a mesh to render
			if( pBone->pMeshContainer )
			{
				LPD3DXMESHCONTAINER& pMeshContainer = pBone->pMeshContainer;
				switch( pMeshContainer->MeshData.Type )
				{
				default:
				case D3DXMESHTYPE_MESH://in case of Mesh
					{
					LPD3DXMESH& pMesh = pMeshContainer->MeshData.pMesh;

					//Sets the mesh vertex declaration
					LPDIRECT3DVERTEXDECLARATION9& pDecl = VertexDeclarationDictionary[pMesh];
					pDevice->SetVertexDeclaration(pDecl);
					
					LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)((LPD3DXBUFFER)pMeshContainer->pMaterials)->GetBufferPointer();
					//For each material render its subset
					for( int i = 0; i < pMeshContainer->NumMaterials ; ++i )
					{	
						pDevice->SetTransform(D3DTS_WORLD, &(pBone->TransformationMatrix*(*pWorldMatrix)));
						pDevice->SetMaterial(&pMaterials[i].MatD3D);
						if( pMaterials->pTextureFilename )
							pDevice->SetTexture(0, TextureDictionary[pMaterials->pTextureFilename] );
						hr = pMesh->DrawSubset(i);
						
						if( FAILED(hr) )
							throw std::exception("Imposible to Render MeshSubset");
					}
					
					}
					break;
				case D3DXMESHTYPE_PMESH://in case of Progressive Mesh
					{
						throw std::exception("HierarchicalModel don't render Progressibe Mesh");
					}
					break;
				case D3DXMESHTYPE_PATCHMESH://in case of Patch Mesh
					{
						throw std::exception("HierarchicalModel don't render Patch Mesh");
					}
					break;
				}
			}

			//render sibling
			if( pBone->pFrameSibling )
				RecursiveRender(pDevice, pWorldMatrix, pBone->pFrameSibling);

			//render children
			if( pBone->pFrameFirstChild )
				RecursiveRender(pDevice, pWorldMatrix, pBone->pFrameFirstChild);
				
			return;
		}


}