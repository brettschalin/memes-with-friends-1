PROG := MemesWithFriends

PKG_CONFIG ?= pkg-config

WARNINGS := -Wall -Wextra

LIBS := allegro-5 allegro_font-5 allegro_image-5 allegro_physfs-5 \
	allegro_primitives-5 allegro_ttf-5
CXXFLAGS = -std=c++11 -O2 -flto $(WARNINGS) -MMD -MP \
	   $(shell $(PKG_CONFIG) --cflags $(LIBS))
LDFLAGS = -Wl,--as-needed -O2 -flto
LDLIBS = $(shell $(PKG_CONFIG) --libs $(LIBS)) -lphysfs

OBJ := main.o Card.o GameDisplay.o CardFactory.o PlayerHand.o Player1Hand.o Player2Hand.o GameManager.o Point.o CreditsState.o MenuButton.o MenuState.o GameState.o

all: $(PROG)
$(PROG): $(OBJ)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

debug: all
debug: CXXFLAGS += -ggdb -Og -Werror
debug: LDFLAGS += -ggdb -Og

clean:
	$(RM) $(OBJ) $(OBJ:.o=.d) $(PROG)

-include $(OBJ:.o=.d)

.PHONY: all debug clean
