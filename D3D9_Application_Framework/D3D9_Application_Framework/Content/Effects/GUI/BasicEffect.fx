float4x4 mPV;
float4x4 mW;

float4 modulateColor;
float Opacity = 1.0f;

// Textures
texture Image;

// Sampler
sampler2D TexSampler = sampler_state
{
	Texture = <Image>;
	MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct VSInput 
{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VSOutput 
{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

VSOutput BasicVertexShader(VSInput input)
{
    VSOutput output;
	float4x4 PVW = mul(mPV, mW);
    //output.position = mul(PVW, input.position);
	output.position = mul(input.position, PVW);
    output.texCoord = input.texCoord;
    return output;
}

float4 BasicPixelShader(VSOutput input) : COLOR
{
    // Sample texture
    float4 color = tex2D(TexSampler, input.texCoord);
	color.w *= Opacity;
	color   *= modulateColor;

    return color;
}

technique BasicEffect
{
	pass Pass1
	{
		VertexShader = compile vs_2_0 BasicVertexShader();
		PixelShader = compile ps_2_0 BasicPixelShader();
	}
}

float4 GrayPixelShader(VSOutput input) : COLOR
{
    // Sample texture
    float4 color = tex2D(TexSampler, input.texCoord);
	
	color.rgb = dot(color, float4(0.3, 0.59, 0.11, 0)).xxx; 

    return color;
}

technique GrayEffect
{
	pass Pass1
	{
		VertexShader = compile vs_2_0 BasicVertexShader();
		PixelShader = compile ps_2_0 GrayPixelShader();
	}
}

///////////

struct VSSolidInput 
{
    float4 position : POSITION;
    float4 color    : COLOR;
};

struct VSSolidOutput 
{
    float4 position : POSITION;
    float4 color    : COLOR;
};

VSSolidOutput SolidVertexShader(VSSolidInput input)
{
    VSSolidOutput output;
	float4x4 PVW = mul(mPV, mW);
	output.position = mul(input.position, PVW);
    output.color = input.color;
    return output;
}

float4 SolidPixelShader(VSSolidOutput input) : COLOR
{
	float4 color = input.color * modulateColor; 

	color.a = Opacity;

    return color;
}

technique SolidEffect
{
	pass Pass1
	{
		VertexShader = compile vs_2_0 SolidVertexShader();
		PixelShader = compile ps_2_0 SolidPixelShader();
	}
}