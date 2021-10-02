#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/druid/DruidAiObjectContext.h"

using namespace ai;


class HealDruidTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( HealDruidTestCase );
    CPPUNIT_TEST( healHimself );
    CPPUNIT_TEST( healOthers );
	CPPUNIT_TEST( aoe );
	CPPUNIT_TEST( range );
	CPPUNIT_TEST( melee );
	CPPUNIT_TEST( boost );
    CPPUNIT_TEST_SUITE_END();

public:
    virtual void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new DruidAiObjectContext(ai), "heal", NULL);

		addAura("moonkin form");
    }

protected:

	void healHimself()
    {
	    tick();
        addAura("tree of life");

		tickWithLowHealth(79);
        spellAvailable("rejuvenation");

		tickWithLowHealth(59);

        spellAvailable("healing touch");
        spellAvailable("regrowth");
        spellAvailable("rejuvenation");
		tickWithLowHealth(39);

        spellAvailable("healing touch");
        spellAvailable("regrowth");
        spellAvailable("rejuvenation");
        tickWithLowHealth(1);
        tickWithLowHealth(1);

        assertActions(">S:tree of life>S:rejuvenation>S:regrowth>S:healing touch>S:regrowth>S:healing touch");
    }

	void healOthers()
    {
        addAura("tree of life");

        tickWithPartyLowHealth(79);
        spellAvailable("rejuvenation");

        tickWithPartyLowHealth(59);

        spellAvailable("healing touch");
        spellAvailable("regrowth");
        spellAvailable("rejuvenation");
        tickWithPartyLowHealth(39);

        spellAvailable("healing touch");
        spellAvailable("regrowth");
        spellAvailable("rejuvenation");
        tickWithPartyLowHealth(1);
        tickWithPartyLowHealth(1);

        assertActions(">P:rejuvenation on party>P:regrowth on party>P:healing touch on party>P:regrowth on party>P:healing touch on party");
    }

    void aoe()
    {
        tickWithAoeHeal("medium");

		assertActions(">P:tranquility");
    }

    void range()
    {
        tickOutOfSpellRange();

        assertActions(">T:reach spell");
    }

    void melee()
    {
        engine->addStrategies("melee", "close", NULL);
        tick();
        addAura("omen of clarity");
        addAura("tree of life");

        tick();
        tickInMeleeRange();
        addTargetAura("faerie fire");
        tickInMeleeRange();

        assertActions(">S:omen of clarity>T:reach melee>T:faerie fire>T:melee");
    }

    void boost()
    {
        addAura("omen of clarity");
        addAura("tree of life");

        engine->addStrategy("boost");
        tick();

        assertActions(">S:nature's swiftness");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( HealDruidTestCase );
