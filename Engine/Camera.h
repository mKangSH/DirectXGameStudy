#pragma once
#include "Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,
	ORTHOGRAPHIC,

	END
};

class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

	virtual void FinalUpdate() override;
	void Render();

	void SetProjectionType(PROJECTION_TYPE type) { _type = type; }
	PROJECTION_TYPE GetProjectionType() { return _type; }

	void SortGameObject();
	void RenderDeferred();
	void RenderForward();

	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
		{
			_cullingMask |= (1 << layer);
		}

		else
		{
			_cullingMask &= ~(1 << layer);
		}
	}

	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.0f;
	float _far = 1000.0f;
	float _fov = XM_PI / 4.0f;
	float _scale = 1.0f;

	Matrix _matView = {};
	Matrix _matProjection = {};

	Frustum _frustum;
	uint32 _cullingMask = 0;

private:
	vector<shared_ptr<GameObject>> _vecDeferred;
	vector<shared_ptr<GameObject>> _vecForward;

public:
	// TEMP
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

