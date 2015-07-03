#pragma once 

#include "..\\..\\stdafx.h"

#pragma region Vertexs Clases

class Vertex
{
public:

	Vertex(): x(0.0f), y(0.0f), z(0.0f), w(1.0f){}

	float x;
	float y;
	float z;
	float w;

	void SetPosition(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	void SetPosition(const D3DXVECTOR3& Vector3)
	{
		x = Vector3.x;
		y = Vector3.y;
		z = Vector3.z;
	}

	void SetPosition(float X, float Y, float Z, float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	void SetPosition(const D3DXVECTOR4& Vector4)
	{
		x = Vector4.x;
		y = Vector4.y;
		z = Vector4.z;
		w = Vector4.w;
	}

	void GetPosition( D3DXVECTOR4& Vector4 )
	{
		Vector4.x = x;
		Vector4.y = y;
		Vector4.z = z;
		Vector4.w = w;
	}

	void GetPosition( D3DXVECTOR3& Vector3 )
	{
		Vector3.x = x;
		Vector3.y = y;
		Vector3.z = z;
	}

	D3DXVECTOR3 GetPosition()
	{
		return D3DXVECTOR3(x,y,z);
	}

	static D3DVERTEXELEMENT9 VertexElement[];
};

class VertexColor:public Vertex
{
public:

	VertexColor(): Color(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)){}

	D3DXCOLOR Color;

	static D3DVERTEXELEMENT9 VertexElement[];
};

class VertexTexture:public Vertex
{
public:

	VertexTexture(): u(0.0f), v(0.0f){}

	float u;
	float v;

	void SetTexture(float U, float V)
	{
		u = U;
		v = V;
	}

	void SetTexture(const D3DXVECTOR2& Vector2)
	{
		u = Vector2.x;
		v = Vector2.y;
	}

	void GetTexture( D3DXVECTOR2& Vector2 )
	{
		Vector2.x = u;
		Vector2.y = v;
	}

	D3DXVECTOR2 GetTexture()
	{
		return D3DXVECTOR2(u,v);
	}

	static D3DVERTEXELEMENT9 VertexElement[];
};

class VertexNormal:public Vertex
{
public:

	VertexNormal(): nx(0.0f), ny(0.0f), nz(0.0f){}

	float nx;
	float ny;
	float nz;

	void SetNormal(float NX, float NY, float NZ)
	{
		nx = NX;
		ny = NY;
		nz = NZ;
	}

	void SetNormal(const D3DXVECTOR3& Vector3)
	{
		nx = Vector3.x;
		ny = Vector3.y;
		nz = Vector3.z;
	}

	void GetNormal( D3DXVECTOR3& Vector3 )
	{
		Vector3.x = nx;
		Vector3.y = ny;
		Vector3.z = nz;
	}

	D3DXVECTOR3 GetNormal()
	{
		return D3DXVECTOR3(nx,ny,nz);
	}

	static D3DVERTEXELEMENT9 VertexElement[];
};

class VertexColorNormal:public VertexColor
{
public:

	VertexColorNormal(): nx(0.0f), ny(0.0f), nz(0.0f){}

	float nx;
	float ny;
	float nz;

	void SetNormal(float NX, float NY, float NZ)
	{
		nx = NX;
		ny = NY;
		nz = NZ;
	}

	void SetNormal(const D3DXVECTOR3& Vector3)
	{
		nx = Vector3.x;
		ny = Vector3.y;
		nz = Vector3.z;
	}

	void GetNormal( D3DXVECTOR3& Vector3 )
	{
		Vector3.x = nx;
		Vector3.y = ny;
		Vector3.z = nz;
	}

	D3DXVECTOR3 GetNormal()
	{
		return D3DXVECTOR3(nx,ny,nz);
	}

	static D3DVERTEXELEMENT9 VertexElement[];
};

class VertexTextureNormal:public VertexTexture
{
public:

	VertexTextureNormal(): nx(0.0f), ny(0.0f), nz(0.0f){}

	float nx;
	float ny;
	float nz;

	void SetNormal(float NX, float NY, float NZ)
	{
		nx = NX;
		ny = NY;
		nz = NZ;
	}

	void SetNormal(const D3DXVECTOR3& Vector3)
	{
		nx = Vector3.x;
		ny = Vector3.y;
		nz = Vector3.z;
	}

	void GetNormal( D3DXVECTOR3& Vector3 )
	{
		Vector3.x = nx;
		Vector3.y = ny;
		Vector3.z = nz;
	}

	D3DXVECTOR3 GetNormal()
	{
		return D3DXVECTOR3(nx,ny,nz);
	}

	static D3DVERTEXELEMENT9 VertexElement[];
};

class VertexTextureNormalSkinned: VertexTexture
{
public:

	VertexTextureNormalSkinned()
	{
		bonesInfluencing[0]=-1;
		bonesInfluencing[1]=-1;
		bonesInfluencing[2]=-1;
		bonesWeights[0]=0.0f;
		bonesWeights[1]=0.0f;
		bonesWeights[2]=0.0f;
	}

	//Indices of Bones influencing this vertex
	int bonesInfluencing[3]; 
	//The n Weight of the n Bone
	float bonesWeights[3];
 
	void SetBonesInfluences(int bone1, int bone2, int bone3)
	{
		bonesInfluencing[0]=bone1;
		bonesInfluencing[1]=bone2;
		bonesInfluencing[2]=bone3;
	}

	void SetBonesWeights(int weights1, int weights2, int weights3)
	{
		bonesWeights[0]=0.0f;
		bonesWeights[1]=0.0f;
		bonesWeights[2]=0.0f;
	}

	static D3DVERTEXELEMENT9 VertexElement[];
};

#pragma endregion