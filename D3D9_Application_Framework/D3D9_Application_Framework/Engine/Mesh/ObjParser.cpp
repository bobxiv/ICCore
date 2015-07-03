#pragma once
#include "stdafx.h"

#include "ObjParser.h"

void ObjParser::Load(LPCSTR Filename, LPDIRECT3DDEVICE9 pDevice)
{
		FILE* fileHandle = fopen(Filename, "r+");
		if( fileHandle == NULL )
			return;

		char line[256];
		D3DXVECTOR3 vec;

		while( !feof(fileHandle) )//Foreach Line
		{
			fgets(line, 256, fileHandle);

			#pragma region Line Read
			switch(line[0])
			{
			case 'm':
				{
				LPCSTR name = new CHAR[256];
				sscanf_s(line, "mtllib %s", name, 255);

				ParseMaterial(name);
				}
				break;
			case 'v'://Vertex Item
				{
				switch(line[1])
					{
					case ' '://Vertex
						{
						sscanf_s(line, "v %f %f %f", &vec.x, &vec.y, &vec.z );

						Vertexs.push_back(vec);
						mVertexsHT.push_back(NULL);//expand HashTable
						}
						break;
					case 't'://Texture Coordinates
						{
						D3DXVECTOR3 tex;
						sscanf_s(line, "vt %f %f %f", &vec.x, &vec.y, &vec.z);

						Textures.push_back(vec);
						}
						break;
					case 'n'://Normal
						{
						D3DXVECTOR3 nor;
						sscanf_s(line, "vn %f %f %f", &vec.x, &vec.y, &vec.z );

						Normals.push_back(vec);
						}
						break;
					default:// Not supposed to happen
						assert( false );
					}

				}
				break;
			case 'f'://Face Item
				{
				DWORD quadP[4];//Position Index quad
				DWORD quadT[4];//Texture  Index quad
				DWORD quadN[4];//Normal   Index quad
				memset(quadP, -1, sizeof(DWORD)*4);
				memset(quadT, -1, sizeof(DWORD)*4);
				memset(quadN, -1, sizeof(DWORD)*4);

				int size = strlen(line);
				int barCount = std::count(line, line+size, '/');

				int readed = 0;

				//By default .obj file puts faces with CW winding
				switch( barCount )
					{
					case 0:// tri/quad	pos
						{
							if( m_Winding == VertexWinding::CCW )
								readed = sscanf_s(line, "f %d %d %d %d", &quadP[3], &quadP[2], &quadP[1], &quadP[0] );
							else
								readed = sscanf_s(line, "f %d %d %d %d", &quadP[0], &quadP[1], &quadP[2], &quadP[3] );
							assert( readed == 3 || readed == 4 );
						}
						break;
					case 3:// tri	pos/tex
						{
							if( m_Winding == VertexWinding::CCW )
								readed = sscanf_s(line, "f %d/%d %d/%d %d/%d", 
								&quadP[2], &quadT[2],
								&quadP[1], &quadT[1],
								&quadP[0], &quadT[0]
								);
							else
								readed = sscanf_s(line, "f %d/%d %d/%d %d/%d", 
								&quadP[0], quadT[0],
								&quadP[1], quadT[1],
								&quadP[2], quadT[2]
								);
							assert( readed == 6 );
							readed /= 2;
						}
						break;
					case 4:// quad	pos/tex
						{
							if( m_Winding == VertexWinding::CCW )
								readed = sscanf_s(line, "f %d/%d %d/%d %d/%d %d/%d", 
								&quadP[3], &quadT[3],
								&quadP[2], &quadT[2], 
								&quadP[1], &quadT[1],
								&quadP[0], &quadT[0] 
								);
							else
								readed = sscanf_s(line, "f %d/%d %d/%d %d/%d %d/%d", 
								&quadP[0], &quadT[0],
								&quadP[1], &quadT[1],
								&quadP[2], &quadT[2],
								&quadP[3], &quadT[3]
								);
							assert( readed == 8 );
							readed /= 2;
						}
						break;
					case 6:// tri	pos/tex/nor
						{
							if( m_Winding == VertexWinding::CCW )
								readed = sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
								&quadP[2], &quadT[2], &quadN[2],
								&quadP[1], &quadT[1], &quadN[1],
								&quadP[0], &quadT[0], &quadN[0]
								);
							else
								readed = sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
								&quadP[0], &quadT[0], &quadN[0],
								&quadP[1], &quadT[1], &quadN[1],
								&quadP[2], &quadT[2], &quadN[2]
								);
							assert( readed == 9 );
							readed /= 3;
						}
						break;
					case 8:// quad	pos/tex/nor
						{
							if( m_Winding == VertexWinding::CCW )
								readed = sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
								&quadP[3], &quadT[3], &quadN[3],
								&quadP[2], &quadT[2], &quadN[2],
								&quadP[1], &quadT[1], &quadN[1],
								&quadP[0], &quadT[0], &quadN[0]
								);
							else
								readed = sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
								&quadP[0], &quadT[0], &quadN[0],
								&quadP[1], &quadT[1], &quadN[1],
								&quadP[2], &quadT[2], &quadN[2],
								&quadP[3], &quadT[3], &quadN[3]
								);
							assert( readed == 12 );
							readed /= 3;
						}
						break;
					default:// Not supposed to happen
						assert( false );
					}

				//The indexs are in 1 Base, we transform to 0 Base
				for( int i=0; i < 4 ; ++i )
				{
					quadP[i]--; quadT[i]--; quadN[i]--;
				}

				for(int j=0; j < 3 ; ++j)
					{
						VertexTextureNormal NewVertex;
						NewVertex.SetPosition( Vertexs[quadP[j]] );
						if( quadT[j] != (DWORD(-1)-1) )
							NewVertex.SetTexture ( Textures[quadT[j]].x, Textures[quadT[j]].y );
						if( quadN[j] != (DWORD(-1)-1) )
							NewVertex.SetNormal  ( Normals[quadN[j]] );
						
						DWORD index = AddVertex(quadP[j], NewVertex);
						mIndexs.push_back(index);
					}
 
				if( readed == 4 )// quad readed
					{
						quadP[1] = quadP[2]; quadT[1] = quadT[2]; quadN[1] = quadN[2]; 
						quadP[2] = quadP[3]; quadT[2] = quadT[3]; quadN[1] = quadN[2]; 
												
						for(int j=0; j < 3 ; ++j)
							{
							VertexTextureNormal NewVertex;
							NewVertex.SetPosition( Vertexs[quadP[j]] );
							if( quadT[j] != (DWORD(-1)-1) )
								NewVertex.SetTexture ( Textures[quadT[j]].x, Textures[quadT[j]].y );
							if( quadN[j] != (DWORD(-1)-1) )
								NewVertex.SetNormal  ( Normals[quadN[j]] );
						
							DWORD index = AddVertex(quadP[j], NewVertex);
							mIndexs.push_back(index);
							}
					}

				}
				break;

			}
			#pragma endregion

		}
			
		fclose(fileHandle);
		
		DWORD FVF = NULL;
		D3DVERTEXELEMENT9* pMeshVDeclaration = NULL;
		int code = 0;
		IdentifieLoadedFormat(FVF, pMeshVDeclaration, code);
		if( code == 0 )
			return;
		
		//Setup Mesh with VertexDeclaration corresponding to the loaded data
		LPD3DXMESH pMesh = NULL;
		HRESULT hr = NULL;
		
		int FacesCount = mIndexs.size()/3;
		switch( m_VertexMetaFormat )
		{
		case VertexMetaFormat::VertexDeclaration:
			{
				if( FacesCount > 65535 )// if huge mesh, 32 bits face index
					hr = D3DXCreateMesh(FacesCount, mVertexs.size(), D3DXMESH_32BIT | D3DXMESH_VB_SYSTEMMEM | D3DXMESH_IB_SYSTEMMEM | D3DXMESH_SYSTEMMEM , 
						pMeshVDeclaration, pDevice, &pMesh);
				else
					hr = D3DXCreateMesh(FacesCount, mVertexs.size(), D3DXMESH_VB_SYSTEMMEM | D3DXMESH_IB_SYSTEMMEM | D3DXMESH_SYSTEMMEM , 
						pMeshVDeclaration, pDevice, &pMesh);
			}
			break;
		case VertexMetaFormat::FVF:
			{
				if( FacesCount > 65535 )// if huge mesh, 32 bits face index
					hr = D3DXCreateMeshFVF(FacesCount, Vertexs.size(), D3DXMESH_32BIT | D3DXMESH_VB_SYSTEMMEM | D3DXMESH_IB_SYSTEMMEM | D3DXMESH_SYSTEMMEM ,
						FVF, pDevice, &pMesh);
				else
					hr = D3DXCreateMeshFVF(FacesCount, Vertexs.size(), D3DXMESH_VB_SYSTEMMEM | D3DXMESH_IB_SYSTEMMEM | D3DXMESH_SYSTEMMEM ,
						FVF, pDevice, &pMesh);
			}
			break;
		default:
			assert( false );
		}

		assert( !FAILED(hr) );
		
		//Puts vertex data inside loadedData in the smallest format needed
		//(not nesesarily VertexTextureNormal)
		void* loadedData = NULL;
		void* loadedIndex = NULL;
		size_t size = 0;
		//Pass to our vertex format
		PutLoadedDataInVertexDeclarationFormat(loadedData,loadedIndex,size,code, FacesCount);

		//Free Auxiliary Arrays
		Vertexs.clear();
		Textures.clear();
		Normals.clear();
		mVertexsHT.clear();
		
		void* data = NULL;
		//Loads the Vertex Buffer
		if( FAILED(pMesh->LockVertexBuffer(NULL, &data)) )
			return;

			memcpy(data, loadedData, size*mVertexs.size());

		pMesh->UnlockVertexBuffer();

		//Loads the Index Buffer
		if( FAILED(pMesh->LockIndexBuffer(NULL, &data)) )
			return;

			if( FacesCount > 65535 )
				memcpy(data, loadedIndex, sizeof(DWORD)*mIndexs.size());
			else
				memcpy(data, loadedIndex, sizeof(WORD)*mIndexs.size());

		pMesh->UnlockIndexBuffer();

		//Free main Arrays
		mVertexs.clear();
		mIndexs.clear();

		//Mesh data ready
		m_RootMeshContainer = new D3DXMESHCONTAINER;
		m_RootMeshContainer->MeshData.pMesh = pMesh;

		return;
	}

	void ObjParser::IdentifieLoadedFormat(DWORD& FVF, D3DVERTEXELEMENT9* &pMeshVDeclaration, int &code)
	{
		if( Vertexs.size() != 0 && Normals.size() != 0 && Textures.size() != 0 )//All
		{
			pMeshVDeclaration = VertexTextureNormal::VertexElement;
			FVF = D3DFVF_XYZ | D3DFVF_TEX0 | D3DFVF_NORMAL;
			code = 4;
		}
		else if( Vertexs.size() != 0 && Textures.size() != 0 )//just Texture
		{
			if( m_ForceNormals )
			{
				GenerateNormals();
				pMeshVDeclaration = VertexTextureNormal::VertexElement;
				FVF = D3DFVF_XYZ | D3DFVF_TEX0 | D3DFVF_NORMAL;
				code = 4;
			}else
			{
				pMeshVDeclaration = VertexTexture::VertexElement;
				FVF = D3DFVF_XYZ | D3DFVF_TEX0;
				code = 3;
			}
		}
		else if( Vertexs.size() != 0 && Normals.size() != 0 )//just Normal
		{
			pMeshVDeclaration = VertexNormal::VertexElement;
			FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
			code = 2;
		}
		else
		{
			if( m_ForceNormals )
			{
				GenerateNormals();
				pMeshVDeclaration = VertexNormal::VertexElement;
				FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
				code = 2;
			}else
			{
				pMeshVDeclaration = Vertex::VertexElement;
				FVF = D3DFVF_XYZ;
				code = 1;
			}
		}

		return;
	}

	void ObjParser::PutLoadedDataInVertexDeclarationFormat(void* &loadedData, void* &loadedIndex, 
		size_t &size, int code, int FacesCount)
	{
		// 0 -> error
		// 1 -> Position
		// 2 -> Position Normal
		// 3 -> Position Texture
		// 4 -> Position Texture Normal
		switch(code)
		{
		case 1:
			{
				loadedData = new Vertex[mVertexs.size()];
				for( int i=0; i < mVertexs.size(); ++i )
					((Vertex*)loadedData)[i].SetPosition(mVertexs[i].x,mVertexs[i].y,mVertexs[i].z,1.0f);
				size = sizeof(Vertex);
			}
			break;
		case 2:
			{
				loadedData = new VertexNormal[mVertexs.size()];
				for( int i=0; i < mVertexs.size(); ++i )
				{
					((VertexNormal*)loadedData)[i].SetPosition(mVertexs[i].x,mVertexs[i].y,mVertexs[i].z,1.0f);
					((VertexNormal*)loadedData)[i].SetNormal(mVertexs[i].nx,mVertexs[i].ny,mVertexs[i].nz);
				}
				size = sizeof(VertexNormal);
			}
			break;
		case 3:
			{
				loadedData = new VertexTexture[mVertexs.size()];
				for( int i=0; i < mVertexs.size(); ++i )
				{
					((VertexTexture*)loadedData)[i].SetPosition(mVertexs[i].x,mVertexs[i].y,mVertexs[i].z,1.0f);
					((VertexTexture*)loadedData)[i].SetTexture(mVertexs[i].u,mVertexs[i].v);
				}
				size = sizeof(VertexTexture);
			}
			break;
		case 4:
			{
				loadedData = &(mVertexs[0]);
				size = sizeof(VertexTextureNormal);
			}
			break;
		default:
			assert( false );
		}

		//Convert Index format 32-16 bits, which ever needed
		if( FacesCount > 65535 )
			loadedIndex = &(mIndexs[0]);
		else
		{	
			loadedIndex = new WORD[mIndexs.size()];

			for( int j=0; j < mIndexs.size() ; ++j )
				((WORD*)loadedIndex)[j] = (WORD)mIndexs[j];
		}

		return;
	}

	//Could be use D3DXComputeNormals instead
	void ObjParser::GenerateNormals()
	{
		//Calculate face normal and adds it to each vertex of the face
		for( int i=0; i < mIndexs.size() ; i+=3 )
		{
			D3DXVECTOR3 D1, D2;
			//the vertexs need to be follow in its winding order to get the plane normal
			if( CCW == this->m_Winding )
			{
				D1 = mVertexs[mIndexs[i+1]].GetPosition()-mVertexs[mIndexs[i]].GetPosition();
				D2 = mVertexs[mIndexs[i+2]].GetPosition()-mVertexs[mIndexs[i+1]].GetPosition();
			}else
			{
				D1 = mVertexs[mIndexs[i+1]].GetPosition()-mVertexs[mIndexs[i+2]].GetPosition();
				D2 = mVertexs[mIndexs[i]].GetPosition()-mVertexs[mIndexs[i+1]].GetPosition();
			}

			D3DXVECTOR3 Normal;
			D3DXVec3Cross(&Normal, &D1, &D2);

			mVertexs[mIndexs[i]].SetNormal  ( mVertexs[mIndexs[i]].GetNormal()   + Normal );
			mVertexs[mIndexs[i+1]].SetNormal( mVertexs[mIndexs[i+1]].GetNormal() + Normal );
			mVertexs[mIndexs[i+2]].SetNormal( mVertexs[mIndexs[i+2]].GetNormal() + Normal );
		}

		//normalize each vertex normal
		for( int i=0; i < mVertexs.size() ; ++i )
		{
			D3DXVECTOR3 Normal;
			mVertexs[i].GetNormal(Normal);
			D3DXVec3Normalize(&Normal, &Normal);
			mVertexs[i].SetNormal(Normal);
		}

		return;
	}


DWORD ObjParser::AddVertex( DWORD hash, VertexTextureNormal& NewVertex)
	{
		bool found = false;
		DWORD index = -1;

		// search in the hashtable for value
		HashEntry* pEntry = mVertexsHT[hash];
		while( pEntry != NULL )
		{
			VertexTextureNormal* pVertex = &mVertexs[pEntry->Index];

			if( memcmp(&NewVertex, pVertex, sizeof(VertexTextureNormal)) == 0 )
			{
				found = true;
				index = pEntry->Index;
			}

			pEntry = pEntry->pNextEntry;
		}

		//in case not found, add it
		if( !found )
		{
			index = mVertexs.size();
			mVertexs.push_back(NewVertex);
			HashEntry* pNewEntry = new HashEntry(index, NULL);

			if( mVertexsHT[hash] == NULL )
				mVertexsHT[hash] = pNewEntry;
			else
			{
				HashEntry* pCurEntry = mVertexsHT[hash];
				while( pCurEntry->pNextEntry != NULL )
					pCurEntry = pCurEntry->pNextEntry;

				pCurEntry->pNextEntry = pNewEntry;
			}
		}

		return index;
	}