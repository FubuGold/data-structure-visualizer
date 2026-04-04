CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -ID:\SFML-3.0.2\include -DSFML_STATIC -static -static-libgcc -static-libstdc++
LDFLAGS = -LD:\SFML-3.0.2\lib
LIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s \
-lopengl32 -lfreetype -ljpeg -lwinmm -lgdi32 -lws2_32 -lopenal -lvorbis \
-lsfml-system -lflac -lvorbisenc -lvorbisfile -lvorbis -logg \
-lcomdlg32 -lole32 -lshell32 -luuid -loleaut32 -limm32 

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

SRC_CPP := $(shell find src/ -type f -name '*.cpp')
LIB_CPP := $(shell find lib/ -type f -name '*.cpp')
LIB_C   := $(shell find lib/ -type f -name '*.c')

OBJ := \
$(SRC_CPP:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/$(SRC_DIR)/%.o) \
$(LIB_CPP:$(LIB_DIR)/%.cpp=$(BUILD_DIR)/$(LIB_DIR)/%.o) \
$(LIB_C:$(LIB_DIR)/%.c=$(BUILD_DIR)/$(LIB_DIR)/%.o)

TARGET := app.exe

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(TARGET)

$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(LIB_DIR)/%.o: $(LIB_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(LIB_DIR)/%.o: $(LIB_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(OBJ) app.exe