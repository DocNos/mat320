# Project: Plucked String Musical Excerpt (proj6_EC)
# Version: 1.0.0
# Purpose: Generate polyphonic music using Karplus-Strong algorithm

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
TARGET = music_gen

# File collections
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
HEADERS = $(wildcard $(INCDIR)/*.h) $(wildcard $(SRCDIR)/*.h)

# Build tools
CXX = g++
AR = ar
RM = rm -f

# Compiler flags
CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -std=c++17 -I$(SRCDIR) -I$(INCDIR)
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

# Run main program
test: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET)

# Run and play output
test-play: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET)
	@if command -v aplay >/dev/null 2>&1; then \
		aplay $(OUTPUTDIR)/*.wav; \
	elif command -v afplay >/dev/null 2>&1; then \
		afplay $(OUTPUTDIR)/*.wav; \
	else \
		echo "No audio player found (aplay or afplay)"; \
	fi

################################################################################
# UTILITY TARGETS
################################################################################

# Display help
help:
	@echo "$(TARGET) - Plucked String Musical Excerpt Build System"
	@echo "========================================================"
	@echo ""
	@echo "Build Targets:"
	@echo "  make / make all     - Build the executable"
	@echo "  make release        - Optimized production build"
	@echo "  make debug          - Debug build with symbols"
	@echo "  make clean          - Remove build artifacts"
	@echo "  make distclean      - Complete cleanup including WAV files"
	@echo ""
	@echo "Test Targets:"
	@echo "  make test           - Run music generator"
	@echo "  make test-play      - Run and play generated audio"
	@echo ""
	@echo "Usage:"
	@echo "  ./$(BINDIR)/$(TARGET)"
	@echo ""

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

.PHONY: all release debug clean distclean mrproper test test-play help info