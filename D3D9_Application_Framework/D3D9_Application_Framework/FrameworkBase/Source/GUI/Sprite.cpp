#pragma once

#include "stdafx.h"
#include "..\\..\\Header\\GUI\\Sprite.h"

#include "..\\..\\..\\MyApp.h"


namespace GUIFramework
{

	extern GUIMaterialsLibrary MaterialsLibrary;

	#pragma region Constructors & Destructors
	
	Sprite::Sprite(): pTexture(NULL), Width(0), Height(0), RotationDegree(0), Opacity(1), 
		fadingOut(false), fadingIn(false), flipX(false), flipY(false)
	{
	D3DXMatrixIdentity(&Translation);
	D3DXMatrixIdentity(&Sizing);
	D3DXMatrixIdentity(&Rotation);
	ModulateColor[0]=1.0f;//r
	ModulateColor[1]=1.0f;//g
	ModulateColor[2]=1.0f;//b

	Position.X=0;Position.Y=0;

	pDevice = MyApp::GetSingleton()->GetD3DDevice();
	Material = ::GUIFramework::MaterialsLibrary.BasicEffect;
	pspriteQuad = new ::ProPrimitive::Quad(pDevice);
	}

	Sprite::Sprite(LPDIRECT3DTEXTURE9& pTex): pTexture(pTex), Opacity(1),
		fadingOut(false), fadingIn(false), flipX(false), flipY(false)
	{
		D3DXMatrixIdentity(&Translation);
		D3DXMatrixIdentity(&Sizing);
		D3DXMatrixIdentity(&Rotation);
		ModulateColor[0]=1.0f;//r
		ModulateColor[1]=1.0f;//g
		ModulateColor[2]=1.0f;//b

		pTexture->GetLevelDesc(0, &TextureDesc);
		Width = TextureDesc.Width;
		Height = TextureDesc.Height;

		Position.X=0;Position.Y=0;

		pDevice = MyApp::GetSingleton()->GetD3DDevice();
		Material = ::GUIFramework::MaterialsLibrary.BasicEffect;
		pspriteQuad = new ::ProPrimitive::Quad(pDevice);
	}

	Sprite::Sprite(LPDIRECT3DTEXTURE9& pTex, AppPoint P): pTexture(pTex), Opacity(1),
		fadingOut(false), fadingIn(false), flipX(false), flipY(false)
	{
		D3DXMatrixIdentity(&Translation);
		D3DXMatrixIdentity(&Sizing);
		D3DXMatrixIdentity(&Rotation);
		ModulateColor[0]=1.0f;//r
		ModulateColor[1]=1.0f;//g
		ModulateColor[2]=1.0f;//b

		pTexture->GetLevelDesc(0, &TextureDesc);
		Width = TextureDesc.Width;
		Height = TextureDesc.Height;

		this->SetPosition(P);

		pDevice = MyApp::GetSingleton()->GetD3DDevice();
		Material = ::GUIFramework::MaterialsLibrary.BasicEffect;
		pspriteQuad = new ::ProPrimitive::Quad(pDevice);
	}

	Sprite::Sprite(AppPoint P, int W, int H): 
		pTexture(NULL), Position(P), Width(W), Height(H), Opacity(1), 
		fadingOut(false), fadingIn(false), flipX(false), flipY(false)
	{
		this->SetPosition(P);
		this->SetWidth(W);
		this->SetHeight(H);
		ModulateColor[0]=1.0f;//r
		ModulateColor[1]=1.0f;//g
		ModulateColor[2]=1.0f;//b

		pDevice = MyApp::GetSingleton()->GetD3DDevice();
		Material = ::GUIFramework::MaterialsLibrary.BasicEffect;
		pspriteQuad = new ::ProPrimitive::Quad(pDevice);
	}

	Sprite::Sprite(AppPoint P, int W, int H, int Degrees): 
		pTexture(NULL), Position(P), Width(W), Height(H), RotationDegree(Degrees), Opacity(1),
		fadingOut(false), fadingIn(false), flipX(false), flipY(false)
	{
		this->SetPosition(P);
		this->SetWidth(W);
		this->SetHeight(H);
		this->SetRotation(Degrees);
		ModulateColor[0]=1.0f;//r
		ModulateColor[1]=1.0f;//g
		ModulateColor[2]=1.0f;//b

		pDevice = MyApp::GetSingleton()->GetD3DDevice();
		Material = ::GUIFramework::MaterialsLibrary.BasicEffect;
		pspriteQuad = new ::ProPrimitive::Quad(pDevice);
	}

	Sprite::Sprite(LPDIRECT3DTEXTURE9& pTex, AppPoint P, int W, int H):
		pTexture(pTex), Position(P), Width(W), Height(H), Opacity(1), 
		fadingOut(false), fadingIn(false), flipX(false), flipY(false)
	{
		this->SetPosition(P);
		this->SetWidth(W);
		this->SetHeight(H);
		ModulateColor[0]=1.0f;//r
		ModulateColor[1]=1.0f;//g
		ModulateColor[2]=1.0f;//b

		pDevice = MyApp::GetSingleton()->GetD3DDevice();
		Material = ::GUIFramework::MaterialsLibrary.BasicEffect;
		pspriteQuad = new ::ProPrimitive::Quad(pDevice);
	}

	Sprite::~Sprite()
	{
		delete pspriteQuad;
	}

	#pragma endregion

	#pragma region Public Methods

	void Sprite::Draw(const double elapsedTime)
	{
		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);
		D3DXMATRIX World;
		D3DXMatrixMultiply(&World, &Sizing, &Rotation );
		D3DXMatrixMultiply(&World, &World, &Translation );
		
		Material.pEffect->SetFloat( "Opacity", Opacity);
		float ModulateAndFade[4];
		memcpy(ModulateAndFade, ModulateColor, sizeof(float)*3);
		ModulateAndFade[3]=1;

		if( fadingOut )
		{
			FadeTime -= elapsedTime;
			if( FadeTime < 0 )
			{
				fadingOut = false;
				FadeTime = 0;
				Opacity = 0.0f;
			}

			ModulateAndFade[3] = FadeTime/FadePeriod;
			
		}else
			if( fadingIn )
			{
				FadeTime += elapsedTime;
				if( FadeTime > FadePeriod )
				{
					fadingIn = false;
					FadeTime = FadePeriod;
					Opacity = 1.0;
				}
				ModulateAndFade[3] = FadeTime/FadePeriod;
				Opacity = 1.0;
			}

		Material.pEffect->SetFloatArray( "modulateColor", ModulateAndFade, 4);

		pspriteQuad->Draw(pDevice, I, World, pTexture);

		Material.pEffect->SetFloat( "Opacity", 1.0f);

		return;
	}

	bool Sprite::FlipX()
	{
		ProPrimitive::Quad::QuadVertex::UV aux;
		aux = pspriteQuad->Vertexs[0].Texture;
		pspriteQuad->Vertexs[0].Texture = pspriteQuad->Vertexs[1].Texture;
		pspriteQuad->Vertexs[1].Texture = aux;

		aux = pspriteQuad->Vertexs[3].Texture;
		pspriteQuad->Vertexs[3].Texture = pspriteQuad->Vertexs[2].Texture;
		pspriteQuad->Vertexs[2].Texture = aux;

		pspriteQuad->ReCreateVertexBuffer(this->pDevice);
	
		bool auxRes = flipX;
		flipX != flipX;

		return auxRes;
	}

	bool Sprite::FlipY()
	{
		ProPrimitive::Quad::QuadVertex::UV aux;
		aux = pspriteQuad->Vertexs[0].Texture;
		pspriteQuad->Vertexs[0].Texture = pspriteQuad->Vertexs[3].Texture;
		pspriteQuad->Vertexs[3].Texture = aux;

		aux = pspriteQuad->Vertexs[1].Texture;
		pspriteQuad->Vertexs[1].Texture = pspriteQuad->Vertexs[2].Texture;
		pspriteQuad->Vertexs[2].Texture = aux;

		pspriteQuad->ReCreateVertexBuffer(this->pDevice);

		bool auxRes = flipY;
		flipY != flipY;

		return auxRes;
	}

	void Sprite::FadeOut(float period)
	{
		FadePeriod = period;
		FadeTime = period;
		fadingOut = true;
		fadingIn = false; 
		return;
	}

	void Sprite::FadeIn(float period)
	{
		FadePeriod = period;
		FadeTime = 0;
		fadingIn = true;
		fadingOut = false;
		return;
	}

	#pragma region Setters

	AppPoint Sprite::SetPosition(AppPoint& P)
	{
		AppPoint aux = Position;
		Position = P;

		D3DXVECTOR2 vP(Position.X, -Position.Y);
		int W;
		int H;
		MyApp::GetSingleton()->GetBackBufferDimension(W,H);
		vP.x /= (float)W;//if vP.x == W then moves 1, half projected box
		vP.y /= (float)H;//if vP.x ==  0 then moves 0, nothing in the projected box
		//Scale vP x and y to be from 0 to 2 for the size of the projected box
		vP.x *=2.0f; vP.y *=2.0f;

		D3DXVECTOR2 Origin(-0.5,0.5);//Vector to set the origin in (-1,1)
									 //Consider that quad vertex 1 is in (-0.5,0.5)
		D3DXVECTOR2 delta(Sizing.m[0][0]-1, 1-Sizing.m[1][1]);
		delta /= 2;//delta fix the sizing movement, depending of the size ts is the fix
				
		//Origin+delta sets point in the correct ref system, and delta fix the size matrix problem
		D3DXMatrixTranslation(&Translation,Origin.x+delta.x+vP.x,Origin.y+delta.y+vP.y,0);

		return aux;
	}

	int Sprite::SetWidth(int W)
	{
		int aux = Width;
		Width = W;

		int Wid;
		int Hei;//we can't use GUI::GetFrameDimensions because button constructor uses this and 
				//using GUI from a Button memeber of GUI would create a recursive singleton call
		MyApp::GetSingleton()->GetBackBufferDimension(Wid,Hei);

		this->Sizing.m[0][0] = (Width /(float)Wid)*2;//the *2 is because the normal cube of
													 //DirectX is from -1 to 1 so dim 2

		float deltax= Sizing.m[0][0]-1;
		Translation.m[3][0] += deltax;//fix x displacement

		return aux;
	}

	int Sprite::SetHeight(int H)
	{
		int aux = Height;
		Height = H;

		int Wid;
		int Hei;//we can't use GUI::GetFrameDimensions because button constructor uses this and 
				//using GUI from a Button memeber of GUI would create a recursive singleton call
		MyApp::GetSingleton()->GetBackBufferDimension(Wid,Hei);
		this->Sizing.m[1][1] = (Height /(float)Hei)*2;//the *2 is because the normal cube of
													 //DirectX is from -1 to 1 so dim 2

		float deltay= 1-Sizing.m[1][1];
		Translation.m[3][1] += deltax;//fix y displacement
		
		return aux;
	}

	int Sprite::SetRotation(int Degree)
	{
		Degree %= 360;
		int aux = RotationDegree;
		RotationDegree = Degree;

		float radians = (Degree*2*D3DX_PI)/360.0f;
		D3DXMatrixRotationZ( &Rotation, radians);

		return aux;
	}

	LPDIRECT3DTEXTURE9 Sprite::SetTexture(LPDIRECT3DTEXTURE9 pTex)
	{
		LPDIRECT3DTEXTURE9 aux = pTexture;
		pTexture = pTex;

		pTexture->GetLevelDesc(0, &TextureDesc);
		Width = TextureDesc.Width;
		Height = TextureDesc.Height;

		return aux;
	}

	AppRectangle Sprite::SetRenderRectangle(AppRectangle& R)
	{
		AppRectangle aux = RenderArea;

			//if Rect don't have area it is set to the whole texture
		if( R.P1 == R.P2 )
		{
			pspriteQuad->Vertexs[0].Texture.u = 0.0f;
			pspriteQuad->Vertexs[0].Texture.v = 0.0f;

			pspriteQuad->Vertexs[1].Texture.u = 1.0f;
			pspriteQuad->Vertexs[1].Texture.v = 0.0f;

			pspriteQuad->Vertexs[2].Texture.u = 1.0f;
			pspriteQuad->Vertexs[2].Texture.v = 1.0f;

			pspriteQuad->Vertexs[3].Texture.u = 0.0f;
			pspriteQuad->Vertexs[3].Texture.v = 1.0f;

		}else
		{
				//only change the area if R is a valid area
			if( R.P1.X >= 0 && R.P1.X <= TextureDesc.Width 
				&& R.P1.Y >= 0 && R.P1.Y <= TextureDesc.Height )
			{
				if( R.P2.X > R.P1.X && R.P2.X <= TextureDesc.Width 
					&& R.P2.Y > R.P1.Y && R.P2.Y <= TextureDesc.Height )
				{
					RenderArea = R;

					pspriteQuad->Vertexs[0].Texture.u = R.P1.X / (float)TextureDesc.Width;
					pspriteQuad->Vertexs[0].Texture.v = R.P1.Y / (float)TextureDesc.Height;

					pspriteQuad->Vertexs[1].Texture.u = R.P2.X / (float)TextureDesc.Width;
					pspriteQuad->Vertexs[1].Texture.v = R.P1.Y / (float)TextureDesc.Height;

					pspriteQuad->Vertexs[2].Texture.u = R.P2.X / (float)TextureDesc.Width;
					pspriteQuad->Vertexs[2].Texture.v = R.P2.Y / (float)TextureDesc.Height;

					pspriteQuad->Vertexs[3].Texture.u = R.P1.X / (float)TextureDesc.Width;
					pspriteQuad->Vertexs[3].Texture.v = R.P2.Y / (float)TextureDesc.Height;

					pspriteQuad->ReCreateVertexBuffer(pDevice);
				}
			}

		}

		return aux;
	}

	GUIMaterial Sprite::SetMaterial(GUIMaterial& M)
	{
		GUIMaterial aux = Material;
		Material = M;

		return aux;
	}

	#pragma endregion

	#pragma region Getter	

	AppPoint Sprite::GetPosition() const
	{
		return Position;
	}

	int Sprite::GetWidth() const
	{
		return Width;
	}

	int Sprite::GetHeight() const
	{
		return Height;
	}

	int Sprite::GetRotation() const
	{
		return RotationDegree;
	}

	LPDIRECT3DTEXTURE9 Sprite::GetTexture() const
	{
		return pTexture;
	}

	AppRectangle Sprite::GetRenderRectangle() const
	{
		return RenderArea;
	}

	GUIMaterial Sprite::GetMaterial() const
	{
		return Material;
	}

	#pragma endregion

	#pragma endregion

}