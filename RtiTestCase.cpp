#include "pch.h"

#include "aitest.h"
#include "EngineTestBase.h"
#include "../../modules/Bots/playerbot/strategy/warrior/WarriorAiObjectContext.h"

using namespace ai;


class RtiTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( RtiTestCase );
    CPPUNIT_TEST( rti );
    CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		EngineTestBase::setUp();
		setupEngine(new WarriorAiObjectContext(ai), "mark rti", NULL);

		addAura("battle shout");
		addAura("defensive stance");
    }

protected:
    void rti()
    {
        set<Unit*>("rti target", NULL);
		tick();

        assertActions(">S:mark rti");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( RtiTestCase );
