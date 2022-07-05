# g++ $(pkg-config --cflags --libs cairomm-1.0 libgps) -std=c++17 GPS.cpp FrameBuffer.cpp main.cpp -o main -lcairomm-1.0 -lgps

CXX = g++

TARGET = main
OUTPUT = main
FLAGS = -std=c++17 -Wall

SRC_DIRS = ./src

LIBS = cairomm-1.0 gps
PKGS = cairomm-1.0 libgps

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
LIB_FLAGS := $(addprefix -l,$(LIBS))
PKG_FLAGS = $(shell pkg-config --cflags --libs $(PKGS))

.PHONY: $(TARGET)
$(TARGET):
	$(CXX) $(PKG_FLAGS) $(FLAGS) $(SRCS) -o $(OUTPUT) $(LIB_FLAGS)

# TODO change tty and install cpp libs

install: $(TARGET)
	sudo mv ./$(OUTPUT) /home/pi/pigps

	sudo cp pigps.service /etc/systemd/system/pigps.service
	sudo systemctl daemon-reload

	sudo systemctl enable gpsd
	sudo systemctl start gpsd

	sudo systemctl enable pigps.service
	sudo systemctl start pigps.service


# .PHONY: clean
# clean:
# 	rm -r $(BUILD_DIR)