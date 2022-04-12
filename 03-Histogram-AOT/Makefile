CXX := g++
TARGET_L := histogram
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
	./$(TARGET_M) /home/jtavares/Documents/Github/Halide/images/Japan.jpg 0 results/r.dat
	./$(TARGET_M) /home/jtavares/Documents/Github/Halide/images/Japan.jpg 1 results/g.dat
	./$(TARGET_M) /home/jtavares/Documents/Github/Halide/images/Japan.jpg 2 results/b.dat
	gnuplot -p plot_histograms.p

clean:
	rm $(TARGET_L).a $(TARGET_L).h 
	rm $(TARGET_M)
	rm results/*
	echo "keep" > results/.gitkeep