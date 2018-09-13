#include "pch.h"

#include "aitest.h"
#include "EngineTestBase.h"
#include "../../modules/Bots/playerbot/strategy/warrior/WarriorAiObjectContext.h"

using namespace ai;


class RpgTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( RpgTestCase );
    CPPUNIT_TEST( rpg );
    CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		EngineTestBase::setUp();
		setupEngine(new WarriorAiObjectContext(ai), "nc", "rpg", NULL);

		addAura("battle shout");
		addAura("defensive stance");
    }

protected:
    void rpg()
    {
        list<ObjectGuid> rpgTargets;
        rpgTargets.push_back(ObjectGuid(HIGHGUID_PLAYER, uint32(1)));
        set<list<ObjectGuid> >("possible rpg targets", rpgTargets);

        set<Unit*>("rpg target", NULL);
		tick();
        set<Unit*>("rpg target", MockedTargets::GetRpgTarget());

        set<float>("distance", "rpg target", 70.0f);
        tick();
        set<float>("distance", "rpg target", 0.0f);

        tick();

        assertActions(">S:choose rpg target>S:move to rpg target>S:rpg");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( RpgTestCase );
