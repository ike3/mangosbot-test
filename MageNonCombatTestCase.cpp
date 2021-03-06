#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/mage/MageAiObjectContext.h"

using namespace ai;


class MageNonCombatTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( MageNonCombatTestCase );
    CPPUNIT_TEST( buff );
    CPPUNIT_TEST( bdps );
    CPPUNIT_TEST( bmana );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new MageAiObjectContext(ai), "nc", NULL);
		set<uint8>("item count", "food", 1);
		set<uint8>("item count", "drink", 1);
    }

protected:
    void buff()
    {
        engine->addStrategy("buff");
		tick();
        addAura("arcane intellect");

		tickWithSpellAvailable("arcane intellect");
        addPartyAura("arcane intellect");

		tickWithNoDrink();
		tickWithNoFood();

		set<Unit*>("party member without food", MockedTargets::GetPartyMember());
        set<uint8>("item count", "conjured food", 1);
		tick();
		set<Unit*>("party member without food", NULL);

		set<Unit*>("party member without water", MockedTargets::GetPartyMember());
        set<uint8>("item count", "conjured water", 1);
        tick();
		set<Unit*>("party member without water", NULL);

        assertActions(">S:arcane intellect>P:arcane intellect on party>S:conjure water>S:conjure food>P:give food>P:give water");
    }

    void bmana()
    {
        engine->addStrategy("bmana");

        addAura("arcane intellect");
        addPartyAura("arcane intellect");

        tick();
        tick();
        tick();

        assertActions(">S:mage armor>S:ice armor>S:frost armor");
    }

    void bdps()
    {
        engine->addStrategy("bdps");

        addAura("arcane intellect");
        addPartyAura("arcane intellect");

        tick();
        tick();
        tick();
        tick();

        assertActions(">S:molten armor>S:mage armor>S:ice armor>S:frost armor");
    }


};

CPPUNIT_TEST_SUITE_REGISTRATION( MageNonCombatTestCase );
