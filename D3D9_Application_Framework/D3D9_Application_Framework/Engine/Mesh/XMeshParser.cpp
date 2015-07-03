#include "stdafx.h"
#include "..\\..\\stdafx.h"

#include "XMeshParser.h"

namespace XFileModel
{

	namespace XFileParser
	{

		BOOL XMeshParser::ParseObject(ID3DXFileData* pDataObj,
											  ID3DXFileData* pParentObj,
											  DWORD Depth,
											  void** Data,
											  BOOL Reference)
		{
		HRESULT hr=0;
		//Gets the GUID to know what is this XFileData object
		GUID* pGuid = GetObjectGUID(pDataObj);
		
		//if it is a Mesh
		if( *pGuid == TID_D3DRMMesh )
			{
			if( pMesh == NULL )
				pMesh = new D3DXMESHCONTAINER;

			LPD3DXBUFFER pMaterialBuffer = NULL;
			LPD3DXBUFFER pAdjacencyBuffer = NULL;
			LPD3DXBUFFER pEffectsBuffer = NULL;

			//Lets D3DX Load the skinned mesh XFileData object for us
			D3DXLoadSkinMeshFromXof(pDataObj, D3DXMESH_SYSTEMMEM,this->pD3dDev, (LPD3DXBUFFER*)&pAdjacencyBuffer, 
				(LPD3DXBUFFER*)&pMaterialBuffer, (LPD3DXBUFFER*)&pEffectsBuffer, 
				&pMesh->NumMaterials, &pMesh->pSkinInfo,
				&pMesh->MeshData.pMesh);

			pMesh->pMaterials = (LPD3DXMATERIAL)pMaterialBuffer;
			pMesh->pAdjacency = (DWORD*)pAdjacencyBuffer;
			pMesh->pEffects   = (LPD3DXEFFECTINSTANCE)pEffectsBuffer;

			pMesh->pNextMeshContainer = NULL;
			pMesh = pMesh->pNextMeshContainer;//Note that m_RootMeshContainer still points the main mesh!

			return TRUE;//Corto la recursion para abajo
			}

		//Parse Children
		return ParseChildObject(pDataObj,Depth,Data,Reference);
		}

	}

}