#pragma once
#include "Object.h"

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM, 
	MESH_RENDERER,
	CAMERA,
	LIGHT,
	PARTICLE_SYSTEM,
	// ...

	// UNITY Style: CUSTOM_SCRIPT�� ���� ��ü�� �ƴ�
	CUSTOM_SCRIPT, 
	END
};

enum 
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;

class Component : public Object
{
public:
	Component(COMPONENT_TYPE type);
	// base class�̱� ������ memory leak ������ ���� virtual Ű���� ����
	virtual ~Component();

public:
	// ������Ʈ ���� �ֱ⿡ ���� �Լ�
	virtual void Awake() {} 
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void FinalUpdate() {}
	
public:
	COMPONENT_TYPE GetType() { return _type; }
	bool IsValid() { return _gameObject.expired() == false; }

	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	friend class GameObject;
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;

	// weak_ptr�� ������ ����
	// Shared_ptr ������ ��� Component, GameObject ��ȣ �� ������ �˰� �־�� �Ѵ�.
	// ��ȣ �� �����ϴ� �ڵ尡 ������� Reference Count�� 0�� ���� �ʴ� ���� �߻�

	// shared_ptr�� �ذ��ϴ� ���
	// �� ��� GameObject�� destroy�� �� ��������� Reference count�� �����ش�.
	weak_ptr<GameObject> _gameObject;
};

