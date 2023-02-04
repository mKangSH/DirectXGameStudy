#pragma once

class SwapChain;
class DescriptorHeap;

// ======================== //
//  GraphicsCommandQueue    //
// ======================== //

class GraphicsCommandQueue
{
public:
	~GraphicsCommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);

	// CPU�� GPU�� ��ٸ��� ��ȿ������ ���
	// Fence�� �̿��Ͽ� �̺�Ʈ�� �߻���Ű�� CPU GPU ��ũ�� ���ߴ� �Լ�
	void WaitSync();

	void RenderBegin();
	void RenderEnd();

	void FlushResourceCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCommandQueue() { return _commandQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCmdList() { return _commandList; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return _resCommandList; }

private:
	ComPtr<ID3D12CommandQueue>			_commandQueue;
	ComPtr<ID3D12CommandAllocator>		_commandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	_commandList;

	ComPtr<ID3D12CommandAllocator>		_resCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	_resCommandList;

	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>				_swapChain;

};

// ======================== //
//   ComputeCommandQueue    //
// ======================== //

class ComputeCommandQueue
{
public:
	~ComputeCommandQueue();

	void Init(ComPtr<ID3D12Device> device);
	void WaitSync();
	void FlushComputeCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetComputeCmdList() { return _cmdList; }

private:
	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;
};