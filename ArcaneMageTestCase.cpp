#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/mage/MageAiObjectContext.h"

using namespace ai;


class ArcaneMageTestCase : public EngineTestBase
{
  CPPUNIT_TEST_SUITE( ArcaneMageTestCase );
      CPPUNIT_TEST( combatVsMelee );
      CPPUNIT_TEST( avoid_melee );
      CPPUNIT_TEST( stress );
  CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new MageAiObjectContext(ai), "arcane", "ranged", NULL);
    }

protected:
 	void combatVsMelee()
	{
        tick();
		addAura("arcane blast");

        tick();

        addAura("missile barrage");
        tickWithNoAggro();
        tickWithSpellAvailable("arcane missiles");
        tickWithNoAggro();

        assertActions(">T:arcane blast>T:arcane barrage>T:arcane missiles>T:shoot>T:arcane missiles");
	}

 	void avoid_melee()
 	{
		tickInMeleeRange();
		tickInMeleeRange();

		assertActions(">T:frost nova>S:flee");
	}

    void stress()
    {
        runStressTest();
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION( ArcaneMageTestCase );
