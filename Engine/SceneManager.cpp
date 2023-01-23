#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "Resources.h"
#include "ParticleSystem.h"

#include "TestCameraScript.h"


void SceneManager::Update()
{
	if (_activeScene == nullptr)
	{
		return;
	}

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

//Temp
void SceneManager::Render()
{
	if (_activeScene)
	{
		_activeScene->Render();
	}
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : ���� Scene ����
	// TODO : ���Ͽ��� Scene ���� �ε�

	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findit = _layerIndex.find(name);

	if (findit == _layerIndex.end())
	{
		return 0;
	}

	return findit->second;
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion

#pragma region ComputeShader
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		shared_ptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture(
			L"UAVTexture", DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS
		);

		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->SetShader(shader);
		material->SetInt(0, 1);
		GEngine->GetComputeDescriptorHeap()->SetUAV(texture->GetUAVHandle(), UAV_REGISTER::u0);

		material->Dispatch(1, 1024, 1);
	}
#pragma endregion
	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near = 1, Far = 1000, FOV = 45 
		camera->AddComponent(make_shared<TestCameraScript>());
		camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true);
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near = 1, Far = 1000, 1280 * 720
		camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll();
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false);
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> obj = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(obj);
		}

		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Textures\\ClearSky01.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}

		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion

#pragma region Object
	for(int i = 0; i < 50; i++)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(25.0f, 25.0f, 25.0f));
		obj->GetTransform()->SetLocalPosition(Vec3(-300.0f + i * 10, 0.0f, 500.0f));
	
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
	
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
			material->SetInt(0, 1);
			meshRenderer->SetMaterial(material);

			//material->SetInt(0, 0);
			//meshRenderer->SetMaterial(material->Clone());
		}
	
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region UI_Test
	for (int32 i = 0; i < 6; ++i)
	{
		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
			obj->AddComponent(make_shared<Transform>());
			obj->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
			obj->GetTransform()->SetLocalPosition(Vec3(-350.0f + (i * 120), 250.0f, 500.0f));
	
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
				meshRenderer->SetMesh(mesh);
			}
	
			{
				shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
	
				shared_ptr<Texture> texture;
	
				if (i < 3)
				{
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
				}
	
				else if (i < 5)
				{
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
				}
	
				else
				{
					texture = GET_SINGLE(Resources)->Get<Texture>(L"UAVTexture");
				}
	
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(shader);
				material->SetTexture(0, texture);
				meshRenderer->SetMaterial(material);
			}
	
			obj->AddComponent(meshRenderer);
			scene->AddGameObject(obj);
		}
	}
#pragma endregion

#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		//light->GetTransform()->SetLocalPosition(Vec3(0.f, 150.f, 150.f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, 0, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.0f, 1.0f, 1.0f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));
	
		scene->AddGameObject(light);
	}
#pragma endregion

//#pragma region ParticleSystem
//	{
//		shared_ptr<GameObject> particle = make_shared<GameObject>();
//		particle->AddComponent(make_shared<Transform>());
//		particle->AddComponent(make_shared<ParticleSystem>());
//		particle->SetCheckFrustum(false);
//		particle->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 100.0f));
//		scene->AddGameObject(particle);
//	}
//#pragma endregion

//#pragma region Point Light
//{
//	shared_ptr<GameObject> light = make_shared<GameObject>();
//	light->AddComponent(make_shared<Transform>());
//	light->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 150.f));
//	light->AddComponent(make_shared<Light>());
//	//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
//	light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
//	light->GetLight()->SetDiffuse(Vec3(0.0f, 0.5f, 0.0f));
//	light->GetLight()->SetAmbient(Vec3(0.0f, 0.3f, 0.0f));
//	light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.0f));
//	light->GetLight()->SetLightRange(200.f);
//
//	scene->AddGameObject(light);
//}
//#pragma endregion
//
//#pragma region Spot Light
//{
//	shared_ptr<GameObject> light = make_shared<GameObject>();
//	light->AddComponent(make_shared<Transform>());
//	light->GetTransform()->SetLocalPosition(Vec3(75.f, 0.f, 150.f));
//	light->AddComponent(make_shared<Light>());
//	light->GetLight()->SetLightDirection(Vec3(-1.f, 0, 0));
//	light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
//	light->GetLight()->SetDiffuse(Vec3(0.0f, 0.f, 0.5f));
//	light->GetLight()->SetAmbient(Vec3(0.0f, 0.0f, 0.1f));
//	light->GetLight()->SetSpecular(Vec3(0.0f, 0.0f, 0.1f));
//	light->GetLight()->SetLightRange(200.f);
//	light->GetLight()->SetLightAngle(3.14f / 2);
//
//	scene->AddGameObject(light);
//}
//#pragma endregion

	return scene;
}
