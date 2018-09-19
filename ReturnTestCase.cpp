#include "pch.h"

#include "aitest.h"
#include "EngineTestBase.h"
#include "../../modules/Bots/playerbot/strategy/warrior/WarriorAiObjectContext.h"
#include "../modules/Bots/playerbot/strategy/values/PositionValue.h"

using namespace ai;


class ReturnTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( ReturnTestCase );
    CPPUNIT_TEST( move );
    CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		EngineTestBase::setUp();
		setupEngine(new WarriorAiObjectContext(ai), "return", NULL);

		addAura("battle shout");
		addAura("defensive stance");
    }

protected:
    void move()
    {
        time_t now = time(0);

        set<time_t>("stay time", now - 40);
        ai::PositionMap& posMap = context->GetValue<ai::PositionMap&>("position")->Get();
        ai::Position randomPosition; randomPosition.Set(1, 1, 1);
        posMap["return"] = randomPosition;
		tick();

        posMap["random"] = randomPosition;

        set<float>("distance", "position_random", 10);
        tick();

        assertActions(">S:set return position>S:return");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( ReturnTestCase );
