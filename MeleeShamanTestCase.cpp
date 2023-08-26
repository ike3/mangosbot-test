#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/shaman/ShamanAiObjectContext.h"

using namespace ai;


class MeleeShamanTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( MeleeShamanTestCase );
    CPPUNIT_TEST( combat );
	CPPUNIT_TEST( buff );
	CPPUNIT_TEST( incompatibles );
    CPPUNIT_TEST( aoe );
    CPPUNIT_TEST( boost );
    CPPUNIT_TEST( interruptSpell );
    CPPUNIT_TEST( interrupt_enemy_healer );
    CPPUNIT_TEST( stress );
    CPPUNIT_TEST( earth );
    CPPUNIT_TEST( fire );
    CPPUNIT_TEST( frost );
    CPPUNIT_TEST( air );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new ShamanAiObjectContext(ai), "melee", NULL);

        addAura("lightning shield");
    }

protected:
    void combat()
    {
		tickInMeleeRange();

        tick();

        tick();
		tick();

        assertActions(">S:searing totem>T:stormstrike>T:lava lash>T:melee");
    }

    void buff()
    {
        engine->addStrategy("bdps");
        removeAura("lightning shield");
        tickInMeleeRange();

        tick();
        addAura("lightning shield");

        assertActions(">S:lightning shield>S:searing totem");
    }

    void earth()
    {
        engine->addStrategy("earth");
        tickInMeleeRange();
        tick();
        tickWithItemForSpell("rockbiter weapon");

        assertActions(">S:searing totem>T:earth shock>S:rockbiter weapon");
    }

    void fire()
    {
        engine->addStrategy("fire");
        tickInMeleeRange();
        tick();
        tickWithItemForSpell("flametongue weapon");

        assertActions(">S:searing totem>T:flame shock>S:flametongue weapon");
    }

    void frost()
    {
        engine->addStrategy("frost");
        tickInMeleeRange();
        tick();
        tickWithItemForSpell("frostbrand weapon");

        assertActions(">S:searing totem>T:frost shock>S:frostbrand weapon");
    }

    void air()
    {
        engine->addStrategy("air");
        tickInMeleeRange();
        tick();
        tickWithItemForSpell("windfury weapon");

        assertActions(">S:searing totem>T:flame shock>S:windfury weapon");
    }

    void incompatibles()
    {
        engine->addStrategies("melee", "dps", "heal", NULL);

        CPPUNIT_ASSERT(engine->ListStrategies() == "Strategies: heal");
    }

    void aoe()
    {
        engine->addStrategy("melee aoe");

        tickInMeleeRange();
        tickWithAoeCount(3);
        tickWithAoeCount(3);
        tickWithAoeCount(3);

        assertActions(">S:searing totem>S:stoneskin totem>S:magma totem>T:fire nova");
    }

    void boost()
    {
        tickWithBalancePercent(1);
        tickWithBalancePercent(1);

        assertActions(">S:heroism>S:bloodlust");
    }

    void interruptSpell()
    {
        tickWithTargetIsCastingNonMeleeSpell();

        assertActions(">T:wind shear");
    }

    void interrupt_enemy_healer()
    {
        tickWithEnemyHealerIsCastingInterruptableSpell("wind shear");

        assertActions(">H:wind shear on enemy healer");
    }

    void stress()
    {
        runStressTest();
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( MeleeShamanTestCase );
