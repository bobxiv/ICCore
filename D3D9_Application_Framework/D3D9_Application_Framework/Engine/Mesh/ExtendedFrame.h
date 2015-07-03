#pragma once

#include "..\\..\\stdafx.h"

class D3DXFRAME_Derived: public D3DXFRAME
{
public:
	D3DXMATRIX CombinedTransformation;

	D3DXFRAME_Derived()
	{
		Name = NULL;
		pMeshContainer = NULL;
		pFrameSibling = pFrameFirstChild = NULL;
		D3DXMatrixIdentity(&TransformationMatrix);
		D3DXMatrixIdentity(&CombinedTransformation);
	}

    ~D3DXFRAME_Derived()
	{ 
		delete [] Name;          Name = NULL;
		delete pFrameSibling;    pFrameSibling = NULL;
		delete pFrameFirstChild; pFrameFirstChild = NULL;
	}


	static D3DXFRAME_Derived* FindFrame(D3DXFRAME_Derived* Node, const char* Name)
	{
		if( Node && Node->Name && Name )//is this one?
			{
			if( !strcmp(Node->Name,Name) )
				return Node;
			}

		if( Node && Node->pFrameSibling )//is a sibling?
			{
			D3DXFRAME_Derived* result = D3DXFRAME_Derived::FindFrame((D3DXFRAME_Derived*)Node->pFrameSibling, Name);
			if( result )
				return result;
			}
		
		if( Node && Node->pFrameFirstChild )//is a children?
			{
			D3DXFRAME_Derived* result = D3DXFRAME_Derived::FindFrame((D3DXFRAME_Derived*)Node->pFrameFirstChild, Name);
			if( result )
				return result;
			}

		return NULL;//not this one and neither related
	}

	static void UpdateHierarchy(D3DXFRAME_Derived* Node, D3DXMATRIX* TransformMat = NULL)
	{
		if( !TransformMat )//by default make the transformation an identity
			{
			TransformMat = new D3DXMATRIX;
			D3DXMatrixIdentity(TransformMat);
			}

		Node->CombinedTransformation = Node->TransformationMatrix*(*TransformMat);

		if( Node->pFrameSibling )//apply to sibling
			D3DXFRAME_Derived::UpdateHierarchy((D3DXFRAME_Derived*)Node->pFrameSibling, TransformMat);
			

		if( Node->pFrameFirstChild )//apply to children
			D3DXFRAME_Derived::UpdateHierarchy((D3DXFRAME_Derived*)Node->pFrameFirstChild, &Node->CombinedTransformation);

		return;
	}


};
