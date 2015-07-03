#pragma once

#include "XParser.h"

namespace XFileModel
{

	namespace XFileParser
	{

		//Specialization of XParser to parse SkinnedMesh models
		class XMeshParser: public XParser
		{
		private:

			//Specialization of XParser:ParseObject
			virtual BOOL ParseObject(ID3DXFileData* pDataObj,
								 ID3DXFileData* pParentObj,
								 DWORD Depth,
								 void** Data,
								 BOOL Reference);

				//The main mesh container of the file
				LPD3DXMESHCONTAINER m_RootMeshContainer;
				//Temporary loading mesh container
				LPD3DXMESHCONTAINER pMesh;
				//Direct3D Device to Load the D3DXMeshContainer
				LPDIRECT3DDEVICE9 pD3dDev;

		public:

			XMeshParser()
				{
				m_RootMeshContainer= NULL;
				pMesh = NULL;
				pD3dDev = NULL;
				}

			~XMeshParser()
				{
				}

			//Load the Meshes of the XFile
			//Parameter:
			//- Filename			the name of the XFile [IN]
			//- pDevice				the Direct3D Device to create the mesh [IN]
			//- LPD3DXMESHCONTAINER the mesh necessary to render [OUT]
			BOOL Load(std::string Filename,LPDIRECT3DDEVICE9 pDevice,LPD3DXMESHCONTAINER* pMeshCont)
				{
				this->pD3dDev = pDevice;// pDevice->AddRef();
				this->pMesh = new D3DXMESHCONTAINER; 
				m_RootMeshContainer = this->pMesh;

				if( Parse(Filename.c_str(), NULL) == FALSE )
					{
					pDevice->Release();
					return FALSE;
					}

				this->pMesh = this->m_RootMeshContainer;

				(*pMeshCont) = this->pMesh;
				this->m_RootMeshContainer= NULL;
				this->pMesh = NULL;
				this->pD3dDev = NULL;
				

				return TRUE;
				}
		};

	}

}