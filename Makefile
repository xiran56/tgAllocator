CXX = clang++

ARCHIVER = ar

SRC_PREF = ./src/

OBJ_PREF = ./obj/

SOURCES = $(wildcard $(SRC_PREF)*.cpp)

OBJECTS = $(patsubst $(SRC_PREF)%.cpp, $(OBJ_PREF)%.o, $(SOURCES))

CXXFLAGS = -std=c++17 -Werror -Wall -Wpedantic -target x86_64-pc-windows-gnu -g

INCLUDE = -I./include

TARGET = ./lib/libtgallocator.a

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(ARCHIVER) rcs $(TARGET) $(OBJECTS)

$(OBJ_PREF)%.o: $(SRC_PREF)%.cpp
	$(CXX) -c -o $@ $(INCLUDE) $< $(CXXFLAGS)

clean:
	rm -f $(OBJECTS)

init:
	mkdir obj
	mkdir lib
