#include "Command.h"

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

void ShootCmd::execute(Actor* actor)
{
	actor->shoot();
}
