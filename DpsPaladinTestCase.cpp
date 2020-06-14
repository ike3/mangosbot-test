#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/paladin/PaladinAiObjectContext.h"

using namespace ai;


class DpsPaladinTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( DpsPaladinTestCase );
    CPPUNIT_TEST( buff );
	CPPUNIT_TEST( combatVsMelee );
	CPPUNIT_TEST( stress );
	CPPUNIT_TEST( boost );
    CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		EngineTestBase::setUp();
		setupEngine(new PaladinAiObjectContext(ai), "dps", "close", NULL);
		engine->addStrategies("bdps", "baoe", NULL);

        addAura("retribution aura");
        addAura("seal of vengeance");
        addAura("blessing of might");

        set<float>("distance", "current target", 0.0f);
    }

protected:
    void buff()
    {
        removeAura("retribution aura");
        removeAura("blessing of might");
        removeAura("seal of vengeance");
        removeAura("holy shield");
        set<bool>("combat", "self target", true);

        tick();
        addAura("devotion aura");

        tick();
        addAura("seal of wisdom");

        tick();
        addAura("blessing of kings");

        assertActions(">S:retribution aura>S:seal of wisdom>S:blessing of might");
    }

    void combatVsMelee()
    {
		tickOutOfMeleeRange();

        tick();
		tick();
		addTargetAura("judgement of wisdom");

		tick();

		tickWithAoeCount(3);
		tickWithAoeCount(3);

		tickWithTargetLowHealth(19);

		assertActions(">T:reach melee>T:judgement of wisdom>T:judgement of light>T:crusader strike>S:divine storm>T:consecration>T:hammer of wrath");
    }

    void stress()
    {
        runStressTest();
    }

    void boost()
    {
        engine->addStrategy("boost");
        tick();

        assertActions(">S:divine favor");
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION( DpsPaladinTestCase );
