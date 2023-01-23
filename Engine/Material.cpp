#include "pch.h"
#include "Material.h"
#include "Engine.h"

Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{

}

Material::~Material()
{

}

void Material::PushGraphicsData()
{
	// Upload CBV
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushGraphicsData(&_params, sizeof(_params));

	// Upload SRV
	for (size_t i = 0; i < _textures.size(); ++i)
	{
		if (_textures[i] == nullptr)
		{
			continue;
		}

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		GEngine->GetGraphicsDescriptorHeap()->SetSRV(_textures[i]->GetSRVHandle(), reg);
	}

	// pipeline setting
	_shader->Update();
}

void Material::PushComputeData()
{
	// Upload CBV
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushComputeData(&_params, sizeof(_params));

	// Upload SRV
	for (size_t i = 0; i < _textures.size(); ++i)
	{
		if (_textures[i] == nullptr)
		{
			continue;
		}

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		GEngine->GetComputeDescriptorHeap()->SetSRV(_textures[i]->GetSRVHandle(), reg);
	}

	// pipeline setting
	_shader->Update();
}

void Material::Dispatch(uint32 x, uint32 y, uint32 z)
{
	// CBV + SRV + SetPipelineState
	PushComputeData();

	// SetDescriptorHeaps + SetComputeRootDescriptorTable
	GEngine->GetComputeDescriptorHeap()->CommitTable();

	COMPUTE_CMD_LIST->Dispatch(x, y, z);

	GEngine->GetComputeCmdQueue()->FlushComputeCommandQueue();
}

shared_ptr<Material> Material::Clone()
{
	shared_ptr<Material> material = make_shared<Material>();

	material->SetShader(_shader);
	material->_params = _params;
	material->_textures = _textures;

	return material;
}
