#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/warrior/WarriorAiObjectContext.h"

using namespace ai;


class TankWarriorTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( TankWarriorTestCase );
    CPPUNIT_TEST( buff );
    CPPUNIT_TEST( melee );
    CPPUNIT_TEST( warriorMustHoldAggro );
    CPPUNIT_TEST( aoe );
    CPPUNIT_TEST( low_health );
	CPPUNIT_TEST( snare );
    CPPUNIT_TEST( interruptSpells );
    CPPUNIT_TEST( incompatibles );
    CPPUNIT_TEST( interrupt_enemy_healer );
    CPPUNIT_TEST( stress );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
        EngineTestBase::setUp();
        setupEngine(new WarriorAiObjectContext(ai), "tank", NULL);

        // this buff is combat-only, so skip for most test cases
        addAura("battle shout");
        addAura("defensive stance");
        set<uint8>("rage", "self target", 20);
        set<float>("distance", "current target", ATTACK_DISTANCE - 1);
    }

protected:
    void interruptSpells()
    {
		tickWithTargetIsCastingNonMeleeSpell();

        assertActions(">T:shield bash");
    }

    void low_health()
    {
        tickWithLowHealth(50); // shield wall
        tickWithLowHealth(50); // shield block

		tickWithLowHealth(4);
		tickWithLowHealth(4);

		assertActions(">T:shield wall>S:shield block>S:last stand>T:intimidating shout");
    }

    void buff()
    {
        removeAura("defensive stance");
        tick();
        addAura("defensive stance");

        set<uint8>("rage", "self target", 0);
        removeAura("battle shout");
        tick();

        tick();
        addAura("battle shout");

        tick();

		assertActions(">S:defensive stance>S:bloodrage>S:battle shout>S:shield block");
    }

    void aoe()
    {
        tickWithAoeCount(2);
        tickWithAoeCount(2);

		spellAvailable("battle shout");
		tickWithAoeCount(3);

        assertActions(">T:demoralizing shout>T:cleave>T:battle shout");
    }

    void warriorMustHoldAggro()
    {
		tickWithNoAggro();

		assertActions(">T:taunt");
    }

    void melee()
    {
        tickOutOfMeleeRange();
        tickWithRage(0);
        tickInMeleeRange();
        tick();
        tick();

        tickWithRage(41);
        tickWithSpellAvailable("heroic strike");

        assertActions(">T:reach melee>S:bloodrage>S:shield block>T:sunder armor>T:melee>T:shield slam>T:heroic strike");
    }

    void revengeIfDodge()
    {
        tick(); // melee
        spellAvailable("revenge");
        tick(); // defensive stance
        tick(); // revenge

		assertActions(">T:melee>S:defensive stance>T:revenge");
    }

	void snare()
	{
		tickWithSnareTargetSpell("concussion blow");

		assertActions(">Sn:concussion blow on snare target");
	}


    void incompatibles()
    {
        engine->addStrategies("tank", "dps", NULL);

        CPPUNIT_ASSERT(engine->ListStrategies() == "Strategies: dps");
    }

    void interrupt_enemy_healer()
    {
        tickWithEnemyHealerIsCastingInterruptableSpell("shield bash");

        assertActions(">H:shield bash on enemy healer");
    }

    void stress()
    {
        runStressTest();
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( TankWarriorTestCase );
