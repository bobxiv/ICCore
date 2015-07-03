#pragma once
#include <map>
#include <string>
#include "XFrameParser.h";
#include "XMeshParser.h";
#include "XAnimationParser.h"
#include "AnimationClasses.h"

namespace XFileModel
{

	//Represent a Skinned 3D Model:
	//- Can loads XFile meshes
	//- Can play its animations
	//- Can Render them
	class SkinnedModel
	{
	private:
		
		//Parser for XFile
		XFileParser::XFrameParser	  m_FrameParser;
		XFileParser::XMeshParser	  m_MeshParser;
		XFileParser::XAnimationParser m_AnimationParser;

		//Dictionary of Mesh Data to render
		std::map<std::string,LPDIRECT3DTEXTURE9> TextureDictionary;
		std::map<LPD3DXMESH,LPDIRECT3DVERTEXDECLARATION9> VertexDeclarationDictionary;

		HRESULT hr;

		//Auxiliary functions:
		//--------------------

		void LoadTextures(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pBone)
		{
		}
		
		void LoadVertexDeclaration(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pBone)
		{
		}
		
		//Could be use D3DXFrameDestroy instead
		void FreeMeshHierarchy(LPD3DXFRAME pBone);

	public:
		
		SkinnedModel(std::string Filename, LPDIRECT3DDEVICE9 pDevice);

		~SkinnedModel()
		{
			/*std::map<std::string,LPDIRECT3DTEXTURE9>::iterator itTex = TextureDictionary.begin();
			while( itTex != TextureDictionary.end() )
			{
				itTex->second->Release();
				++itTex;
			}
			TextureDictionary.clear();

			std::map<LPD3DXMESH,LPDIRECT3DVERTEXDECLARATION9>::iterator itDecl = VertexDeclarationDictionary.begin();
			while( itDecl != VertexDeclarationDictionary.end() )
			{
				itDecl->second->Release();
				++itDecl;
			}
			VertexDeclarationDictionary.clear();
			
			FreeMeshHierarchy(m_Parser.m_pRootFrame);*/
		}

		LPD3DXMESHCONTAINER pMeshContainer;
		LPD3DXBONECOMBINATION pBoneCombinationTable;
		DWORD NumBoneCombinations;

		DWORD m_NumberOfBones;
		//Have the transformations of each frame link
		D3DXMATRIX** m_ppBoneMatrixArray; 
		//The final transformation
		D3DXMATRIX* m_pBonePalette;

		LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;
		DWORD m_MaxVertexInfluence;

		bool PlayingAnimation;
		std::string CurrentAnimation; 
		bool LoopAnimation;
		DWORD AnimationTime;

		D3DXMATRIX* matBone;

		//Render the Model
		//Parameter:
		//- pDevice				the Direct3D Device to render the model [IN]
		//- pWorldMatrix		the world transformation for the model [OPCIONAL] [IN]
		void Render(LPDIRECT3DDEVICE9 pDevice, __int64 ElapsedTime, LPD3DXMATRIX pWorldMatrix = NULL) throw(std::exception)
		{
			
			//Animation Player
			if( PlayingAnimation )
			{
				#pragma region Animation Update 
				AnimationTime += ElapsedTime;

				XFileModel::XFileParser::XAnimationSet* pCurAnimationSet = m_AnimationParser.FindAnimationSet( CurrentAnimation.c_str() );
				DWORD AnimationLength = pCurAnimationSet->m_Length;

				//Update the bones with the animation keyframes
				if( LoopAnimation )
					m_AnimationParser.Update(CurrentAnimation.c_str(), AnimationTime%AnimationLength );
				else
				{
					
					if( AnimationTime >= AnimationLength )
					{
						PlayingAnimation = false;
						AnimationTime = 0;
					}else
						m_AnimationParser.Update(CurrentAnimation.c_str(), min(AnimationTime,AnimationLength) );
				}

				//Combine the new bones transformations
				D3DXFRAME_Derived::UpdateHierarchy(m_FrameParser.m_pRootFrame);

				//Trasform the Bone Matrix Palette
				for( int i=0;i < m_NumberOfBones; ++i )
					{
					m_pBonePalette[i] =  *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);

					if( m_ppBoneMatrixArray[i] )
						m_pBonePalette[i] *= (*m_ppBoneMatrixArray[i]);

					}

				//hr = pEffect->SetMatrixArray("WorldBoneMatrixs", TinySkinned.m_pBonePalette/*matBone*/, TinySkinned.m_NumberOfBones);
				//if( FAILED(hr) )
				//	MessageBoxA(NULL,"No se puede cargar un arreglo de matrizes en el effecto","Error de memoria en el shader",MB_OK);
				#pragma endregion 

			}

			if( !pWorldMatrix )
			{
				pWorldMatrix = new D3DXMATRIX;
				D3DXMatrixIdentity(pWorldMatrix);
			}

			LPD3DXMESH& pMesh = pMeshContainer->MeshData.pMesh;

			pDevice->SetVertexDeclaration(m_pVertexDeclaration);
			
			for( int i=0; i < pMeshContainer->NumMaterials ; ++i )
			{
				hr = pMesh->DrawSubset(i);
				if( FAILED(hr) )
					throw std::exception("SkinnedMesh could not be rendered");
			}

			return;
		}

		void GetMeshBoundingSphere(D3DXVECTOR3& Center, float& Radius)
		{
			LPD3DXVECTOR3 pData;
			LPD3DXMESH& pMesh = pMeshContainer->MeshData.pMesh;
			pMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pData);

			D3DXComputeBoundingSphere( pData, pMesh->GetNumVertices(), D3DXGetFVFVertexSize(pMesh->GetFVF()), &Center, &Radius);

			pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

			return;
		}

		void PlayAnimation(std::string AnimationName, bool Loop, DWORD StartTime = 0) throw(std::exception)
		{
			AnimationTime = StartTime;
			CurrentAnimation = AnimationName;
			PlayingAnimation = true;
			LoopAnimation = Loop;
			return;
		}

	};

}