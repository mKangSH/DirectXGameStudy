#pragma once
#include "Component.h"

class CustomScript : public Component
{
public:
	CustomScript();
	virtual ~CustomScript();

private:
	virtual void FinalUpdate() sealed {}
};

