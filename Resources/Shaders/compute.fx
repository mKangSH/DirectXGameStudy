#ifndef _COMPUTE_FX_
#define _COMPUTE_FX_

#include "params.fx"

RWTexture2D<float4> g_rwtex_0 : register(u0);

// 쓰레드 그룹 당 쓰레드 개수
// 1024 x 1 x 1 => 1024
[numthreads(1024, 1, 1)]
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
	if (threadIndex.y % 2 == 0)
	{
		g_rwtex_0[threadIndex.xy] = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	else
	{
		g_rwtex_0[threadIndex.xy] = float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
}

#endif