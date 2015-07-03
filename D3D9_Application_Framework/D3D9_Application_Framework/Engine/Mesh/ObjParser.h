#pragma once

#include "..\\..\\stdafx.h"

#include "CommonVertexClases.h"

//Parser to load .obj Mesh Model
//(Because loads .obj files it can only load static mesh data)
class ObjParser
{
private:

	#pragma region Private Variables
	//Obj format possible geometry data, load buffer
	std::vector<D3DXVECTOR3> Vertexs;
	std::vector<D3DXVECTOR3> Textures;
	std::vector<D3DXVECTOR3> Normals;

	//Vertex Hashtable indexed by VertexPosition Index, so we can make faster the 
	//vertexs creating with out getting repeted vertexs in the buffer
	struct HashEntry
	{
		HashEntry(): Index(-1), pNextEntry(NULL) {}
		HashEntry(DWORD newIndex, HashEntry* pNext): Index(newIndex), pNextEntry(pNext) {}

		DWORD Index;		  //Index inside mVertexs
		HashEntry* pNextEntry;//Next HashEntry inside bucket
	};
	std::vector<HashEntry*> mVertexsHT;
	//Checks if NewVertex existed(in the HashTable), else adds it to mVertexs array
	//Parameter:
	//- hash				the Vertex position index, wich is the hash code [IN]
	//- NewVertex			the new vertex to check existance and add if nessesary [IN]
	//Returns:
	//	The index inside mVertexs of the NewVertex
	DWORD AddVertex( DWORD hash, VertexTextureNormal& NewVertex);

	//Final Vertex and Index data:
	//if could use smaller format, it convert this to the smaller format before put inside mesh
	std::vector<VertexTextureNormal> mVertexs;
	std::vector<DWORD> mIndexs;

	#pragma endregion

	#pragma region Private Methods
	//In base of the data loaded deduces the unified format needed for the Vertexs
	//Parameter:
	//- FVF					the vertex FVF format for the vertexs [OUT]
	//- pMeshVDeclaration	the vertex declaration for the vertexs [OUT]
	//- code				the to identify the class represented by the FVF/VertexDeclaration [OUT]
	//
	//Note:				Posible codes for the vertexs 
	//					0 -> error		
	//					1 -> Position					=>	class Vertex
	//					2 -> Position Normal			=>	class VertexNormal
	//					3 -> Position Texture			=>	class VertexTexture
	//					4 -> Position Texture Normal	=>	class VertexTextureNormal
	void IdentifieLoadedFormat(DWORD& FVF, D3DVERTEXELEMENT9* &pMeshVDeclaration, int &code);

	//Generates the Vertex Data following the FVF/Vertex Declaration needed to render,
	//in base of the data loaded from the file
	//Parameter:
	//- loadedData	the data in renderable format following the FVF/VD [OUT]
	//- loadedIndex	the indexs in renderable index format [OUT]
	//- size		the size in bytes of each vertex in the renderable format [OUT]
	//- code		to identify the class represented by the FVF/VertexDeclaration [IN]
	//- FacesCount	if facescount > 65535 then mesh is 32bits indexed, else 16 bits [IN]
	void PutLoadedDataInVertexDeclarationFormat(void* &loadedData, void* &loadedIndex, size_t &size, int code, int FacesCount);

	//Generates the normal data of each vertex, so it can render with a smooth shading
	//Puts the normals for each vertex in the Normals array, indexed respectively 
	//for the vertex data
	//
	//Note:	
	//							EXPENSIVE, USE ONLY IF NECESSARY!!!
	void GenerateNormals();

	//Parses and loads a Material Template Library file reference by a .obj file
	//Parameter:
	//- Name	the name of the .mtl file [IN]
	void ParseMaterial(LPCSTR Name)
	{
		return;
	}
	#pragma endregion

public:

	#pragma region Public Variables
	//Commands that if normal information isn't in the obj file
	//then we create it in the fly	(EXTREMELY EXPENSIVE)
	bool m_ForceNormals;

	//Winding of the vertexs in each triangle
	enum VertexWinding{CCW,CW};
	enum VertexMetaFormat{FVF,VertexDeclaration};
	VertexWinding m_Winding;
	VertexMetaFormat m_VertexMetaFormat;

	//Mesh loaded
	LPD3DXMESHCONTAINER m_RootMeshContainer;

	#pragma endregion

	#pragma region Constructors/Destructors & Public Methods
	ObjParser(): m_ForceNormals(false), m_Winding(VertexWinding::CW), m_VertexMetaFormat(VertexMetaFormat::VertexDeclaration)
		, m_RootMeshContainer(NULL)
	{}

	~ObjParser()
	{
		Vertexs.clear();
		Textures.clear();
		Normals.clear();
		mVertexsHT.clear();
		mVertexs.clear();
		mIndexs.clear();
	}

	//Load the Meshes of the .obj File
	//Parameter:
	//- Filename	the name of the .obj File [IN]
	//- pDevice		the Direct3D Device to create the mesh [IN]
	void Load(LPCSTR Filename, LPDIRECT3DDEVICE9 pDevice);

	#pragma endregion

};