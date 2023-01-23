#pragma once
#include "Component.h"
#include "Object.h"

class Transform;
class MeshRenderer;
class CustomScript;
class Camera;
class Light;
class ParticleSystem;

class GameObject : public Object, public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Camera> GetCamera();
	shared_ptr<Light> GetLight();
	shared_ptr<ParticleSystem> GetParticleSystem();

	void AddComponent(shared_ptr<Component> component);

	void SetCheckFrustum(bool checkFrustum) { _checkFrustum = checkFrustum; }
	bool GetCheckFrustum() { return _checkFrustum; }

	void SetLayerIndex(uint8 layer) { _layerIndex = layer; }
	uint8 GetLayerIndex() { return _layerIndex; }

private:
	// 개수 예측 o
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	// 개수 예측 x
	vector<shared_ptr<CustomScript>> _scripts;

	bool _checkFrustum = true;
	uint8 _layerIndex = 0;
};

