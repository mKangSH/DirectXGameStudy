#ifndef _LIGHTING_FX_
#define _LIGHTING_FX_

#include "params.fx"
#include "utils.fx"

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

struct PS_OUT
{
	float4 diffuse : SV_Target0;
	float4 specular : SV_Target1;
};

// [Directional Light]
// g_int_0 : Light Index
// g_tex_0 : Position RT
// g_tex_1 : Normal RT
// Mesh : Rectangle 

VS_OUT VS_DirLight(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	// (Resources) Rectangle
	// width : 0.5, height : 0.5f
	// 전체 화면을 표현하기 위해 강제 스케일 업
	output.pos = float4(input.pos * 2.0f, 1.0f);
	output.uv = input.uv;

	return output;
}

PS_OUT PS_DirLight(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;

	float3 viewPos = g_tex_0.Sample(g_sam_0, input.uv).xyz;
	
	if(viewPos.z <= 0.0f)
	{
		// exit(-1) 과 비슷
		clip(-1);
	}

	float3 viewNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;

	LightComponents model = CalculateLightColor(g_int_0, viewNormal, viewPos);
	output.diffuse = model.diffuse * model.ambient;
	output.specular = model.specular;

	return output;
}

// [Point Light]
// g_int_0 : Light Index
// g_tex_0 : Position RT
// g_tex_1 : Normal RT
// g_vec2_0 : RenderTarget Resolution
// Mesh : Sphere

VS_OUT VS_PointLight(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	// (Resources) Sphere
	output.pos = mul(float4(input.pos, 1.0f), g_matWVP);
	output.uv = input.uv;

	return output;
}

PS_OUT PS_PointLight(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;

	// input.pos = SV_Position = Screen 좌표
	float2 uv = float2(input.pos.x / g_vec2_0.x, input.pos.y / g_vec2_0.y);
	float3 viewPos = g_tex_0.Sample(g_sam_0, input.uv).xyz;

	if (viewPos.z <= 0.0f)
	{
		// exit(-1) 과 비슷
		clip(-1);
	}

	int lightIndex = g_int_0;
	float3 viewLightPos = mul(float4(g_lights[lightIndex].position.xyz, 1.0f), g_matView).xyz;
	float distance = length(viewPos - viewLightPos);
	if (distance > g_lights[lightIndex].range)
	{
		clip(-1);
	}

	float3 viewNormal = g_tex_1.Sample(g_sam_0, uv).xyz;

	LightComponents model = CalculateLightColor(g_int_0, viewNormal, viewPos);

	output.diffuse = model.diffuse * model.ambient;
	output.specular = model.specular;

	return output;
}

// [Final Assembler]
// g_tex_0 : Diffuse Color Target
// g_tex_1 : Diffuse Light Target
// g_tex_2 : Specular Light Target
// Mesh : Rectangle

VS_OUT VS_Final(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	// (Resources) Rectangle
	output.pos = float4(input.pos * 2.0f, 1.0f);
	output.uv = input.uv;

	return output;
}

float4 PS_Final(VS_OUT input) : SV_Target
{
	float4 output = (float4)0;

	float4 lightPower = g_tex_1.Sample(g_sam_0, input.uv);
	if (lightPower.x == 0.0f && lightPower.y == 0.0f && lightPower.z == 0.0f)
	{
		clip(-1);
	}
	
	float4 color = g_tex_0.Sample(g_sam_0, input.uv);
	float4 specular = g_tex_2.Sample(g_sam_0, input.uv);

	output = (color * lightPower) + specular;

	return output;
}
#endif