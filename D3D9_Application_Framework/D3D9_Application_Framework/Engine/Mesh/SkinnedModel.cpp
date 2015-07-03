#pragma once

#include "stdafx.h"

#include "ExtendedFrame.h"
#include "SkinnedModel.h"


namespace XFileModel
{

	SkinnedModel::SkinnedModel(std::string Filename, LPDIRECT3DDEVICE9 pDevice): m_ppBoneMatrixArray(NULL), m_pBonePalette(NULL), m_NumberOfBones(0),
		m_pVertexDeclaration(NULL), pMeshContainer(NULL), m_MaxVertexInfluence(0), PlayingAnimation(false), LoopAnimation(false), AnimationTime(0)
		{
			//CurrentAnimation = "Walk";

			//Loads the Bones
			m_FrameParser.pD3dDev = pDevice;
			m_FrameParser.Load(Filename.c_str());

			//XFileParser::UpdateHierarchy(m_FrameParser.m_pRootFrame);
			D3DXFRAME_Derived::UpdateHierarchy(m_FrameParser.m_pRootFrame);

			//Loads the mesh
			m_MeshParser.Load(Filename, pDevice, &pMeshContainer);

			LPD3DXSKININFO& pSkinInfo = pMeshContainer->pSkinInfo;
			LPD3DXMESH& pMesh = pMeshContainer->MeshData.pMesh;

			DWORD NumOfBoneMatrices = pSkinInfo->GetNumBones();//NOT RIGHT
			m_NumberOfBones = pSkinInfo->GetNumBones();
			NumBoneCombinations =0;
			LPD3DXBUFFER pBoneCombinationTableBuf = NULL;

			hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh(pMesh, D3DXMESH_SYSTEMMEM,
				NumOfBoneMatrices, pMeshContainer->pAdjacency, NULL, NULL, NULL, &m_MaxVertexInfluence,
				&NumBoneCombinations, &pBoneCombinationTableBuf, &pMesh);

			if( FAILED(hr) )
				MessageBoxA(NULL, "Error en skinned mesh", "Skinned Mesh", MB_OK);

			pBoneCombinationTable = (LPD3DXBONECOMBINATION)pBoneCombinationTableBuf->GetBufferPointer();

			D3DXFRAME Frame;
			Frame.Name="root";
			Frame.pFrameFirstChild = NULL;
			Frame.pFrameSibling = NULL;
			D3DXMatrixIdentity(&Frame.TransformationMatrix);
			Frame.pMeshContainer = pMeshContainer;

			//Gets, creates and saves the vertex declaration
			D3DVERTEXELEMENT9 Elem[15];
			pMeshContainer->MeshData.pMesh->GetDeclaration(Elem);
			pDevice->CreateVertexDeclaration(Elem, &m_pVertexDeclaration);

	
			pMeshContainer->MeshData.pMesh->UpdateSemantics(Elem);


			m_pBonePalette = new D3DXMATRIX[m_NumberOfBones];
			//Load the Frame Matrixs in an Array
			m_ppBoneMatrixArray = new D3DXMATRIX*[m_NumberOfBones];
			for( int i=0; i < m_NumberOfBones ; ++i )
				{
				std::string name = pMeshContainer->pSkinInfo->GetBoneName(i);
				D3DXFRAME_Derived* pFrame = D3DXFRAME_Derived::FindFrame( m_FrameParser.m_pRootFrame, name.c_str() );
				
				if( pFrame )
					{
					m_ppBoneMatrixArray[i] = &pFrame->CombinedTransformation;
					}else
					{
					m_ppBoneMatrixArray[i] = NULL;
						}
				}	
			//Trasform the Bone Matrix Palette
			for( int i=0;i < m_NumberOfBones; ++i )
				{
					//takes the mesh from Bind Space to Bone Space
				m_pBonePalette[i] =  *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
				//D3DXFRAME_Derived* pBoneFrame = D3DXFRAME_Derived::FindFrame(m_FrameParser.m_pRootFrame,pMeshContainer->pSkinInfo->GetBoneName(i));
				//D3DXMatrixInverse(&m_pBonePalette[i], NULL, &pBoneFrame->CombinedTransformation);

				if( m_ppBoneMatrixArray[i] )
					m_pBonePalette[i] *= (*m_ppBoneMatrixArray[i]);//From Bone Space to Character Space
				}

			D3DXFRAME_Derived::UpdateHierarchy(m_FrameParser.m_pRootFrame);


			//Loads the animations if any
			m_AnimationParser.Load(Filename);

			m_AnimationParser.Map(m_FrameParser.m_pRootFrame);

			//Loads the Mesh Resources needed
				//LoadVertexDeclaration(pDevice, &Frame);

				//LoadTextures(pDevice, &Frame);
		}

}