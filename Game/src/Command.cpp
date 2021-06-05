#include "Command.h"
#include <iostream>

void JumpCmd::execute(Actor* actor)
{
	actor->jump();
}

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

void PressCloneCmd::execute(Actor* actor)
{
	auto cast = (ControllableActor*)actor;
	if (cast != nullptr) cast->pressClone();
}

void ReleaseCloneCmd::execute(Actor* actor)
{
	auto cast = (ControllableActor*)actor;
	if (cast != nullptr) cast->releaseClone();
}

void HeavyAttackCmd::execute(Actor* actor)
{
	actor->heavyAttack();
}
