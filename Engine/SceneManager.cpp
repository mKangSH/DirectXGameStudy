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
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

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

	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Sphere
//{
//	shared_ptr<GameObject> sphere = make_shared<GameObject>();
//	sphere->AddComponent(make_shared<Transform>());
//	sphere->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
//	sphere->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 150.0f));
//
//	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//	{
//		shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
//		meshRenderer->SetMesh(sphereMesh);
//	}
//
//	{
//		shared_ptr<Shader> shader = make_shared<Shader>();
//		shared_ptr<Texture> texture = make_shared<Texture>();
//		shared_ptr<Texture> texture2 = make_shared<Texture>();
//		shader->Init(L"..\\Resources\\Shaders\\shaderD.fx");
//		texture->Init(L"..\\Resources\\Textures\\BlueIce.jpg");
//		texture2->Init(L"..\\Resources\\Textures\\BlueIce_Normal.jpg");
//		shared_ptr<Material> material = make_shared<Material>();
//		material->SetShader(shader);
//		material->SetTexture(0, texture);
//		material->SetTexture(1, texture2);
//		meshRenderer->SetMaterial(material);
//	}
//
//	sphere->AddComponent(meshRenderer);
//	scene->AddGameObject(sphere);
//}
#pragma endregion

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
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}

		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Textures\\Sky01.jpg");
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
{
	shared_ptr<GameObject> obj = make_shared<GameObject>();
	obj->AddComponent(make_shared<Transform>());
	obj->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
	obj->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 150.0f));

	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
		meshRenderer->SetMesh(sphereMesh);
	}

	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Metal_Panel", L"..\\Resources\\Textures\\Metal_Panel.jpg");
		shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Metal_Panel_Normal", L"..\\Resources\\Textures\\Metal_Panel_Normal.jpg");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		material->SetTexture(1, texture2);
		meshRenderer->SetMaterial(material);
	}

	obj->AddComponent(meshRenderer);
	scene->AddGameObject(obj);
}
#pragma endregion

#pragma region UI_Test
for (int32 i = 0; i < 5; ++i)
{
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
		sphere->GetTransform()->SetLocalPosition(Vec3(-350.0f + (i * 160), 250.0f, 500.0f));

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

			else
			{
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
			}

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}

		sphere->AddComponent(meshRenderer);
		scene->AddGameObject(sphere);
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
	light->GetLight()->SetDiffuse(Vec3(1.f, 0.f, 0.f));
	light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
	light->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));

	scene->AddGameObject(light);
}
#pragma endregion

#pragma region Point Light
{
	shared_ptr<GameObject> light = make_shared<GameObject>();
	light->AddComponent(make_shared<Transform>());
	light->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 150.f));
	light->AddComponent(make_shared<Light>());
	//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
	light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
	light->GetLight()->SetDiffuse(Vec3(0.0f, 0.5f, 0.0f));
	light->GetLight()->SetAmbient(Vec3(0.0f, 0.3f, 0.0f));
	light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.0f));
	light->GetLight()->SetLightRange(200.f);

	scene->AddGameObject(light);
}
#pragma endregion

#pragma region Spot Light
{
	shared_ptr<GameObject> light = make_shared<GameObject>();
	light->AddComponent(make_shared<Transform>());
	light->GetTransform()->SetLocalPosition(Vec3(75.f, 0.f, 150.f));
	light->AddComponent(make_shared<Light>());
	light->GetLight()->SetLightDirection(Vec3(-1.f, 0, 0));
	light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
	light->GetLight()->SetDiffuse(Vec3(0.0f, 0.f, 0.5f));
	light->GetLight()->SetAmbient(Vec3(0.0f, 0.0f, 0.1f));
	light->GetLight()->SetSpecular(Vec3(0.0f, 0.0f, 0.1f));
	light->GetLight()->SetLightRange(200.f);
	light->GetLight()->SetLightAngle(3.14f / 2);

	scene->AddGameObject(light);
}
#pragma endregion

	return scene;
}
