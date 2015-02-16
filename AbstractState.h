#pragma once
#include "settings.h"
class AbstractState
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleEvents() = 0;

	virtual bool OnEnter() = 0;
	virtual bool OnExit() = 0;

};
