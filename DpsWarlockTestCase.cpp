#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/warlock/WarlockAiObjectContext.h"

using namespace ai;


class DpsWarlockTestCase : public EngineTestBase
{
  CPPUNIT_TEST_SUITE( DpsWarlockTestCase );
      CPPUNIT_TEST( combatVsMelee );
      CPPUNIT_TEST( aoe );
      CPPUNIT_TEST( low_mana );
      CPPUNIT_TEST( cc );
      CPPUNIT_TEST( stress );
      CPPUNIT_TEST( boost );
  CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
        EngineTestBase::setUp();
        setupEngine(new WarlockAiObjectContext(ai), "dps", "dps debuff", NULL);
    }

protected:
 	void combatVsMelee()
	{
        tick();
        addTargetAura("siphon life");
        tick();
        addTargetAura("corruption");
        tick();
        addTargetAura("curse of agony");
        tick();
        addTargetAura("immolate");

 	    tick();
        tick();

        tickWithLowHealth(39);

        tick();

		tickWithTargetLowHealth(19);

        set<uint8>("item count", "soul shard", 2);
		spellAvailable("drain soul");
		tick();

		spellAvailable("shadow bolt");
		addAura("shadow trance");
		tick();

        spellAvailable("shadow bolt");
		addAura("backlash");
		tick();

		assertActions(">T:siphon life>T:corruption>T:curse of agony>T:immolate>T:conflagrate>T:incinirate>T:drain life>T:shadow bolt>T:drain soul>T:shoot>T:shadow bolt>T:shadow bolt");
	}

    void low_mana()
    {
        tickWithLowMana(1);

		assertActions(">S:life tap");
    }

    void aoe()
    {
        engine->addStrategy("aoe");
        set<Unit*>("attacker without aura", "corruption", MockedTargets::GetAttackerWithoutAura());
        set<Unit*>("attacker without aura", "curse of agony", MockedTargets::GetAttackerWithoutAura());
        set<Unit*>("attacker without aura", "siphon life", MockedTargets::GetAttackerWithoutAura());

        tick();
        set<Unit*>("attacker without aura", "siphon life", NULL);

        tick();
        set<Unit*>("attacker without aura", "curse of agony", NULL);

        tick();
        set<Unit*>("attacker without aura", "corruption", NULL);

        tickWithAoeCount(3);

		assertActions(">A:siphon life on attacker>A:curse of agony on attacker>A:corruption on attacker>T:rain of fire");
    }

    void cc()
    {
        engine->addStrategy("cc");
        tickWithCcTarget("fear");

        assertActions(">Cc:fear on cc");
    }

    void boost()
    {
        engine->addStrategy("boost");
        tick();

        assertActions(">S:amplify curse");
    }

    void stress()
    {
        runStressTest();
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( DpsWarlockTestCase );
