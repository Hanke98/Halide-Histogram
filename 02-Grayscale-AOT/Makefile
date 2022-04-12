CXX := g++
TARGET_L := grayscale
TARGET_M := main
HEADER := -I $(HALIDE_ROOT)/include
HEADERTOOLS := -I $(HALIDE_ROOT)/tools
LIB = -L $(HALIDE_ROOT)/lib
COMPILER_FLAGS = -std=c++17 -fno-rtti
LINKER_FLAGS = -lHalide -lpthread -ljpeg `libpng-config --cflags --ldflags` -ldl -lz

all: run

compile_lib: $(TARGET_L).cpp
	$(CXX) $(COMPILER_FLAGS) $(TARGET_L).cpp -o $(TARGET_L) $(HEADER) \
	$(HEADERTOOLS) $(LIB) $(LINKER_FLAGS)
	./$(TARGET_L) && rm $(TARGET_L)

compile_main: compile_lib
	$(CXX) $(COMPILER_FLAGS) $(TARGET_M).cpp $(TARGET_L).a -o $(TARGET_M) $(HEADER) \
	$(HEADERTOOLS) $(LIB) -lHalide -lpng16 -ljpeg -lpthread -ldl $(LINKER_FLAGS)

run: compile_main
	./$(TARGET_M) /home/jtavares/Documents/Github/Halide/images/Australia.jpg 0 \
			results/Australia_brighter0.jpg
	./$(TARGET_M) /home/jtavares/Documents/Github/Halide/images/Australia.jpg 1 \
			results/Australia_brighter1.jpg

clean:
	rm $(TARGET_M)
	rm $(TARGET_L).a $(TARGET_L).h 
	rm results/*
	echo "keep" > results/.gitkeep