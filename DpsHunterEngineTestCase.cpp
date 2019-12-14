#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/hunter/HunterAiObjectContext.h"

using namespace ai;

class DpsHunterEngineTestCase : public EngineTestBase
{
  CPPUNIT_TEST_SUITE( DpsHunterEngineTestCase );
  CPPUNIT_TEST( combatVsMelee );
  CPPUNIT_TEST( summonPet );
  CPPUNIT_TEST( lowMana );
  CPPUNIT_TEST( boost );
  CPPUNIT_TEST( aoe );
  CPPUNIT_TEST( buff );
  CPPUNIT_TEST( incompatibles );
  CPPUNIT_TEST( feign_death );
  CPPUNIT_TEST( stress );
  CPPUNIT_TEST( snare );
  CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new HunterAiObjectContext(ai), "dps", "dps debuff", NULL);
		engine->addStrategy("bdps");

        addAura("aspect of the hawk");
        addTargetAura("hunter's mark");
        set<float>("distance", "current target", 15.0f);
    }

protected:
 	void combatVsMelee()
	{
        removeAura("aspect of the hawk");
        removeTargetAura("hunter's mark");

		tick();
        addAura("aspect of the hawk");

		tick();
        tick();
		tick();
		addTargetAura("serpent sting");

		tick();
		tick();

		tickInMeleeRange();
		spellAvailable("wing clip");
		addTargetAura("wing clip");
		tickInMeleeRange();
		tickInSpellRange();

		// resetSpells
		tickWithSpellUnavailable("aimed shot");

        tick();
        tick();
		tickWithSpellAvailable("auto shot");

		assertActions(">S:aspect of the hawk>T:hunter's mark>T:serpent sting>T:black arrow>T:explosive shot>T:auto shot>T:wing clip>S:flee>T:aimed shot>T:auto shot>T:chimera shot>T:auto shot>T:arcane shot");

	}

    void lowMana()
    {
        spellUnavailable("serpent sting");
        spellUnavailable("concussive shot");

        set<float>("distance", "current target", 15.0f);
		tickWithLowMana(30);

		assertActions(">T:viper sting");

    }

    void summonPet()
    {
        tickWithNoPet();
		tickWithPetLowHealth(30);

		assertActions(">S:call pet>Pet:mend pet");
	}


    void boost()
    {
        spellUnavailable("serpent sting");

		tickWithBalancePercent(1);
		tickWithBalancePercent(1);

		assertActions(">S:rapid fire>S:readiness");
    }


    void aoe()
    {
        engine->addStrategy("aoe");

		addTargetAura("hunter's mark");
		addTargetAura("black arrow");
		addTargetAura("serpent sting");
		tickWithAoeCount(3);
		tickWithAoeCount(4);

        set<Unit*>("attacker without aura", "serpent sting", MockedTargets::GetAttackerWithoutAura());
        tick();

        set<Unit*>("attacker without aura", "serpent sting", NULL);
        tick();

		assertActions(">T:multi-shot>T:volley>A:serpent sting on attacker>T:explosive shot");
    }

	void buff()
	{
		engine->removeStrategy("bdps");
		engine->addStrategy("rnature");
		removeAura("aspect of the hawk");

		tick();
		addAura("aspect of the wild");

		assertActions(">S:aspect of the wild");

	}

    void incompatibles()
    {
        engine->removeStrategy("dps");
        engine->removeStrategy("dps debuff");
        engine->addStrategies("bdps", "bspeed", "rnature", NULL);

        CPPUNIT_ASSERT(engine->ListStrategies() == "Strategies: rnature");
    }

    void feign_death()
    {
        tickWithMyAttackerCount(3);
        set<float>("distance", "current target", 5);

        assertActions(">S:feign death");
    }

    void stress()
    {
        runStressTest();
    }

    void snare()
    {
        tickWithSnareTargetSpell("concussive shot");

        assertActions(">Sn:concussive shot on snare target");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( DpsHunterEngineTestCase );
