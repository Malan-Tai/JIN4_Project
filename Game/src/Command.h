#pragma once
#include "Actor.h"

class Command
{
public:
	virtual ~Command() = default;
	virtual void execute(Actor& actor) = 0;
};