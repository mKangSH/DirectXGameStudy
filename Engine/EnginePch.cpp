#include "pch.h"
#include "EnginePch.h"
#include "Engine.h"

unique_ptr<Engine> GEngine;

void GlobalEngineInit()
{
	GEngine = make_unique<Engine>();
}
