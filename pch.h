#pragma once

#include "../game/Tools/Language.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "WorldSession.h"
#include "Config.h"
#include "Util.h"
#include "AccountMgr.h"
#include "MapManager.h"
#include "Player.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include <openssl/opensslv.h>
#include <openssl/crypto.h>

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestPath.h>
#include <stdexcept>

#include <stdio.h>
#include <iostream>

#include <list>
#include "../framework/Platform/Define.h"

#include <stdexcept>

#include "../../modules/Bots/playerbot/playerbotDefs.h"
