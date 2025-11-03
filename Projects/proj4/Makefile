# Project: MAT320 Project 4 - Low Pass Filter
# Version: 1.0.0
# Created: 2025-11-01
# Purpose: Implement digital lowpass filter with WAV file I/O

################################################################################
# CONFIGURATION
################################################################################

# Directory Variables
SRCDIR = src
INCDIR = include
BINDIR = bin
OBJDIR = obj
DATADIR = data
INPUTDIR = input
OUTPUTDIR = output
TESTDIR = tests
DOCDIR = docs
SCRIPTDIR = scripts

# Build Configuration
TARGET = lowpass
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Build Tools
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(INCDIR) -I$(SRCDIR)
LDFLAGS = -lm

# Build Mode Flags
DEBUGFLAGS = -g -O0 -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG

################################################################################
# BUILD RULES
################################################################################

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Directory creation
$(BINDIR) $(OBJDIR):
	@mkdir -p $@

################################################################################
# BUILD MODES
################################################################################

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean all

release: CXXFLAGS += $(RELEASEFLAGS)
release: clean all

################################################################################
# TESTING
################################################################################

test: all
	@echo "Running test with transrights.wav..."
	@echo "Command: ./$(BINDIR)/$(TARGET) 0.99 100 transrights.wav"
	./$(BINDIR)/$(TARGET) 0.99 100 transrights.wav
	@echo ""
	@echo "Output written to output.wav"
	@if [ -f output.wav ]; then \
		echo "✓ output.wav created successfully"; \
		file output.wav; \
	else \
		echo "✗ output.wav not found"; \
	fi

################################################################################
# CLEANUP
################################################################################

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/$(TARGET)

distclean: clean
	rm -rf $(BINDIR) $(OBJDIR)

################################################################################
# UTILITY TARGETS
################################################################################

help:
	@echo "$(TARGET) - Low Pass Filter Build System"
	@echo "========================================"
	@echo ""
	@echo "Build Targets:"
	@echo "  make / make all     - Build program"
	@echo "  make release        - Optimized production build"
	@echo "  make debug          - Debug build with symbols"
	@echo "  make clean          - Remove build artifacts"
	@echo "  make distclean      - Complete cleanup"
	@echo ""
	@echo "Test Targets:"
	@echo "  make test           - Run all tests"
	@echo ""
	@echo "Usage:"
	@echo "  ./$(BINDIR)/$(TARGET) <a1> <n> <input.wav>"
	@echo ""
	@echo "Example:"
	@echo "  ./$(BINDIR)/$(TARGET) 0.99 100 input/test.wav"

.PHONY: all debug release test clean distclean help