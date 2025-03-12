CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude -lsqlite3
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

SRCS = $(SRC_DIR)/database.cc
OBJS = $(SRCS:$(SRC_DIR)/%.cc=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*
