#include "pch.h"
#include "Shader.h"
#include "Engine.h"

Shader::Shader() : Object(OBJECT_TYPE::SHADER)
{

}

Shader::~Shader()
{

}

void Shader::CreateGraphicsShader(const wstring& path, ShaderInfo info, const string& vs, const string& ps, const string& gs)
{
	_info = info;

	CreateVertexShader(path, vs, "vs_5_0");
	CreatePixelShader(path, ps, "ps_5_0");

	if (gs.empty() == false)
	{
		CreateGeometryShader(path, gs, "gs_5_0");
	}

	D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

		// World Matrix
		{ "W", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		{ "W", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		{ "W", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		{ "W", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		// World View Matrix
		{ "WV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		{ "WV", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		{ "WV", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 96, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		{ "WV", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 112, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		// World View Projection Matrix
		{ "WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 128, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		{ "WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 144, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		{ "WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 160, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		{ "WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 176, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
	};

	_garaphicsPipelineDesc.InputLayout = { desc, _countof(desc) };
	_garaphicsPipelineDesc.pRootSignature = GRAPHICS_ROOT_SIGNATURE.Get();

	_garaphicsPipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_garaphicsPipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_garaphicsPipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	_garaphicsPipelineDesc.SampleMask = UINT_MAX;
	_garaphicsPipelineDesc.PrimitiveTopologyType = GetTopologyType(info.topology);
	_garaphicsPipelineDesc.NumRenderTargets = 1;
	_garaphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_garaphicsPipelineDesc.SampleDesc.Count = 1;
	_garaphicsPipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	switch (info.shaderType)
	{
		case SHADER_TYPE::DEFERRED:
			_garaphicsPipelineDesc.NumRenderTargets = RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT;
			_garaphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
			_garaphicsPipelineDesc.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
			_garaphicsPipelineDesc.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;

		case SHADER_TYPE::FORWARD:
			_garaphicsPipelineDesc.NumRenderTargets = 1;
			_garaphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;

		case SHADER_TYPE::LIGHTING:
			_garaphicsPipelineDesc.NumRenderTargets = RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT;
			_garaphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			_garaphicsPipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;

		case SHADER_TYPE::PARTICLE:
			_garaphicsPipelineDesc.NumRenderTargets = 1;
			_garaphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
	}

	switch (info.rasterizerType)
	{
		case RASTERIZER_TYPE::CULL_BACK:
			_garaphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			_garaphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
			break;

		case RASTERIZER_TYPE::CULL_FRONT:
			_garaphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			_garaphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
			break;

		case RASTERIZER_TYPE::CULL_NONE:
			_garaphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			_garaphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			break;

		case RASTERIZER_TYPE::WIREFRAME:
			_garaphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
			_garaphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			break;
	}

	switch (info.depthStencilType)
	{
		case DEPTH_STENCIL_TYPE::LESS:
			_garaphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
			_garaphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
			break;

		case DEPTH_STENCIL_TYPE::LESS_EQUAL:
			_garaphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
			_garaphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
			break;

		case DEPTH_STENCIL_TYPE::GREATER:
			_garaphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
			_garaphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
			break;

		case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
			_garaphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
			_garaphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
			break;

		case DEPTH_STENCIL_TYPE::NO_DEPTH_TEST:
			_garaphicsPipelineDesc.DepthStencilState.DepthEnable = FALSE;
			_garaphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
			break;

		case DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE:
			_garaphicsPipelineDesc.DepthStencilState.DepthEnable = FALSE;
			_garaphicsPipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			break;

		case DEPTH_STENCIL_TYPE::LESS_NO_WRITE:
			_garaphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
			_garaphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
			_garaphicsPipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			break;
	}

	D3D12_RENDER_TARGET_BLEND_DESC& rtBlendState = _garaphicsPipelineDesc.BlendState.RenderTarget[0];

	// SrcBlend = Pixel Shader
	// DestBlend = Render Target
	switch (info.blendType)
	{
		case BLEND_TYPE::DEFAULT:
			rtBlendState.BlendEnable = FALSE;
			rtBlendState.LogicOpEnable = FALSE;
			rtBlendState.SrcBlend = D3D12_BLEND_ONE;
			rtBlendState.DestBlend = D3D12_BLEND_ZERO;
			break;

		case BLEND_TYPE::ALPHA_BLEND:
			rtBlendState.BlendEnable = TRUE;
			rtBlendState.LogicOpEnable = FALSE;
			rtBlendState.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			rtBlendState.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // (1 - alpha)
			break;

		case BLEND_TYPE::ONE_TO_ONE_BLEND:
			rtBlendState.BlendEnable = TRUE;
			rtBlendState.LogicOpEnable = FALSE;
			rtBlendState.SrcBlend = D3D12_BLEND_ONE;
			rtBlendState.DestBlend = D3D12_BLEND_ONE;
			break;
	}

	DEVICE->CreateGraphicsPipelineState(&_garaphicsPipelineDesc, IID_PPV_ARGS(&_pipelineState));
}

void Shader::CreateComputeShader(const wstring& path, const string& name, const string& version)
{
	_info.shaderType = SHADER_TYPE::COMPUTE;

	CreateShader(path, name, version, _csBlob, _computePipelineDesc.CS);
	_computePipelineDesc.pRootSignature = COMPUTE_ROOT_SIGNATURE.Get();

	HRESULT hr = DEVICE->CreateComputePipelineState(&_computePipelineDesc, IID_PPV_ARGS(&_pipelineState));
	assert(SUCCEEDED(hr));
}

void Shader::Update()
{
	if (GetShaderType() == SHADER_TYPE::COMPUTE)
	{
		COMPUTE_CMD_LIST->SetPipelineState(_pipelineState.Get());
	}

	else 
	{
		GRAPHICS_CMD_LIST->IASetPrimitiveTopology(_info.topology);
		GRAPHICS_CMD_LIST->SetPipelineState(_pipelineState.Get());
	}
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE Shader::GetTopologyType(D3D_PRIMITIVE_TOPOLOGY topology)
{
	switch (topology)
	{
	case D3D_PRIMITIVE_TOPOLOGY_POINTLIST:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	case D3D_PRIMITIVE_TOPOLOGY_LINELIST:
	case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP:
	case D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	case D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	default:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
	}
}

void Shader::CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	uint32 compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	if (FAILED(::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(), version.c_str(), compileFlag, 0, &blob, &_errBlob)))
	{
		::MessageBoxA(nullptr, "Shader Create Failed !", nullptr, MB_OK);
	}

	shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };
}

void Shader::CreateVertexShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, _vsBlob, _garaphicsPipelineDesc.VS);
}

void Shader::CreatePixelShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, _psBlob, _garaphicsPipelineDesc.PS);
}

void Shader::CreateGeometryShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, _gsBlob, _garaphicsPipelineDesc.GS);
}
