#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "Texture.h"
#include "RenderTargetGroup.h"

class Engine
{
public:
	void Init(const WindowInfo& window);
	void Update();
	
public:
	void Render();
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

private:
	void ShowFps();
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);
	void CreateRenderTargetGroups();

public:
	const WindowInfo& GetWindow() { return _window; }

public:
	// Rendering 관련
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCommandQueue() { return _commandQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain; }
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; }
	shared_ptr<TableDescriptorHeap> GetTableDescriptorHeap() { return _tableDescriptorHeap; }

	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }
	shared_ptr<RenderTargetGroup> GetRTGroup(RENDER_TARGET_GROUP_TYPE type) { return _rtGroups[static_cast<uint8>(type)]; }

private:
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	// Rendering 관련
	shared_ptr<Device>					_device;
	shared_ptr<CommandQueue>			_commandQueue;
	shared_ptr<SwapChain>				_swapChain;
	shared_ptr<RootSignature>			_rootSignature;
	shared_ptr<TableDescriptorHeap>		_tableDescriptorHeap;

	vector<shared_ptr<ConstantBuffer>>	_constantBuffers;
	array<shared_ptr<RenderTargetGroup>, RENDER_TARGET_GROUP_COUNT> _rtGroups;
};

