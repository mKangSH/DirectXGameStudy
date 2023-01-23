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

	// UNITY Style: CUSTOM_SCRIPT는 단일 개체가 아님
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
	// base class이기 때문에 memory leak 방지를 위해 virtual 키워드 적용
	virtual ~Component();

public:
	// 컴포넌트 생명 주기에 관한 함수
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

	// weak_ptr로 선언한 이유
	// Shared_ptr 선언일 경우 Component, GameObject 상호 간 정보를 알고 있어야 한다.
	// 상호 간 참조하는 코드가 만들어져 Reference Count가 0이 되지 않는 문제 발생

	// shared_ptr로 해결하는 경우
	// 이 경우 GameObject가 destroy될 때 명시적으로 Reference count를 끊어준다.
	weak_ptr<GameObject> _gameObject;
};

