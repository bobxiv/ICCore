#pragma once

#include "ExtendedFrame.h"
#include "XParser.h"

namespace XFileModel
{

	namespace XFileParser
	{

		//class XFrame: public D3DXFRAME
			//{
			//public:
			//	D3DXMATRIX matOriginal;
			//};

		//Specialization of XParser to parse models with FrameStructure
		//(Gets the Frame Hierarchy, their transformations and Frames Meshes)
		class XFrameParser: public XParser
		{
		private:

			//Specialization of XParser:ParseObject
			virtual BOOL ParseObject(ID3DXFileData* pDataObj,
								 ID3DXFileData* pParentObj,
								 DWORD Depth,
								 void** Data,
								 BOOL Reference);

			

		public:

			//Main Frame of the XFile Loaded
			//This Frame could have siblings and children linked
			D3DXFRAME_Derived* m_pRootFrame;
			
			//If we should load the mesh information of each Frame [IN]
			bool m_LoadMeshes;
			//if we load meshes, and we find a mesh outside a frame we create it a frame
			bool m_ForceHierarchyMesh;

			LPDIRECT3DDEVICE9 pD3dDev;
			
			XFrameParser(): m_pRootFrame(NULL), m_LoadMeshes(false), m_ForceHierarchyMesh(false), pD3dDev(NULL)
			{}

			~XFrameParser()
			{}

			//Load the Hierarchy of bones of the XFile
			//Parameter:
			//- Filename   the name of the file to get the hierarchy [IN]
			BOOL Load(LPCSTR Filename)
			{
				if( Parse(Filename, NULL) == FALSE )
					return FALSE;
				
				return TRUE;
				}
		};

		//Search in the Frame Hierarchy of a Node(In Pre-Order), by its Name
		//Parameters:
		//- Node	The root Frame, from wich to start the search
		//- Name	The name of the Frame to search
		D3DXFRAME* FindFrame(D3DXFRAME* Node, const char* Name);
		
		//Updates the Frame Hierarchy, transforming the hierarchy by a Transformation
		//Ofcourse this function makes the update considering the hierarchy
		//Parameters:
		//- Node			The root Frame, from wich to start the transformation
		//- TransformMat	The transformation, wich will be applied to the hierarchy
		void UpdateHierarchy(D3DXFRAME* Node, D3DXMATRIX* TransformMat = NULL);


	}

}