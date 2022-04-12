CXX := g++
TARGET := main
HEADER := -I $(HALIDE_ROOT)/include
HEADERTOOLS := -I $(HALIDE_ROOT)/tools
LIB = -L $(HALIDE_ROOT)/lib
COMPILER_FLAGS = -std=c++17 -fno-rtti
LINKER_FLAGS =  -lHalide -lpthread -ljpeg `libpng-config --cflags --ldflags` -ldl -lz

all: run

compile: $(TARGET).cpp
	$(CXX) $(COMPILER_FLAGS) $(TARGET).cpp -o $(TARGET) $(HEADER) \
	$(HEADERTOOLS) $(LIB) $(LINKER_FLAGS)

run: compile
	./main /home/jtavares/Documents/Github/Halide/images/Australia.jpg \
			results/Australia_brighter.jpg

clean:
	rm $(TARGET) || echo -n ""
	rm results/*
	echo "keep" > results/.gitkeep 