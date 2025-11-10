# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = 
TARGET = bin/myshell
SRCDIR = src
OBJDIR = obj
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Install dependencies (for later features)
deps:
	sudo apt update
	sudo apt install -y libreadline-dev

.PHONY: all clean deps
