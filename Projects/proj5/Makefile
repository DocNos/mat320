# Project: Plucked String Filter (proj5)
# Version: 1.0.0
# Purpose: Generate WAV files using Karplus-Strong algorithm

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

# Primary executable name
TARGET = pluck

# File collections
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
HEADERS = $(wildcard $(INCDIR)/*.h) $(wildcard $(SRCDIR)/*.h)

# Build tools
CXX = g++
AR = ar
RM = rm -f

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(SRCDIR) -I$(INCDIR)
LDFLAGS = -lm
ARFLAGS = rcs

# Build mode flags
DEBUGFLAGS = -g -O0 -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG

################################################################################
# BUILD RULES
################################################################################

.DEFAULT_GOAL := all

# Default target
all: $(BINDIR)/$(TARGET)

# Release build (optimized)
release: CXXFLAGS += $(RELEASEFLAGS)
release: clean all

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean all

# Linking
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR) $(OUTPUTDIR) $(INPUTDIR) $(TESTDIR) $(DOCDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation pattern
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Directory creation
$(BINDIR) $(OBJDIR) $(OUTPUTDIR) $(INPUTDIR) $(TESTDIR) $(DOCDIR):
	@mkdir -p $@

################################################################################
# CLEAN TARGETS
################################################################################

# Remove build artifacts
clean:
	$(RM) -r $(OBJDIR)
	$(RM) -r $(BINDIR)

# Full cleanup including generated files
distclean: clean
	$(RM) -r $(OUTPUTDIR)/*.wav

# Nuclear option - reset to pristine state
mrproper: distclean
	$(RM) -r $(OBJDIR) $(BINDIR)

################################################################################
# TEST TARGETS
################################################################################

# Run test with 220 Hz
test: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) 220

# Run test with 440 Hz (A4)
test-440: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) 440

# Run test with 261.63 Hz (C4 - middle C)
test-c: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) 261.63

################################################################################
# UTILITY TARGETS
################################################################################

# Display help
help:
	@echo "$(TARGET) - Plucked String Filter Build System"
	@echo "=============================================="
	@echo ""
	@echo "Build Targets:"
	@echo "  make / make all     - Build the executable"
	@echo "  make release        - Optimized production build"
	@echo "  make debug          - Debug build with symbols"
	@echo "  make clean          - Remove build artifacts"
	@echo "  make distclean      - Complete cleanup including WAV files"
	@echo ""
	@echo "Test Targets:"
	@echo "  make test           - Run with 220 Hz"
	@echo "  make test-440       - Run with 440 Hz (A4)"
	@echo "  make test-c         - Run with 261.63 Hz (C4)"
	@echo ""
	@echo "Usage:"
	@echo "  ./$(BINDIR)/$(TARGET) <frequency>"
	@echo ""
	@echo "Example:"
	@echo "  ./$(BINDIR)/$(TARGET) 440"

# Show build configuration
info:
	@echo "Source files: $(SOURCES)"
	@echo "Object files: $(OBJECTS)"
	@echo "Headers: $(HEADERS)"
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"

################################################################################
# PHONY TARGETS
################################################################################

.PHONY: all release debug clean distclean mrproper test test-440 test-c help info
