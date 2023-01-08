#pragma once

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);

	// CPU�� GPU�� ��ٸ��� ��ȿ������ ���
	// Fence�� �̿��Ͽ� �̺�Ʈ�� �߻���Ű�� CPU GPU ��ũ�� ���ߴ� �Լ�
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* viewport, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCommandQueue() { return _commandQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return _commandList; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCommandList() { return _resCommandList; }

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

