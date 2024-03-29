#include "pch.h"
#include "aitest.h"

#include "EngineTestBase.h"

using namespace ai;


class NonCombatEngineTestCase : public EngineTestBase
{
  CPPUNIT_TEST_SUITE( NonCombatEngineTestCase );
      CPPUNIT_TEST( stay );
      CPPUNIT_TEST( eatDrink );
      CPPUNIT_TEST( dpsAssist );
      CPPUNIT_TEST( defense );
      CPPUNIT_TEST( dpsAoe );
      CPPUNIT_TEST( doNotGrindIfLowMpHp );
      CPPUNIT_TEST( grindIfNoMana );
      CPPUNIT_TEST( loot );
      CPPUNIT_TEST( loot_failed );
      CPPUNIT_TEST( runaway );
      CPPUNIT_TEST( passive );
      CPPUNIT_TEST( movementStrategies );
      CPPUNIT_TEST( assistStrategies );
      CPPUNIT_TEST( tell_target );
      CPPUNIT_TEST( pvp );
      CPPUNIT_TEST( collision );
      CPPUNIT_TEST( patrol );
  CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		EngineTestBase::setUp();
		setupEngine(new AiObjectContext(ai), NULL);
	}

protected:
    void runaway()
    {
        engine->addStrategy("runaway");
		tickInMeleeRange();
        assertActions(">S:flee");
    }

    void stay()
    {
		engine->addStrategy("stay");

		tickWithAttackerCount(0);
		assertActions(">S:stay");
    }

    void dpsAssist()
    {
        engine->addStrategy("stay");
        engine->addStrategy("dps assist");

		tickWithNoTarget();
        set<Unit*>("current target", MockedTargets::GetTargetForDps());
		tick();

		assertActions(">Dps:dps assist>S:stay");
    }

    void defense()
    {
        engine->addStrategy("stay");
        engine->addStrategy("defense");

		tickWithNoTarget();
        set<Unit*>("current target", MockedTargets::GetTargetForDefense());
		tick();

		assertActions(">Defense:defense>S:stay");
    }

    void dpsAoe()
    {
        engine->addStrategy("stay");
        engine->addStrategy("dps aoe");

		tickWithNoTarget();
        set<Unit*>("current target", MockedTargets::GetTargetForDps());
		tick();

		assertActions(">Dps:dps aoe>S:stay");
    }

	void pvp()
	{
		engine->addStrategy("stay");
		engine->addStrategy("pvp");

        set<Unit*>("current target", MockedTargets::GetEnemyPlayer());
		tick();

		tickWithNoTarget();

		assertActions(">S:stay>Enemy:attack enemy player");
	}

    void loot()
    {
		engine->addStrategy("stay");
		engine->addStrategy("loot");

		tickWithLootAvailable();

        set<float>("distance", "loot target", 15.0f);
        tick();

        set<float>("distance", "loot target", 0.0f);
        set<bool>("can loot", true);
        tick();

        set<bool>("can loot", false);
        tick();

        set<uint8>("bag space", 0);
        tickWithLootAvailable();
        set<uint8>("bag space", 1);

        assertActions(">S:loot>S:move to loot>S:open loot>S:stay>S:check mount state");
    }

    void loot_failed()
    {
		engine->addStrategy("follow");
		engine->addStrategy("loot");

		tickWithLootAvailable();

        set<float>("distance", "loot target", 15.0f);
        tick();
        set<float>("distance", "loot target", 0.0f);

        set<bool>("can loot", true);
        tick();

        spellAvailable("open loot");
        tick();

        assertActions(">S:loot>S:move to loot>S:open loot>S:open loot");
    }

    void eatDrink()
    {
        engine->addStrategy("food");
        set<uint8>("item count", "food", 1);
        set<uint8>("item count", "drink", 1);

        tickWithLowHealth(1);
        tickWithLowMana(1);

        assertActions(">S:food>S:drink");
    }

    void passive()
    {
        engine->addStrategy("stay");
        engine->addStrategy("passive");

        tick();
		tickWithNoTarget();

        assertActions(">S:stay>S:check mount state");
    }

    void movementStrategies()
    {
        engine->addStrategy("follow");
        engine->addStrategy("follow line");
        engine->addStrategy("be near");
        engine->addStrategy("runaway");
        engine->addStrategy("stay");

        cout << engine->ListStrategies();
        CPPUNIT_ASSERT(engine->ListStrategies() == "Strategies: stay");
    }

    void assistStrategies()
    {
        engine->addStrategy("dps assist");
        engine->addStrategy("tank asssist");
        engine->addStrategy("dps aoe");
        engine->addStrategy("tank asssist");
        engine->addStrategy("grind");

        cout << engine->ListStrategies();
        CPPUNIT_ASSERT(engine->ListStrategies() == "Strategies: grind");
    }

    void tell_target()
    {
        engine->addStrategy("tell target");
        set<Unit*>("old target", MockedTargets::GetPet());
        tick();
        set<Unit*>("current target", NULL);
        tick();
        assertActions(">S:tell target");
    }

    void doNotGrindIfLowMpHp()
    {
        engine->addStrategy("stay");
        engine->addStrategy("grind");
        context->GetValue<Unit*>("grind target")->Set(MockedTargets::GetGridTarget());

        tick();
        set<uint8>("health", "self target", 1);
        tickWithNoTarget();
        set<uint8>("health", "self target", 100);
        set<uint8>("mana", "self target", 1);
        tickWithNoTarget();

        set<uint8>("health", "self target", 100);
        set<uint8>("mana", "self target", 100);
        tickWithNoTarget();

        set<uint8>("health", "self target", 1);
        set<bool>("combat", "self target", true);
        tickWithNoTarget();

        set<bool>("combat", "self target", false);
        tickWithNoTarget();

        assertActions(">S:stay>S:check mount state>S:check values>Grind:attack anything>Grind:attack anything>S:stay");
    }

    void grindIfNoMana()
    {
        engine->addStrategy("stay");
        engine->addStrategy("grind");
        context->GetValue<Unit*>("grind target")->Set(MockedTargets::GetGridTarget());

        tick();
        set<uint8>("health", "self target", 100);
        set<uint8>("mana", "self target", 0);
        tickWithNoTarget();

        assertActions(">S:stay>Grind:attack anything");
    }

    void collision()
    {
        engine->addStrategy("collision");

        tickWithCollision();

        assertActions(">S:move out of collision");
    }

    void patrol()
    {
        engine->addStrategy("patrol");

        tick();

        assertActions(">S:patrol");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( NonCombatEngineTestCase );
