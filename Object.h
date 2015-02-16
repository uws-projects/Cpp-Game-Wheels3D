#pragma once
#include "Settings.h"

class Object
{
public:
	virtual void Initialize() = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void HandleEvents() = 0;
	~Object(void) {};
};