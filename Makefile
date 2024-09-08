# Compiler and flags
CC = gcc
CFLAGS = -I./luajit/include

# Directories
SRCDIR = src
BINDIR = bin
LUADIR = luajit
LUAJIT_REPO = .luajit-repo
LUAJIT_URL = https://luajit.org/git/luajit.git

# Source and output files
SRC = $(SRCDIR)/main.c
TARGET = $(BINDIR)/lofi

# LuaJIT libraries
LUA_LIBS = -L$(LUADIR)/lib -lluajit
LDFLAGS = -lm -ldl -Wl,-rpath=$(LUADIR)/lib

# Default target: build the program
all: $(TARGET)

# Create bin directory if it doesn't exist
$(BINDIR):
	mkdir -p $(BINDIR)

# Compile and link the program with LuaJIT library
$(TARGET): $(SRC) $(LUADIR)/lib/libluajit.a | $(BINDIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LUA_LIBS) $(LDFLAGS)

# Rebuild: Clean project, clone, build, and install LuaJIT, then build the program
rebuild: clean luajit $(TARGET)

# Clone LuaJIT repository
luajit-repo:
	@if [ -d $(LUAJIT_REPO) ]; then \
    echo "Removing old LuaJIT repository..."; \
    rm -rf $(LUAJIT_REPO); \
	fi
	git clone $(LUAJIT_URL) $(LUAJIT_REPO)

# Build LuaJIT and copy headers and libraries
luajit: luajit-repo
	# Build LuaJIT
	cd $(LUAJIT_REPO) && $(MAKE)
	# Create directories for headers and libraries if they don't exist
	mkdir -p $(LUADIR)/include $(LUADIR)/lib
	# Copy headers
	cp -r $(LUAJIT_REPO)/src/*.h $(LUADIR)/include/
	# Copy the static and shared libraries
	cp $(LUAJIT_REPO)/src/libluajit.a $(LUADIR)/lib/
	cp $(LUAJIT_REPO)/src/libluajit.so $(LUADIR)/lib/
	ln -sf libluajit.so $(LUADIR)/lib/libluajit-5.1.so.2
	# Clean up by removing the LuaJIT repo
	rm -rf $(LUAJIT_REPO)

# Clean build artifacts (binary files) only, keep LuaJIT intact
clean:
	rm -rf $(BINDIR)
	@if [ -d $(LUAJIT_REPO) ]; then \
    echo "Removing old LuaJIT repository..."; \
    rm -rf $(LUAJIT_REPO); \
	fi

# Clean LuaJIT files and project files
distclean: clean
	rm -rf $(LUADIR)/lib/*
	rm -rf $(LUADIR)/include/*
	rm -rf $(LUAJIT_REPO)

# Phony targets
.PHONY: all clean rebuild luajit luajit-repo distclean
