#include "pch.h"

#include "aitest.h"
#include "EngineTestBase.h"
#include "../../modules/Bots/playerbot/strategy/warrior/WarriorAiObjectContext.h"
#include "../modules/Bots/playerbot/strategy/values/PositionValue.h"

using namespace ai;


class SitTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( SitTestCase );
    CPPUNIT_TEST( sit );
    CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		EngineTestBase::setUp();
		setupEngine(new WarriorAiObjectContext(ai), "stay", "sit", NULL);

		addAura("battle shout");
		addAura("defensive stance");
    }

protected:
    void sit()
    {
        time_t now = time(0);

        set<time_t>("stay time", now);
        tick();

        set<time_t>("stay time", now - 40);
		tick();

        assertActions(">S:stay>S:sit");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( SitTestCase );
