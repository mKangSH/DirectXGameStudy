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

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();
#pragma region TestObject
/*
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();

	vector<Vertex> vec(4);
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
	vec[0].uv = Vec2(0.0f, 0.0f);

	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec[1].uv = Vec2(1.0f, 0.0f);

	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec[2].uv = Vec2(1.0f, 1.0f);

	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec[3].uv = Vec2(0.0f, 1.0f);

	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}

	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}

	gameObject->AddComponent(make_shared<Transform>()); // Transform
	shared_ptr<Transform> transform = gameObject->GetTransform();
	transform->SetLocalPosition(Vec3(0.0f, 100.0f, 200.0f));
	transform->SetLocalScale(Vec3(100.0f, 100.0f, 1.0f));

	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(vec, indexVec);
		meshRenderer->SetMesh(mesh);
	}

	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Texture> texture = make_shared<Texture>();
		shader->Init(L"..\\Resources\\Shaders\\shaderD.hlsl");
		texture->Init(L"..\\Resources\\Textures\\Background.png");

		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetFloat(0, 0.3f);
		material->SetFloat(1, 0.4f);
		material->SetFloat(2, 0.3f);
		material->SetTexture(0, texture);

		meshRenderer->SetMaterial(material);
	}

	gameObject->AddComponent(meshRenderer);
	scene->AddGameObject(gameObject);
*/
#pragma endregion

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
//		shader->Init(L"..\\Resources\\Shaders\\shaderD.hlsl");
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
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shader->Init(L"..\\Resources\\Shaders\\skybox.hlsl",
				{ RASTERIZER_TYPE::CULL_FRONT, DEPTH_STENCIL_TYPE::LESS_EQUAL });
			texture->Init(L"..\\Resources\\Textures\\Sky01.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}

		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion

#pragma region Cube
{
	shared_ptr<GameObject> cube = make_shared<GameObject>();
	cube->AddComponent(make_shared<Transform>());
	cube->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
	cube->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 150.0f));

	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
		meshRenderer->SetMesh(cubeMesh);
	}

	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Texture> texture = make_shared<Texture>();
		shared_ptr<Texture> texture2 = make_shared<Texture>();
		shader->Init(L"..\\Resources\\Shaders\\shaderD.hlsl");
		texture->Init(L"..\\Resources\\Textures\\Background.png");
		texture2->Init(L"..\\Resources\\Textures\\BlueIce_Normal.jpg");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		material->SetTexture(1, texture2);
		meshRenderer->SetMaterial(material);
	}

	cube->AddComponent(meshRenderer);
	scene->AddGameObject(cube);
}
#pragma endregion

#pragma region Camera
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->AddComponent(make_shared<Transform>());
	camera->AddComponent(make_shared<Camera>()); // Near = 1, Far = 1000, FOV = 45 
	camera->AddComponent(make_shared<TestCameraScript>());
	camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
	scene->AddGameObject(camera);
#pragma endregion

#pragma region Green Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		//light->GetTransform()->SetLocalPosition(Vec3(0.f, 150.f, 150.f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(1.0f, 0.0f, 1.0f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		scene->AddGameObject(light);
	}

#pragma endregion

#pragma region Red Point Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(150.f, 150.f, 200.f));
		light->AddComponent(make_shared<Light>());
		//light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.6f, 0.1f, 0.1f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.f, 0.f));
		light->GetLight()->SetSpecular(Vec3(0.3f, 0.1f, 0.1f));
		light->GetLight()->SetLightRange(10000.f);
		//light->GetLight()->SetLightAngle(XM_PI / 4);
		scene->AddGameObject(light);
	}
#pragma endregion

#pragma region Blue Spot Light
	//{
	//	shared_ptr<GameObject> light = make_shared<GameObject>();
	//	light->AddComponent(make_shared<Transform>());
	//	light->GetTransform()->SetLocalPosition(Vec3(-150.f, 0.f, 150.f));
	//	light->AddComponent(make_shared<Light>());
	//	light->GetLight()->SetLightDirection(Vec3(1.f, 0.f, 0.f));
	//	light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
	//	light->GetLight()->SetDiffuse(Vec3(0.f, 0.1f, 0.3f));
	//	//light->GetLight()->SetAmbient(Vec3(0.f, 0.f, 0.1f));
	//	light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
	//	light->GetLight()->SetLightRange(10000.f);
	//	light->GetLight()->SetLightAngle(XM_PI / 8);
	//	scene->AddGameObject(light);
	//}
#pragma endregion

	return scene;
}
