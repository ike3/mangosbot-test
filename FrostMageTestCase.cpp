#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/mage/MageAiObjectContext.h"

using namespace ai;


class FrostMageTestCase : public EngineTestBase
{
  CPPUNIT_TEST_SUITE( FrostMageTestCase );
  CPPUNIT_TEST( combatVsMelee );
  CPPUNIT_TEST( dispel );
  CPPUNIT_TEST( boost );
  CPPUNIT_TEST( cc );
  CPPUNIT_TEST( aoe );
  CPPUNIT_TEST( incompatibles );
  CPPUNIT_TEST( low_mana );
  CPPUNIT_TEST( interrupt_enemy_healer );
  CPPUNIT_TEST( stress );
  CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new MageAiObjectContext(ai), "frost", "ranged", NULL);
    }

protected:
 	void combatVsMelee()
	{
        tick();
        spellAvailable("frostbolt");

		tickInMeleeRange();
		tickInMeleeRange();

		spellAvailable("frostbolt");
		tickInSpellRange();
        tick();

		tickWithLowHealth(19);

        spellAvailable("frost nova");
        spellAvailable("frostbolt");
        tickInMeleeRange();
        tickInMeleeRange();

		assertActions(">T:frostbolt>T:frost nova>S:flee>T:frostbolt>T:shoot>S:ice block>T:frost nova>S:flee");
	}

    void dispel()
    {
        engine->addStrategy("cure");
        tick();

		tickWithAuraToDispel(DISPEL_CURSE);
		spellAvailable("remove curse");
		tickWithPartyAuraToDispel(DISPEL_CURSE);

        tickWithAuraToDispel(DISPEL_CURSE);
		spellAvailable("remove lesser curse");
		tickWithPartyAuraToDispel(DISPEL_CURSE);

        tick();
		tickWithTargetAuraToDispel(DISPEL_MAGIC);

		assertActions(">T:frostbolt>S:remove curse>P:remove curse on party>S:remove lesser curse>P:remove lesser curse on party>T:shoot>T:spellsteal");
    }

    void cc()
    {
        engine->addStrategy("cc");
        tickWithCcTarget("polymorph");

        assertActions(">Cc:polymorph");
    }

   	void aoe()
	{
        engine->addStrategy("frost aoe");

		tick();
		tickWithAoeCount(4);
		tick();

		assertActions(">T:frostbolt>T:blizzard>T:shoot");
	}

   	void incompatibles()
   	{
        engine->addStrategies("frost", "fire", NULL);

        CPPUNIT_ASSERT(engine->ListStrategies() == "Strategies: fire, ranged");
   	}

    void low_mana()
    {
        engine->addStrategy("flee");
        addAura("arcane intellect");
        addPartyAura("arcane intellect");
        addAura("mage armor");

        tickWithLowMana(5);

        set<uint8>("item count", "drink", 0);
        set<float>("distance", "current target", 5);
        tickWithLowMana(5);
        tickWithLowMana(5);

        assertActions(">S:evocation>T:frost nova>S:flee");
    }

    void interrupt_enemy_healer()
    {
        tickWithEnemyHealerIsCastingInterruptableSpell("counterspell");

        assertActions(">H:counterspell on enemy healer");
    }

    void boost()
    {
        engine->addStrategy("boost");
        tick();
        tick();

        assertActions(">S:presence of mind>S:arcane power");
    }

    void stress()
    {
        runStressTest();
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( FrostMageTestCase );
