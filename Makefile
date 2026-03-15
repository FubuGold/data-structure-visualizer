CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -ID:\SFML-3.0.2\include -DSFML_STATIC -static -static-libgcc -static-libstdc++
LDFLAGS = -LD:\SFML-3.0.2\lib
LIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lopengl32 -lfreetype -ljpeg -lwinmm -lgdi32 -lws2_32 -lopenal -lvorbis -lsfml-system -lflac -lvorbisenc -lvorbisfile -lvorbis -logg

SRC_DIR = src
BUILD_DIR = build
SRC := $(shell find src/ -type f -name '*.cpp') $(shell find lib/ -type f -name '*.cpp')
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET := app.exe

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(OBJ) app.exe