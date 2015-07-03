#pragma once

#include "stdafx.h"

#include "XParser.h"
#include "XFrameParser.h"

namespace XFileModel
{

	namespace XFileParser
	{

		BOOL XFrameParser::ParseObject(ID3DXFileData* pDataObj,
											  ID3DXFileData* pParentObj,
											  DWORD Depth,
											  void** Data,
											  BOOL Reference) throw(std::exception)
			{
				//Important, in this ParseObject Data carries the Parent Frame, if one exists

			HRESULT hr=0;
			//Gets the GUID to know what is this XFileData object
			GUID* pGuid = GetObjectGUID(pDataObj);

			//if it is a Frame
			if( *pGuid == TID_D3DRMFrame && !Reference )
				{
					//Creates and sets the new Frame
				D3DXFRAME_Derived* pNewFrame = new D3DXFRAME_Derived;
				//ZeroMemory(pNewFrame, sizeof(D3DXFRAME_Derived));
				pNewFrame->Name = GetObjectName(pDataObj);

					//Chain this Frame in the current hierarchy of Frames
				if( Data == NULL )//if this frame don't have a parent
					{
					pNewFrame->pFrameSibling = m_pRootFrame;
					m_pRootFrame = pNewFrame;
					Data = (void**)pNewFrame;
					pNewFrame = NULL;
					}else
					{
					D3DXFRAME* pFatherFrame = (D3DXFRAME*)Data;
					pNewFrame->pFrameSibling = pFatherFrame->pFrameFirstChild;
					pFatherFrame->pFrameFirstChild = pNewFrame;
					Data = (void**)pNewFrame;
					pNewFrame = NULL;
						}
				}

			//if it is the Matrix of Transformation of a Frame
			if( *pGuid == TID_D3DRMFrameTransformMatrix && !Reference )
				{
				D3DXFRAME* pCurrentFrame = (D3DXFRAME*)Data;//the current Frame
				if( pCurrentFrame )//if we are standing in a Frame
					{
					D3DXMATRIX* pTransMat;
					SIZE_T size;
					pDataObj->Lock(&size,(const void**)&pTransMat);
					pCurrentFrame->TransformationMatrix = *pTransMat;
					pDataObj->Unlock();
					}
				}
			
			//should we load the mesh information?
			if( m_LoadMeshes )
			{
				//if it is a mesh
				if( *pGuid == TID_D3DRMMesh && !Reference  && (Data || m_ForceHierarchyMesh) )
					{
					//if we load it although it is not inside a frame(m_ForceHierarchyMesh = true)
					// or it is the Mesh of a Frame
					if( m_ForceHierarchyMesh && !Data)//not inside frame and forcing hierarchy
						{
							D3DXFRAME_Derived* pNewFrame = new D3DXFRAME_Derived;
							pNewFrame->Name = GetObjectName(pDataObj);
							pNewFrame->pFrameSibling = m_pRootFrame;
							m_pRootFrame = pNewFrame;
							Data = (void**)pNewFrame;
						}
							
					D3DXFRAME* pCurrentFrame = (D3DXFRAME*)Data;//the current Frame
					if( pCurrentFrame )//if we are standing in a Frame
						{
						LPD3DXMESHCONTAINER pMeshCont = new D3DXMESHCONTAINER;
						ZeroMemory(pMeshCont, sizeof(D3DXMESHCONTAINER));
						pMeshCont->Name = pCurrentFrame->Name;
						
						//Buffers to get the loaded data
						LPD3DXBUFFER pMaterialBuffer = NULL;
						LPD3DXBUFFER pAdjacencyBuffer = NULL;
						LPD3DXBUFFER pEffectsBuffer = NULL;
						hr = D3DXLoadSkinMeshFromXof(pDataObj, D3DXMESH_SYSTEMMEM,this->pD3dDev, (LPD3DXBUFFER*)&pAdjacencyBuffer, 
							(LPD3DXBUFFER*)&pMaterialBuffer, (LPD3DXBUFFER*)&pEffectsBuffer, 
							&pMeshCont->NumMaterials, &pMeshCont->pSkinInfo,
							&pMeshCont->MeshData.pMesh);
	 
						//they are stored as LPD3DXBuffer
						pMeshCont->pMaterials = (LPD3DXMATERIAL)pMaterialBuffer;
						pMeshCont->pAdjacency = (DWORD*)pAdjacencyBuffer;
						pMeshCont->pEffects   = (LPD3DXEFFECTINSTANCE)pEffectsBuffer;

						if( FAILED(hr) )
							throw std::exception("The mesh could't be loaded in XFrameParser");

						pMeshCont->pNextMeshContainer = pCurrentFrame->pMeshContainer;
						pCurrentFrame->pMeshContainer = pMeshCont;
						}
					}

			}
				
			//Parse Children
			return ParseChildObject(pDataObj,Depth,Data,Reference);
			}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////	  Utility Functions	     ////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////


		D3DXFRAME* FindFrame(D3DXFRAME* Node, const char* Name)
			{
			if( Node && Node->Name && Name )//is this one?
				{
				if( !strcmp(Node->Name,Name) )
					return Node;
				}

			if( Node && Node->pFrameSibling )//is a sibling?
				{
				D3DXFRAME* result = FindFrame(Node->pFrameSibling, Name);
				if( result )
					return result;
				}
			
			if( Node && Node->pFrameFirstChild )//is a children?
				{
				D3DXFRAME* result = FindFrame(Node->pFrameFirstChild, Name);
				if( result )
					return result;
				}

			return NULL;//not this one and neither related
			}

		void UpdateHierarchy(D3DXFRAME* Node, D3DXMATRIX* TransformMat)
			{
			if( !TransformMat )//by default make the transformation an identity
				{
				TransformMat = new D3DXMATRIX;
				D3DXMatrixIdentity(TransformMat);
				}

			Node->TransformationMatrix *= (*TransformMat);

			if( Node->pFrameSibling )//apply to sibling
				UpdateHierarchy(Node->pFrameSibling, TransformMat);
				

			if( Node->pFrameFirstChild )//apply to children
				UpdateHierarchy(Node->pFrameFirstChild, &Node->TransformationMatrix);

			return;
			}

	}

}