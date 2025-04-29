COMPILER = clang++
COMPILER_FLAGS = -std=c++17 -F/Library/Frameworks -framework SDL2 -I/Library/Frameworks/SDL2.framework/Headers -Wl,-rpath,/Library/Frameworks
SRC = main.cpp
TARGET_FILE = main

all: main

main: main.cpp ./*
	$(COMPILER) $(COMPILER_FLAGS) $(SRC) -o $(TARGET_FILE)

clean:
	rm -f $(TARGET_FILE)
