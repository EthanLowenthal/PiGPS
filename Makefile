# g++ $(pkg-config --cflags --libs cairomm-1.0 libgps) -std=c++17 GPS.cpp FrameBuffer.cpp main.cpp -o main -lcairomm-1.0 -lgps

CXX = g++

TARGET = main
OUTPUT = ./dist/main

FLAGS = -std=c++17 -Wall

PKGS = cairomm-1.0 libgps
LIBS = cairomm-1.0 gps


LIB_FLAGS := $(addprefix -l,$(LIBS))

# The final build step.
$(TARGET):
	$(CXX) $$(pkg-config --cflags --libs $(PKGS)) $(FLAGS) $(SRCS) -o $(OUTPUT) $(LIB_FLAGS)



.PHONY: clean
clean:
	rm -r $(BUILD_DIR)