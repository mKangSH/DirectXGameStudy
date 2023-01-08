#pragma once

enum class OBJECT_TYPE : uint8
{
	NONE,
	GAMEOBJECT,  // Unity: Prefab, Unreal: Actor?
	COMPONENT,
	MATERIAL,
	MESH,
	SHADER,
	TEXTURE,

	END
};

enum
{
	OBJECT_TYPE_COUNT = static_cast<uint8>(OBJECT_TYPE::END)
};

class Object
{
public:
	Object(OBJECT_TYPE type);
	virtual ~Object();
	
	OBJECT_TYPE GetType() { return _objectType; }

	void SetName(const wstring& name) { _name = name; }
	const wstring& GetName() { return _name; }

	// TODO : Instantiate
	// 오브젝트의 클론을 만들어 주는 역할

protected:
	friend class Resources;
	virtual void Load(const wstring& path) {}
	virtual void Save(const wstring& path) {}

protected:
	OBJECT_TYPE _objectType = OBJECT_TYPE::NONE;
	wstring _name;
};

