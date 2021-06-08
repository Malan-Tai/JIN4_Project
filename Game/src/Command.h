#pragma once
#include "ControllableActor.h"
#include "myMain.h"

// command interface for the Command deisgn patter
// lower are all the different commands used for input
// (for now, only the user inputs use commands, but more complex AIs might too)
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

class HeavyAttackCmd : public Command
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

class CloneCmd : public Command
{
public:
	void execute(Actor* actor) override;
};

class GrabCmd : public Command
{
public:
	void execute(Actor* actor) override;
};