#ifndef _UTILS_HLSLI_
#define _UTILS_HLSLI_

#include "params.hlsli"

LightComponents CalculateLightColor(int lightIndex, float3 viewNormal, float3 viewPos)
{
    LightComponents lightModel = (LightComponents)0.f;

    float3 viewLightDir = (float3)0.f;

    float diffuseRatio = 0.f;
    float specularRatio = 0.f;
    float distanceRatio = 1.f;

    if (g_lights[lightIndex].lightType == 0)
    {
        // Directional Light
        viewLightDir = normalize(mul(float4(g_lights[lightIndex].direction.xyz, 0.f), g_matView).xyz);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
    }

    else if (g_lights[lightIndex].lightType == 1)
    {
        // Point Light
        float3 viewLightPos = mul(float4(g_lights[lightIndex].position.xyz, 1.f), g_matView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

        float dist = distance(viewPos, viewLightPos);
        if (g_lights[lightIndex].range == 0.f)
        {
            distanceRatio = 0.f;
        }

        else
        {
            distanceRatio = saturate(1.f - pow(dist / g_lights[lightIndex].range, 2));
        }
    }

    else
    {
        // Spot Light
        float3 viewLightPos = mul(float4(g_lights[lightIndex].position.xyz, 1.f), g_matView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

        if (g_lights[lightIndex].range == 0.f)
        {
            distanceRatio = 0.f;
        }

        else
        {
            float halfAngle = g_lights[lightIndex].angle / 2;

            float3 viewLightVec = viewPos - viewLightPos;
            float3 viewCenterLightDir = normalize(mul(float4(g_lights[lightIndex].direction.xyz, 0.f), g_matView).xyz);

            float centerDist = dot(viewLightVec, viewCenterLightDir);
            distanceRatio = saturate(1.f - centerDist / g_lights[lightIndex].range);

            float lightAngle = acos(dot(viewLightDir, viewCenterLightDir));

            if (centerDist < 0.f || centerDist > g_lights[lightIndex].range) // �ִ� �Ÿ��� �������
            {
                distanceRatio = 0.f;
            }

            else if (lightAngle > halfAngle) // �ִ� �þ߰��� �������
            {
                distanceRatio = 0.f;
            }

            else // �Ÿ��� ���� ������ ���⸦ ����
            {
                distanceRatio = saturate(1.f - pow(centerDist / g_lights[lightIndex].range, 2));
            }
        }
    }

    float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, viewNormal)) * viewNormal));
    float3 eyeDir = normalize(viewPos);
    specularRatio = saturate(dot(-eyeDir, reflectionDir));
    specularRatio = pow(specularRatio, 4);

    lightModel.diffuse = g_lights[lightIndex].model.diffuse * diffuseRatio * distanceRatio;
    lightModel.ambient = g_lights[lightIndex].model.ambient * distanceRatio;
    lightModel.specular = g_lights[lightIndex].model.specular * specularRatio * distanceRatio;

    return lightModel;
}

#endif