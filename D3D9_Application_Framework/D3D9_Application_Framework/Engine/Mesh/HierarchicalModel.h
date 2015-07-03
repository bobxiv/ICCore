#pragma once
#include "..\\..\\stdafx.h"
#include <map>
#include "XFrameParser.h"


struct EffectData
{
	EffectData(LPD3DXEFFECT pE, DWORD NumDef, LPD3DXEFFECTDEFAULT pDef): 
		pEffect(pE), NumDefaultsVars(NumDef), pDefaultsVars(pDef){}

	LPD3DXEFFECT		pEffect;
	DWORD               NumDefaultsVars;
    LPD3DXEFFECTDEFAULT pDefaultsVars;
};

namespace XFileModel
{

	//Represent a Hierarchical 3D Model:
	//- Can loads XFile meshes
	//- Can Render them
	class HierarchicalModel
	{
	private:

		//Parser for XFile
		XFileParser::XFrameParser m_Parser;

		//Dictionary of Mesh Data to render
		std::map<std::string,LPDIRECT3DTEXTURE9>			TextureDictionary;
		std::map<LPD3DXMESH,LPDIRECT3DVERTEXDECLARATION9>	VertexDeclarationDictionary;
		std::map<std::string,EffectData>					EffectDictionary;

		//Error control variable
		HRESULT hr;

		//Auxiliary functions:
		//--------------------

		void LoadTextures(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pBone);

		void LoadVertexDeclaration(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pBone);

		void LoadEffects(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pBone, LPD3DXEFFECTPOOL pEffectPool);

		//Calculates the bounding Sphere of the hierarchy mesh, it also needs to 
		//get all the vertices using GetVertexs over pRoot
		//Parameter:
		//- pRoot	the root of the tree to compute the bounding sphere [IN]
		void ComputeBoundingSphere( LPD3DXFRAME pRoot );

		//Calculates the bounding Box of the hierarchy mesh, it also needs to 
		//get all the vertices using GetVertexs over pRoot
		//Parameter:
		//- pRoot	the root of the tree to compute the bounding box [IN]
		void ComputeBoundingBox( LPD3DXFRAME pRoot );

		//Gets all the vertices of all meshes inside every bone from the frame pBone
		//Used to get vertices to calculate Bounding geometries
		//Called recursively over the bone tree
		//Parameter:
		//- pBone				the bone from witch to gets the vertices [IN]
		//- data				the data array of vertices of all the hierarchy mesh [OUT]
		//- dataVertexCount		the number of vertices found, suppose to be enter 0 [OUT]
		//- dataStride			the stride of the vertices array, size of a single vertex in bytes [OUT]
		//Note: if there are 2 meshes with vertices with diferent size then the method throws an exception
		void GetVertexs(LPD3DXFRAME pBone, void*& data, DWORD& dataVertexCount, DWORD& dataStride);
		
		//Could be use D3DXFrameDestroy instead
		void FreeMeshHierarchy(LPD3DXFRAME pBone);

		void RecursiveRender(LPDIRECT3DDEVICE9 pDevice, LPD3DXMATRIX pWorldMatrix, LPD3DXFRAME pBone) throw(std::exception);

	public:
		
		HierarchicalModel() throw(std::exception)
		{}

		HierarchicalModel(std::string Filename, LPDIRECT3DDEVICE9 pDevice, bool LoadStaticMeshes = false, LPD3DXEFFECTPOOL pEffectPool = NULL) 
			throw(std::exception)
		{
			m_Parser.m_LoadMeshes = true;
			//if we load a mesh that don't have frames, by creating a frame for it
			m_Parser.m_ForceHierarchyMesh = LoadStaticMeshes;
			m_Parser.pD3dDev = pDevice;
			m_Parser.Load(Filename.c_str());

			//if no frame
			if( !m_Parser.m_pRootFrame )
				throw std::exception("No Frame Hierarchy in model");
							
			//Updates the Mesh from Object Space -> World Space
			XFileParser::UpdateHierarchy(m_Parser.m_pRootFrame);

			//ComputeBoundingSphere( m_Parser.m_pRootFrame );
			
			//Loads the Mesh Resources needed
			LoadVertexDeclaration(pDevice, m_Parser.m_pRootFrame);

			LoadEffects(pDevice, m_Parser.m_pRootFrame, pEffectPool);

			LoadTextures(pDevice, m_Parser.m_pRootFrame);
		}

		~HierarchicalModel()
		{
			std::map<std::string,LPDIRECT3DTEXTURE9>::iterator itTex = TextureDictionary.begin();
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

			std::map<std::string,EffectData>::iterator itEff = EffectDictionary.begin();
			while( itEff != EffectDictionary.end() )
			{
				itEff->second.pEffect->Release();
				++itEff;
			}
			EffectDictionary.clear();
			
			FreeMeshHierarchy(m_Parser.m_pRootFrame);
		}

		//Render the Model
		//Parameter:
		//- pDevice				the Direct3D Device to render the model [IN]
		//- pWorldMatrix		the world transformation for the model [OPCIONAL] [IN]
		void Render(LPDIRECT3DDEVICE9 pDevice, LPD3DXMATRIX pWorldMatrix = NULL) throw(std::exception)
		{
			if( !pWorldMatrix )
			{
				D3DXMATRIX WorldMatrix;
				D3DXMatrixIdentity(&WorldMatrix);
				RecursiveRender(pDevice, &WorldMatrix, m_Parser.m_pRootFrame);
			}else
				RecursiveRender(pDevice, pWorldMatrix, m_Parser.m_pRootFrame);

			return;
		}

	};

}