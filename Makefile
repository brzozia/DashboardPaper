# Makefile for testing on PC

CC := gcc
# Enable generation of dependency files so changes to headers force rebuilds
CFLAGS := -g -O2 -Wall -MMD -MP
INCLUDES := -I./src -I./lib -I./lib/Config -I./lib/GUI -I./lib/Fonts

TARGET := test_program

# Source files: project sources and the GUI / Config / Fonts sources used at runtime
SRCS := $(wildcard src/*.c) $(wildcard lib/Config/*.c) $(wildcard lib/GUI/*.c) $(wildcard lib/Fonts/*.c)
OBJS := $(SRCS:.c=.o)
DEPS := $(SRCS:.c=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Generic rule: compile any .c to .o preserving directories
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Include generated dependency files (if present). The leading '-' keeps make
# from failing on the first build when .d files don't exist yet.
-include $(DEPS)

clean:
	rm -f $(OBJS) $(TARGET) $(DEPS)