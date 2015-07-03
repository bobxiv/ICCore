////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					Utility Library				  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <d3d9.h>

//Usuful to create the buffer and load an array of vertices into a
//Vertex Buffer quickly using FVF
//pD3dDev		The device to use
//Buff_output	The buffer to fill with the vertices
//count			The size of the array of vertices to fill
//V				The vertex array to copy into the buffer
//FvF_Flags		The FVF format of the Vertices, or NULL if not FVF
	template<class Vertex>
bool CreateFillBuffer(LPDIRECT3DDEVICE9 pD3dDev, LPDIRECT3DVERTEXBUFFER9 Buff_output, int count, Vertex* V, int FvF_Flags);

//Usuful to load an array of vertices into a Vertex Buffer quickly
//using FVF
//Buff_output	The buffer to fill with the vertices
//count			The size of the array of vertices to fill
//V				The vertex array to copy into the buffer
	template<class Vertex>
bool FillBuffer(LPDIRECT3DVERTEXBUFFER9 Buff_output, int count, Vertex* V);

//Usuful to create the index buffer and load an array of indices into a
//Index Buffer quickly 
//pD3dDev		The device to use
//Buff_output	The buffer to fill with the vertices
//count			The size of the array of vertices to fill
//V				The vertex array to copy into the buffer
//FvF_Flags		The FVF format of the Vertices, or NULL if not FVF
	template<class Vertex>
bool CreateFillIndexBuffer(LPDIRECT3DDEVICE9 pD3dDev, LPDIRECT3DVERTEXBUFFER9 Buff_output, int count, Vertex* V, D3DFORMAT Index_FMT);

//Usuful to load an array of vertices into a Index Buffer quickly
//Buff_output	The buffer to fill with the vertices
//count			The size of the array of vertices to fill
//V				The vertex array to copy into the buffer
	template<class Vertex>
bool FillIndexBuffer(LPDIRECT3DVERTEXBUFFER9 Buff_output, int count, Vertex* V);



	template<class Vertex>
bool CreateFillBuffer(LPDIRECT3DDEVICE9 pD3dDev, LPDIRECT3DVERTEXBUFFER9* Buff_output, int count, Vertex* V, int FvF_Flags)
	{
	if( FAILED(pD3dDev->CreateVertexBuffer( count*sizeof(Vertex), D3DUSAGE_WRITEONLY, FvF_Flags,
	D3DPOOL_DEFAULT, Buff_output, NULL)) )
		return false;

	LPVOID Data;
	if( FAILED((*Buff_output)->Lock(0, count*sizeof(Vertex), &Data, NULL)) )
		return false;

	memcpy(Data, V, count*sizeof(Vertex));

	if( FAILED((*Buff_output)->Unlock()) )
		return false;

	return true;
	}

	template<class Vertex>
bool FillBuffer(LPDIRECT3DVERTEXBUFFER9* Buff_output, int count, Vertex* V)
	{
	LPVOID Data;
	if( FAILED(*Buff_output->Lock(0, count*sizeof(Vertex), &Data, NULL)) )
		return false;

	memcpy(Data, V, count*sizeof(Vertex));

	if( FAILED(*Buff_output->Unlock()) )
		return false;

	return true;
	}

	template<class Index>
bool CreateFillIndexBuffer(LPDIRECT3DDEVICE9 pD3dDev, LPDIRECT3DINDEXBUFFER9* Buff_output, int count, Index* I, D3DFORMAT Index_FMT)
	{
	if( FAILED(pD3dDev->CreateIndexBuffer( count*sizeof(Index), D3DUSAGE_WRITEONLY, Index_FMT,
	D3DPOOL_DEFAULT, Buff_output, NULL)) )
		return false;

	LPVOID Data;
	if( FAILED((*Buff_output)->Lock(0, count*sizeof(Index), &Data, NULL)) )
		return false;

	memcpy(Data, I, count*sizeof(Index));

	if( FAILED((*Buff_output)->Unlock()) )
		return false;

	return true;
	}

	template<class Index>
bool FillIndexBuffer(LPDIRECT3DINDEXBUFFER9* Buff_output, int count, Index* I)
	{
	LPVOID Data;
	if( FAILED(*Buff_output->Lock(0, count*sizeof(Index), &Data, NULL)) )
		return false;

	memcpy(Data, I, count*sizeof(Index));

	if( FAILED(*Buff_output->Unlock()) )
		return false;

	return true;
	}