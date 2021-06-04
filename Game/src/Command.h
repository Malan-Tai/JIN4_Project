#pragma once
#include "ControllableActor.h"
#include "myMain.h"

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

class PressRollCmd : public Command
{
public:
	void execute(Actor* actor) override;
};

class ReleaseRollCmd : public Command
{
public:
	void execute(Actor* actor) override;
};

class LightAttackCmd : public Command
{
public:
	void execute(Actor* actor) override;
};

class SwitchWeaponSizeCmd : public Command
{
public:
	void execute(Actor* actor) override;
};

class SwitchWeaponRangeCmd : public Command
{
public:
	void execute(Actor* actor) override;
};