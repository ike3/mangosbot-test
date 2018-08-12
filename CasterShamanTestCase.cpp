#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/shaman/ShamanAiObjectContext.h"

using namespace ai;


class CasterShamanTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( CasterShamanTestCase );
    CPPUNIT_TEST( combat );
	CPPUNIT_TEST( buff );
	CPPUNIT_TEST( incompatibles );
    CPPUNIT_TEST( aoe );
    CPPUNIT_TEST( snare );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new ShamanAiObjectContext(ai), "caster", NULL);

        addAura("water shield");
        addTargetAura("earth shock");
    }

protected:
    void combat()
    {
        removeTargetAura("earth shock");

		tick();
        tick();
        addTargetAura("flame shock");

		tick();
		tick();

        assertActions(">T:earth shock>T:flame shock>S:searing totem>T:lightning bolt");
    }

    void buff()
    {
        engine->addStrategy("bmana");
        removeAura("water shield");

        tickWithItemForSpell("flametongue weapon");

        tick();
        addAura("water shield");

        assertActions(">S:flametongue weapon>S:water shield");
    }

    void incompatibles()
    {
        engine->addStrategies("melee", "dps", "heal", "caster", NULL);

        CPPUNIT_ASSERT(engine->ListStrategies() == "Strategies: caster");
    }

    void aoe()
    {
        engine->addStrategy("caster aoe");

        tickWithAoeCount(3);
        tickWithAoeCount(3);

        assertActions(">S:flametongue totem>T:chain lightning");
    }

    void snare()
    {
        tickWithSnareTargetSpell("frost shock");

        assertActions(">Sn:frost shock on snare target");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( CasterShamanTestCase );
