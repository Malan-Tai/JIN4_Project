#include <gtest/gtest.h>
#include "Actor.h"
#include "Command.h"

#if TESTS
TEST(AnimInit, AnimFrames)
{
    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_idle, "resources/MC_idle");
    auto const& anim = animHolder.get(animation::ID::MC_idle);

    EXPECT_EQ(anim.nbFrames,            4);
    EXPECT_EQ(anim.hitboxesLength(),    4);
}

TEST(AnimInit, AnimInfo)
{
    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_idle, "resources/MC_idle");
    animHolder.load(animation::ID::monster_hurt, "resources/monster_hurt");
    animHolder.load(animation::ID::monster_walk, "resources/monster_walk");
    auto const& idle = animHolder.get(animation::ID::MC_idle);
    auto const& hurt = animHolder.get(animation::ID::monster_hurt);
    auto const& walk = animHolder.get(animation::ID::monster_walk);

    EXPECT_EQ(idle.looping, true);
    EXPECT_EQ(idle.reverseLoop, true);
    EXPECT_EQ(idle.timePerFrame, 500);

    EXPECT_EQ(hurt.looping, false);
    EXPECT_EQ(hurt.reverseLoop, false);
    EXPECT_EQ(hurt.timePerFrame, 100);

    auto allyLayer = idle.getHitboxes(0)[0]->getLayer();
    auto enemyLayer = walk.getHitboxes(0)[0]->getLayer();

    EXPECT_EQ(allyLayer, hitboxes::Layers::Ally);
    EXPECT_EQ(enemyLayer, hitboxes::Layers::Enemy);
}

TEST(AnimInit, FrameRectangles)
{
    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_light_attack, "resources/MC_light_attack");
    auto const& anim = animHolder.get(animation::ID::MC_light_attack);
    auto frameBoxes = anim.getHitboxes(1);

    EXPECT_EQ(frameBoxes.size(), 2);

    auto box1 = frameBoxes[0]->getRect();
    auto box2 = frameBoxes[1]->getRect();

    EXPECT_EQ(box1.left,    10);
    EXPECT_EQ(box1.top,     8);
    EXPECT_EQ(box1.width,   24);
    EXPECT_EQ(box1.height,  37);

    EXPECT_EQ(box2.left,    35);
    EXPECT_EQ(box2.top,     16);
    EXPECT_EQ(box2.width,   14);
    EXPECT_EQ(box2.height,  12);

    EXPECT_EQ(frameBoxes[0]->getType(), hitboxes::Type::Hit);
    EXPECT_EQ(frameBoxes[1]->getType(), hitboxes::Type::Hurt);
}


TEST(StatesTest, Initial)
{
    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_idle, "resources/MC_idle");

    ControllableActor a{ animHolder };
    auto const& machine = a.getMachine();

    EXPECT_TRUE(machine.is_initial());
    EXPECT_EQ(machine.state(), States::Ground);
}

TEST(StatesTest, Jump)
{
    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_idle, "resources/MC_idle");
    animHolder.load(animation::ID::MC_jump, "resources/MC_jump");
    animHolder.load(animation::ID::MC_fall, "resources/MC_fall");

    ControllableActor a{ animHolder };
    auto const& machine = a.getMachine();
    a.jump();

    EXPECT_FALSE(machine.is_initial());
    EXPECT_EQ(machine.state(), States::Fall);
    EXPECT_LT(a.getVelocity().y, 0);
}

TEST(StatesTest, LightAttack)
{
    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_idle, "resources/MC_idle");
    animHolder.load(animation::ID::MC_light_attack, "resources/MC_light_attack");

    ControllableActor a{ animHolder };
    auto const& machine = a.getMachine();
    a.lightAttack();

    EXPECT_FALSE(machine.is_initial());
    EXPECT_EQ(machine.state(), States::LightAttack);
}

TEST(StatesTest, Buffer)
{
    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_idle, "resources/MC_idle");
    animHolder.load(animation::ID::MC_fall, "resources/MC_fall");
    animHolder.load(animation::ID::MC_light_attack, "resources/MC_light_attack");
    animHolder.load(animation::ID::MC_heavy_attack, "resources/MC_heavy_attack");

    ControllableActor a{ animHolder };
    auto const& machine = a.getMachine();

    a.lightAttack();
    EXPECT_EQ(machine.state(), States::LightAttack);

    a.heavyAttack();
    EXPECT_NE(machine.state(), States::HeavyAttack);
    EXPECT_EQ(a.getBuffer(), Triggers::HeavyAttack);

    // two light attack frames
    a.update(sf::milliseconds(301), Level{});
    a.update(sf::milliseconds(301), Level{});
    EXPECT_EQ(machine.state(), States::HeavyAttack);
}

TEST(StatesTest, ChainAnims)
{
    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_idle, "resources/MC_idle");
    animHolder.load(animation::ID::MC_fall, "resources/MC_fall");
    animHolder.load(animation::ID::MC_roll, "resources/MC_roll");
    animHolder.load(animation::ID::MC_roll_attack, "resources/MC_roll_attack");

    ControllableActor a{ animHolder };
    auto const& machine = a.getMachine();

    a.pressRoll();
    a.releaseRoll();
    EXPECT_EQ(machine.state(), States::Roll);

    a.lightAttack();
    EXPECT_NE(machine.state(), States::LightAttack);
    EXPECT_EQ(a.getBuffer(), Triggers::LightAttack);

    // four roll frames
    a.update(sf::milliseconds(101), Level{});
    a.update(sf::milliseconds(101), Level{});
    a.update(sf::milliseconds(101), Level{});
    a.update(sf::milliseconds(101), Level{});
    EXPECT_EQ(machine.state(), States::RollLightAttack);
}
#endif