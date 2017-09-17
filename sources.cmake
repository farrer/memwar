# This file is the list of sources needed for dnt, dntMapEditor and 
# dntPartEditor executables

########################################################################
# Files related to the core engine
########################################################################
set(GAME_SOURCES
src/game/initial.cpp
src/game/level.cpp
src/game/mblock.cpp
src/game/options.cpp
src/game/pair.cpp
src/game/pause.cpp
src/game/selector.cpp
src/game/score.cpp
src/game/world.cpp
src/game/worldselector.cpp
)
set(GAME_HEADERS
src/game/initial.h
src/game/level.h
src/game/mblock.h
src/game/options.h
src/game/pair.h
src/game/pause.h
src/game/selector.h
src/game/score.h
src/game/world.h
src/game/worldselector.h
)

set(CHALLENGE_SOURCES
src/challenge/alien.cpp
src/challenge/bomb.cpp
src/challenge/challenge.cpp
src/challenge/challengeinfo.cpp
src/challenge/chrono.cpp
src/challenge/compass.cpp
src/challenge/punk.cpp
src/challenge/thief.cpp
)
set(CHALLENGE_HEADERS
src/challenge/alien.h
src/challenge/bomb.h
src/challenge/challenge.h
src/challenge/challengeinfo.h
src/challenge/chrono.h
src/challenge/compass.h
src/challenge/punk.h
src/challenge/thief.h
)

set(SPECIAL_SOURCES
src/special/ocarina.cpp
src/special/quantum.cpp
src/special/selector.cpp
src/special/special.cpp
src/special/wind.cpp
)
set(SPECIAL_HEADERS
src/special/ocarina.h
src/special/quantum.h
src/special/selector.h
src/special/special.h
src/special/wind.h
)

IF(${APPLE})
   #set(APPLE_CONFIG_SOURCES
   #    src/config_mac.cpp
   #    macosx/SDLMain.m)
   #set(APPLE_CONFIG_HEADERS
   #    macosx/SDLMain.h)
ENDIF(${APPLE})

IF(${WIN32} OR ${MINGW})
   set(WIN_SOURCES
       windows/resource.rc)
ENDIF(${WIN32} OR ${MINGW})

set(MEMWAR_LIB_HEADERS
${GAME_HEADERS}
${CHALLENGE_HEADERS}
${SPECIAL_HEADERS}
${APPLE_CONFIG_HEADERS}
)

set(MEMWAR_LIB_SOURCES
${GAME_SOURCES}
${CHALLENGE_SOURCES}
${SPECIAL_SOURCES}
${APPLE_CONFIG_SOURCES}
)

set(MEMWAR_SOURCES
src/game/memwar.cpp
${WIN_SOURCES}
)
set(MEMWAR_HEADERS
src/game/memwar.h
)



