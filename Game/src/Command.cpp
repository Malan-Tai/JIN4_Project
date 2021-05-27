#include "Command.h"

void JumpCmd::execute(Actor* actor)
{
	actor->jump();
}
