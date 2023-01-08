#pragma once
#include "CustomScript.h"

class TestCameraScript : public CustomScript
{
public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

private:
	float _speed = 100.0f;
};

