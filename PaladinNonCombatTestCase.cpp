#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/paladin/PaladinAiObjectContext.h"

using namespace ai;


class PaladinNonCombatTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( PaladinNonCombatTestCase );
		CPPUNIT_TEST( resurrect );
        CPPUNIT_TEST( curePoison );
        CPPUNIT_TEST( cureMagic );
        CPPUNIT_TEST( cureDisease );
    CPPUNIT_TEST_SUITE_END();


public:
    virtual void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new PaladinAiObjectContext(ai), "nc", NULL);

        set<uint8>("attacker count", 0);
    }

protected:
	void resurrect()
	{
		tickWithDeadPartyMember();
		assertActions(">P:redemption");
	}

    void cureDisease()
    {
        engine->addStrategy("cure");
        cureKind(DISPEL_DISEASE);
        assertActions(">S:cleanse>P:cleanse disease on party>S:purify>P:purify disease on party");
    }

    void curePoison()
    {
        engine->addStrategy("cure");
        cureKind(DISPEL_POISON);
        assertActions(">S:cleanse>P:cleanse poison on party>S:purify>P:purify poison on party");
    }

    void cureMagic()
    {
        engine->addStrategy("cure");
        cureKind(DISPEL_MAGIC);
        assertActions(">S:cleanse>P:cleanse magic on party>S:check mount state>S:check values");
    }

    void cureKind(DispelType type)
    {
        spellAvailable("cleanse");
        tickWithAuraToDispel(type);

        spellAvailable("cleanse");
        tickWithPartyAuraToDispel(type);

        spellAvailable("purify");
        tickWithAuraToDispel(type);

        spellAvailable("purify");
        tickWithPartyAuraToDispel(type);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( PaladinNonCombatTestCase );
