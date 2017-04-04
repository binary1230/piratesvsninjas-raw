bin_PROGRAMS = keymania

PRE_CXXCOMPILE = 	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
									$(AM_CPPFLAGS) $(CPPFLAGS) $(PRECOMPILED_CXXFLAGS) $(CXXFLAGS)

headers = \
external/StdString.h external/alogg.h external/aloggdll.h \
external/loadpng.h external/xmlParser.h stdafx.h precompiled.h \
basename.h gameOptions.h gameState.h input.h oggFile.h \
precompiled.h singleton.h sprite.h stdafx.h timer.h vector2D.h window.h globals.h \
assetManager.h gameSound.h keymania.h

keymania_SOURCES = \
basename.cpp gameOptions.cpp gameState.cpp input.cpp \
main.cpp oggFile.cpp timer.cpp vector2D.cpp window.cpp \
external/regpng.cpp external/savepng.cpp external/xmlParser.cpp keymania.cpp \
external/alogg.cpp external/loadpng.cpp assetManager.cpp gameSound.cpp globals.cpp \
$(headers)

VPATH = external

# if we're using precompiled headers...
if PRECOMPILE_ENABLE

BUILT_SOURCES = precompiled.h.gch
CLEANFILES = 		precompiled.h.gch

include ./$(DEPDIR)/precompiled.h.Po

$(DEPDIR)/precompiled.h.Po:
	if test ! -d "$(DEPDIR)"; then mkdir $(DEPDIR); fi
	touch $(DEPDIR)/precompiled.h.Po
	
precompiled.h.gch: precompiled.h
	if $(PRE_CXXCOMPILE) -x c++-header -MT $@ -MD -MP -MF "$(DEPDIR)/precompiled.h.Tpo" \
	-c -o $@ $<; then mv -f "$(DEPDIR)/precompiled.h.Tpo" "$(DEPDIR)/precompiled.h.Po"; \
	else rm -f "$(DEPDIR)/precompiled.h.Tpo"; exit 1; fi;

endif
