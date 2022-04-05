CXX := g++
TARGET := main
HEADER := -I $(HALIDE_ROOT)/include
HEADERTOOLS := -I $(HALIDE_ROOT)/tools
LIB = -L $(HALIDE_ROOT)/lib
COMPILER_FLAGS = -std=c++17 -fno-rtti
LINKER_FLAGS = -lHalide -lpthread -ljpeg `libpng-config --cflags --ldflags`

all: run

compile: $(TARGET).cpp
	$(CXX) $(COMPILER_FLAGS) $(TARGET).cpp -o $(TARGET) $(HEADER) \
	$(HEADERTOOLS) $(LIB) -lHalide -lpng16 -ljpeg -lpthread -ldl

run: compile
	./main

clean:
	rm $(TARGET) || echo -n ""