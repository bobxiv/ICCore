//BasicEffect technique global variables
//---------------------------------------
float4x4 WorldViewProjection;	//The transform fot the vertex poisition
float4x4 World;					//The transform for the normal
float3 LightDirection;			//The light direction for normal calc
bool UseNormal;					//if true then uses normal calc else just Color
float4 Color;					//if use solid color uses this
//---------------------------------------

//Skinned Mesh technique global variables
//---------------------------------------
float4x4 ViewProjMatrix;
static const int MAX_BONE_MATRICES = 50;
float4x3 WorldBoneMatrixs[MAX_BONE_MATRICES];

texture MeshTexture;              

sampler MeshTextureSampler
{
    Texture = <MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
//---------------------------------------


//BasicEffect technique
//----------------------

struct VSInput
{
	float4 position: POSITION;
	float3 normal: NORMAL;
};

VSInput BasicVertexShader(VSInput Input)
{
	VSInput Output;
	
	Output.position = mul(Input.position, WorldViewProjection);
	Output.normal = mul(float4(Input.normal,1), World).xyz;
	
	Output.normal = normalize(Input.normal);
	
	return Output;
}

float4 BasicPixelShader(VSInput Input): COLOR
{
	float4 color = Color;
	
	if( UseNormal )
		color.xyz = dot(Input.normal,LightDirection)*color.xyz;
	return color;
}


technique BasicEffect
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 BasicVertexShader();
		PixelShader = compile ps_3_0 BasicPixelShader();
	}
}

//Skinned Mesh technique
//----------------------

struct VSSkinnedInput
{
	float4 position   : POSITION;
	float3 normal     : NORMAL;
	float2 texcoord   : TEXCOORD0;
	float4 boneIndex  : BLENDINDICES;
	float4 boneWeight : BLENDWEIGHT;
};

VSSkinnedInput SkinnedVertexShader(VSSkinnedInput Input, uniform int NumInfluencies)
{
	VSSkinnedInput Output = (VSSkinnedInput)0;
	
	float3      Pos = 0.0f;
	float3      Normal = 0.0f;    
	
	float WeightArray[4] = (float[4])Input.boneWeight;
	int IndexArray[4]    = (int[4])  Input.boneIndex;
	
	float LastWeight = 0.0f;
	for(int iBone=0; iBone < (NumInfluencies-1) ; ++iBone)
	{
		LastWeight = LastWeight + WeightArray[iBone];
		
		Pos    += mul(Input.position, WorldBoneMatrixs[IndexArray[iBone]]) * WeightArray[iBone];
		Normal += mul(Input.normal,   WorldBoneMatrixs[IndexArray[iBone]]) * WeightArray[iBone];
	}
	LastWeight = 1.0f - LastWeight;
	
	Pos    += mul(Input.position, WorldBoneMatrixs[IndexArray[NumInfluencies-1]]) * LastWeight;
	Normal += mul(Input.normal,   WorldBoneMatrixs[IndexArray[NumInfluencies-1]]) * LastWeight;
	
	
	Output.position = mul(float4(Pos,1.0f), ViewProjMatrix);
	
	Output.normal = normalize(Normal);
	
	Output.texcoord = Input.texcoord;
	
	return Output;
}

float4 SkinnedPixelShader(VSSkinnedInput Input): COLOR
{
	float4 Color = dot(Input.normal,LightDirection)*tex2D(MeshTextureSampler,Input.texcoord);
	return tex2D(MeshTextureSampler,Input.texcoord);
}

int CurNumInfluencies = 2;
VertexShader vsArray[4] = { compile vs_3_0 SkinnedVertexShader(1), 
                            compile vs_3_0 SkinnedVertexShader(2),
                            compile vs_3_0 SkinnedVertexShader(3),
                            compile vs_3_0 SkinnedVertexShader(4)
                          };

technique SkinnedMesh
{
	pass Pass1
	{
		VertexShader = (vsArray[CurNumInfluencies]);
		PixelShader = compile ps_3_0 SkinnedPixelShader();
	}
}