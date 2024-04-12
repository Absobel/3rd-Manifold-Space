# Compiler settings - Can change according to need
CC := gcc
CFLAGS := -Wall -Wextra -g
LIBRARIES := -lglfw -lGL -lGLEW

# Project structure
APP_NAME := 3rd-manifold-space
SRCDIR := src
TARGETDIR := target
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(TARGETDIR)/%.o)
TARGET := $(TARGETDIR)/$(APP_NAME)

# Default target
all: $(TARGET)

# Ensure target directory exists
$(TARGETDIR):
	mkdir -p $(TARGETDIR)

# Linking the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBRARIES) $^ -o $@

# Compiling source files into object files
$(TARGETDIR)/%.o: $(SRCDIR)/%.c | $(TARGETDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
.PHONY: clean run
clean:
	rm -rf $(TARGETDIR)

run: $(TARGET)
	./$(TARGET)