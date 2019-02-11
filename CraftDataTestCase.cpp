#include "pch.h"

#include "aitest.h"
#include "MockPlayerbotAIBase.h"
#include "../modules/Bots/playerbot/strategy/values/CraftValue.h"

using namespace ai;

class CraftDataTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( CraftDataTestCase );
      CPPUNIT_TEST( empty );
      CPPUNIT_TEST( craft );
      CPPUNIT_TEST( crafted );
  CPPUNIT_TEST_SUITE_END();

protected:

public:
	void setUp()
	{
	}

protected:
    void empty()
    {
        CraftData data;
        CPPUNIT_ASSERT(data.IsEmpty());
    }

	void craft()
	{
	    CraftData data;
	    data.itemId = 1;
	    data.required[2] = 2;
	    data.required[3] = 3;

	    CPPUNIT_ASSERT(!data.IsEmpty());
	    CPPUNIT_ASSERT(!data.IsFulfilled());

	    CPPUNIT_ASSERT(data.IsRequired(2));
	    CPPUNIT_ASSERT(!data.IsRequired(4));
	}

	void crafted()
	{
	    CraftData data;
	    data.itemId = 1;
	    data.required[2] = 2;
	    data.required[3] = 3;

        data.AddObtained(2, 2);
	    CPPUNIT_ASSERT(!data.IsFulfilled());

        data.AddObtained(3, 3);
        CPPUNIT_ASSERT(data.IsFulfilled());

        data.Crafted(1);
        CPPUNIT_ASSERT(!data.IsFulfilled());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( CraftDataTestCase );
