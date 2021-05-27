#pragma once
#include "Actor.h"

class Command
{
public:
	virtual ~Command() = default;
	virtual void execute(Actor* actor) = 0;
};

class JumpCmd : public Command
{
public:
	void execute(Actor* actor) override;
};