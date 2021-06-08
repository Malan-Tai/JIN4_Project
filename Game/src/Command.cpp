#include "Command.h"
#include <iostream>

void JumpCmd::execute(Actor* actor)
{
	actor->jump();
}

// this kind of commands, useful only to the player because related to user inputs
// might be incorrectly programmed, but shouldn't be called on anything else than a ControllableActor anyway
void PressRollCmd::execute(Actor* actor)
{
	auto cast = (ControllableActor*)actor;
	if (cast != nullptr) cast->pressRoll();
}

void ReleaseRollCmd::execute(Actor* actor)
{
	auto cast = (ControllableActor*)actor;
	if (cast != nullptr) cast->releaseRoll();
}

void LightAttackCmd::execute(Actor* actor)
{
	actor->lightAttack();
}

void SwitchWeaponSizeCmd::execute(Actor* actor)
{
	auto cast = (ControllableActor*)actor;
	if (cast != nullptr) cast->switchWeaponSize();
}

void SwitchWeaponRangeCmd::execute(Actor* actor)
{
	auto cast = (ControllableActor*)actor;
	if (cast != nullptr) cast->switchWeaponRange();
}

void CloneCmd::execute(Actor* actor)
{
	auto cast = (ControllableActor*)actor;
	if (cast != nullptr) cast->pressClone();
}

void HeavyAttackCmd::execute(Actor* actor)
{
	actor->heavyAttack();
}

void GrabCmd::execute(Actor* actor)
{
	actor->grab();
}
