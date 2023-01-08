#ifndef _SHADER_HLSL_
#define _SHADER_HLSL_

#include "params.hlsli"
#include "utils.hlsli"

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
	float3 viewPos : POSITION;
	float3 viewNormal : NORMAL;
	float3 viewTangent : TANGENT;
	float3 viewBinormal : BINORMAL;
};

VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.0f), g_matWVP);
	output.uv = input.uv;

	output.viewPos = mul(float4(input.pos, 1.0f), g_matWV).xyz;
	output.viewNormal = normalize(mul(float4(input.normal, 0.0f), g_matWV).xyz);
	output.viewTangent = normalize(mul(float4(input.tangent, 0.0f), g_matWV).xyz);
	output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));

	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (g_tex_on_0)
	{
		color = g_tex_0.Sample(g_sam_0, input.uv);
	}

	float3 viewNormal = input.viewNormal;

	if (g_tex_on_1)
	{
		float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
		tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.0f;
		float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
		viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
	}

	// float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	LightComponents lightModel = (LightComponents)0.0f;

	for (int i = 0; i < g_lightCount; ++i)
	{
		LightComponents model = CalculateLightColor(i, viewNormal, input.viewPos);
		lightModel.diffuse += model.diffuse;
		lightModel.ambient += model.ambient;
		lightModel.specular += model.specular;
	}

	color.xyz = (lightModel.diffuse.xyz * color.xyz) + 
		lightModel.ambient.xyz * color.xyz + lightModel.specular.xyz;

	return color;
}

#endif