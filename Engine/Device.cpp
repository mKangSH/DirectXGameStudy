#include "pch.h"
#include "Device.h"
#include <stdexcept>

void Device::Init()
{
	UINT dxgiFactoryFlags = 0;

#ifdef _DEBUG
	
	if (SUCCEEDED(::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController))))
	{
		_debugController->EnableDebugLayer();
	}
#endif

	// dxgi는 그래픽 런타임과 독립적일 수 있는 하위 수준 작업 관리
	::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));

	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
}
