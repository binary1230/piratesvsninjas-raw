SUBDIRS = tests

bin_PROGRAMS = ninjas

# AM_CXXFLAGS = -Wall

# LDFLAGS = -ggdb -lm
# ninjas_LDFLAGS = @LDFLAGS@

ninjas_CXXFLAGS = -Wall

headers = \
globals.h timer.h \
gameOptions.h gameState.h gameBase.h window.h \
gameMode.h physSimulation.h \
input.h inputLiveHandler.h inputRecord.h inputPlayback.h \
object.h objectFactory.h objectIDs.h \
objectPlayer.h objectRadiusBlock.h objectBackground.h \
vector2D.h force.h forceFactory.h \
forceGravity.h forceInput.h \
platform.h basename.h resourceLoader.h \
animation.h \
StdString.h xmlParser.h

ninjas_SOURCES = \
force.cpp forceGravity.cpp forceInput.cpp forceFactory.cpp \
gameOptions.cpp gameState.cpp main.cpp gameBase.cpp \
gameMode.cpp physSimulation.cpp \
input.cpp inputLiveHandler.cpp inputRecord.cpp inputPlayback.cpp \
object.cpp objectFactory.cpp objectPlayer.cpp objectRadiusBlock.cpp \
objectBackground.cpp \
timer.cpp vector2D.cpp window.cpp \
platform.cpp basename.cpp resourceLoader.cpp \
animation.cpp xmlParser.cpp \
$(headers)

# EXTRA_DIST = dir1 dir2
EXTRA_DIST = Doxyfile Doxyfile.in
