# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB_RECURSE sources_cppunit cppunit/*.cpp cppunit/*.h)
source_group("CppUnit" FILES ${sources_cppunit})
file(GLOB_RECURSE sources_efficiency efficiency/*.cpp efficiency/*.h)
source_group("Efficency" FILES ${sources_efficiency})
file(GLOB_RECURSE sources_main *.cpp *.h)
source_group("Main" FILES ${sources_main})

# Create game-libary

if (PCH)
  set(tests_STAT_PCH_HDR pch.h)
  set(tests_STAT_PCH_SRC pch.cpp)
endif ()

set(tests_STAT_SRCS
  ${tests_STAT_SRCS}
  ${sources_cppunit}
  ${sources_efficiency}
  ${sources_main}
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/game
    ${CMAKE_SOURCE_DIR}/src/game/AI
    ${CMAKE_SOURCE_DIR}/src/game/Accounts
    ${CMAKE_SOURCE_DIR}/src/game/Addons
    ${CMAKE_SOURCE_DIR}/src/game/Arena
    ${CMAKE_SOURCE_DIR}/src/game/AuctionHouse
    ${CMAKE_SOURCE_DIR}/src/game/BattleGround
    ${CMAKE_SOURCE_DIR}/src/game/Chat
    ${CMAKE_SOURCE_DIR}/src/game/ChatCommands
    ${CMAKE_SOURCE_DIR}/src/game/Combat
    ${CMAKE_SOURCE_DIR}/src/game/DBScripts
    ${CMAKE_SOURCE_DIR}/src/game/Entities
    ${CMAKE_SOURCE_DIR}/src/game/GMTickets
    ${CMAKE_SOURCE_DIR}/src/game/GameEvents
    ${CMAKE_SOURCE_DIR}/src/game/Globals
    ${CMAKE_SOURCE_DIR}/src/game/Grids
    ${CMAKE_SOURCE_DIR}/src/game/Groups
    ${CMAKE_SOURCE_DIR}/src/game/Guilds
    ${CMAKE_SOURCE_DIR}/src/game/LFG
    ${CMAKE_SOURCE_DIR}/src/game/Loot
    ${CMAKE_SOURCE_DIR}/src/game/Mails
    ${CMAKE_SOURCE_DIR}/src/game/Maps
    ${CMAKE_SOURCE_DIR}/src/game/MotionGenerators
    ${CMAKE_SOURCE_DIR}/src/game/Movement
    ${CMAKE_SOURCE_DIR}/src/game/Object
    ${CMAKE_SOURCE_DIR}/src/game/OutdoorPvP
    ${CMAKE_SOURCE_DIR}/src/game/Pools
    ${CMAKE_SOURCE_DIR}/src/game/Quests
    ${CMAKE_SOURCE_DIR}/src/game/References
    ${CMAKE_SOURCE_DIR}/src/game/Reputation
    ${CMAKE_SOURCE_DIR}/src/game/Server
    ${CMAKE_SOURCE_DIR}/src/game/Server
    ${CMAKE_SOURCE_DIR}/src/game/Skills
    ${CMAKE_SOURCE_DIR}/src/game/Social
    ${CMAKE_SOURCE_DIR}/src/game/Spells
    ${CMAKE_SOURCE_DIR}/src/game/Tools
    ${CMAKE_SOURCE_DIR}/src/game/Trade
    ${CMAKE_SOURCE_DIR}/src/game/VoiceChat
    ${CMAKE_SOURCE_DIR}/src/game/Warden
    ${CMAKE_SOURCE_DIR}/src/game/Weather
    ${CMAKE_SOURCE_DIR}/src/game/World
    ${CMAKE_SOURCE_DIR}/src/game/WorldHandlers
    ${CMAKE_SOURCE_DIR}/src/game/movement
    ${CMAKE_SOURCE_DIR}/src/game/vmap
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/src/shared/Auth
    ${CMAKE_SOURCE_DIR}/src/shared/Config
    ${CMAKE_SOURCE_DIR}/src/shared/Common
    ${CMAKE_SOURCE_DIR}/src/shared/Database
    ${CMAKE_SOURCE_DIR}/src/shared/DataStores
    ${CMAKE_SOURCE_DIR}/src/shared/Network
    ${CMAKE_SOURCE_DIR}/src/shared/Log
    ${CMAKE_SOURCE_DIR}/src/shared/Threading
    ${CMAKE_SOURCE_DIR}/src/shared/Utilities
    ${CMAKE_SOURCE_DIR}/src/framework
    ${CMAKE_SOURCE_DIR}/src/framework/Platform
    ${CMAKE_SOURCE_DIR}/dep/g3dlite
    ${ACE_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${MYSQL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/src/test
    ${CMAKE_SOURCE_DIR}/src/test/cppunit
    ${CMAKE_SOURCE_DIR}/src/modules/Bots/playerbot
    ${CMAKE_SOURCE_DIR}/src/modules/Bots/ahbot
)

# Define ENABLE_PLAYERBOTS if need
if (BUILD_PLAYERBOT)
  add_definitions(-DENABLE_PLAYERBOTS)
endif()

# Define ENABLE_IMMERSIVE if need
if (BUILD_IMMERSIVE)
  add_definitions(-DENABLE_IMMERSIVE)
endif()

add_executable(tests
  ${tests_STAT_SRCS}
  ${tests_STAT_PCH_SRC}
)

if (NOT BUILD_PLAYERBOT)
if(NOT ACE_USE_EXTERNAL)
    add_dependencies(tests ace)
    target_link_libraries(tests ace)
else()
    target_link_libraries(tests ACE)
endif()
endif()

target_link_libraries(tests
  game
  shared
  framework
  g3dlite
  ${OPENSSL_LIBRARIES}
  ${MYSQL_LIBRARY}
)

add_dependencies(tests game)

# Generate precompiled header
if (PCH)
 if (ADD_CXX_PCH)
  add_cxx_pch(tests ${tests_STAT_PCH_HDR} ${tests_STAT_PCH_SRC})
 elseif (MSVC)
    set(PCHBaseName "pchdef")
    set(PrecompiledBinary "${CMAKE_SOURCE_DIR}/pch.pch")
    set(PrecompiledHeader "${CMAKE_SOURCE_DIR}/pch.h")
    set(PrecompiledSource "${CMAKE_SOURCE_DIR}/pch.cpp")
    set_target_properties(${LIBRARY_NAME} PROPERTIES COMPILE_FLAGS " /Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\" /bigobj")
    set_source_files_properties(${PrecompiledSource} PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\"")
    set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "Test")
  elseif(NOT MINGW)
    # TODO: Resolve issues with order of includes before enabling Cotire for MinGW builds
    cotire(${LIBRARY_NAME})
  endif()
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()
