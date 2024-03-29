#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/shaman/ShamanAiObjectContext.h"

using namespace ai;


class ShamanNonCombatTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( ShamanNonCombatTestCase );
        CPPUNIT_TEST( ressurect );
        CPPUNIT_TEST( healing );
        CPPUNIT_TEST( aoe );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new ShamanAiObjectContext(ai), "nc", NULL);
    }

protected:
	void ressurect()
	{
		tickWithDeadPartyMember();

		assertActions(">P:ancestral spirit");
	}

	void healing()
	{
	    tickWithLowHealth(10);
	    spellAvailable("healing wave");
	    tickWithPartyLowHealth(10);

		assertActions(">S:healing wave>P:healing wave on party");
	}

	void aoe()
	{
	    tickWithAoeHeal("medium");

		assertActions(">P:chain heal");
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( ShamanNonCombatTestCase );
