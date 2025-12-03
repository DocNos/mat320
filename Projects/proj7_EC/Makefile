# Project: Spectrum Shaping (proj7_EC)
# Version: 1.0.0
# Purpose: Generate spectrum-shaped audio using reson filters and digital buzz signal

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

# Primary executable name
TARGET = spectrum_shaper

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
# TEST TARGETS WITH PRESETS
################################################################################

# Quick test - single combination
test-quick: $(BINDIR)/$(TARGET)
	@echo "Running quick test: full110 buzz + vowel_a reson..."
	./$(BINDIR)/$(TARGET) full110 vowel_a

# Test all vowel formants with full buzz
test-vowels: $(BINDIR)/$(TARGET)
	@echo "Generating vowel sounds..."
	./$(BINDIR)/$(TARGET) full110 vowel_a
	./$(BINDIR)/$(TARGET) full110 vowel_e
	./$(BINDIR)/$(TARGET) full110 vowel_i
	./$(BINDIR)/$(TARGET) full110 vowel_o
	./$(BINDIR)/$(TARGET) full110 vowel_u

# Test different buzz richness with narrow filter
test-buzz-variants: $(BINDIR)/$(TARGET)
	@echo "Testing buzz harmonic variations..."
	./$(BINDIR)/$(TARGET) minDull narrow_440
	./$(BINDIR)/$(TARGET) midDull narrow_440
	./$(BINDIR)/$(TARGET) maxDull narrow_440
	./$(BINDIR)/$(TARGET) full110 narrow_440

# Test bandwidth variations
test-bandwidth: $(BINDIR)/$(TARGET)
	@echo "Testing bandwidth effects..."
	./$(BINDIR)/$(TARGET) full110 narrow_440
	./$(BINDIR)/$(TARGET) full110 vowel_a
	./$(BINDIR)/$(TARGET) full110 wide_1000

# Test different buzz frequencies
test-frequencies: $(BINDIR)/$(TARGET)
	@echo "Testing different buzz frequencies..."
	./$(BINDIR)/$(TARGET) full50 vowel_a
	./$(BINDIR)/$(TARGET) full110 vowel_a
	./$(BINDIR)/$(TARGET) full220 vowel_a

# Comprehensive test suite
test-suite: $(BINDIR)/$(TARGET)
	@echo "Running comprehensive test suite..."
	@echo "==================================="
	@echo "Test 1: Vowel synthesis (full110 buzz)"
	./$(BINDIR)/$(TARGET) full110 vowel_a
	./$(BINDIR)/$(TARGET) full110 vowel_e
	./$(BINDIR)/$(TARGET) full110 vowel_i
	./$(BINDIR)/$(TARGET) full110 vowel_o
	./$(BINDIR)/$(TARGET) full110 vowel_u
	@echo ""
	@echo "Test 2: Harmonic richness comparison"
	./$(BINDIR)/$(TARGET) minDull narrow_440
	./$(BINDIR)/$(TARGET) full110 narrow_440
	@echo ""
	@echo "Test 3: Bandwidth effects"
	./$(BINDIR)/$(TARGET) full110 narrow_440
	./$(BINDIR)/$(TARGET) full110 wide_1000
	@echo ""
	@echo "Test 4: Frequency variations"
	./$(BINDIR)/$(TARGET) full50 vowel_a
	./$(BINDIR)/$(TARGET) full110 vowel_a
	./$(BINDIR)/$(TARGET) full220 vowel_a
	@echo ""
	@echo "All tests complete! Check output/ directory"

# Play all generated files
play-all:
	@if command -v aplay >/dev/null 2>&1; then \
		for file in $(OUTPUTDIR)/*.wav; do \
			echo "Playing $$file..."; \
			aplay $$file; \
		done; \
	elif command -v afplay >/dev/null 2>&1; then \
		for file in $(OUTPUTDIR)/*.wav; do \
			echo "Playing $$file..."; \
			afplay $$file; \
		done; \
	else \
		echo "No audio player found"; \
	fi

# List generated files
list-output:
	@echo "Generated WAV files:"
	@ls -lh $(OUTPUTDIR)/*.wav 2>/dev/null || echo "No WAV files found"

################################################################################
# UTILITY TARGETS
################################################################################

# Display help
help:
	@echo "$(TARGET) - Spectrum Shaping Build System"
	@echo "=========================================="
	@echo ""
	@echo "Build Targets:"
	@echo "  make / make all     - Build the executable"
	@echo "  make release        - Optimized production build"
	@echo "  make debug          - Debug build with symbols"
	@echo "  make clean          - Remove build artifacts"
	@echo "  make distclean      - Complete cleanup including WAV files"
	@echo ""
	@echo "Quick Test Targets:"
	@echo "  make test-quick     - Single test (full110 + vowel_a)"
	@echo "  make test-vowels    - Test all vowel formants"
	@echo ""
	@echo "Comprehensive Test Targets:"
	@echo "  make test-suite     - Run complete test suite"
	@echo "  make test-buzz-variants - Test harmonic richness"
	@echo "  make test-bandwidth     - Test bandwidth effects"
	@echo "  make test-frequencies   - Test different buzz frequencies"
	@echo ""
	@echo "Playback:"
	@echo "  make play-all       - Play all generated WAV files"
	@echo "  make list-output    - List generated files"
	@echo ""
	@echo "Usage:"
	@echo "  ./$(BINDIR)/$(TARGET) <buzz_preset> <reson_preset>"
	@echo ""
	@echo "Buzz Presets: minDull, midDull, maxDull, full110, full220, full50"
	@echo "Reson Presets: vowel_a, vowel_e, vowel_i, vowel_o, vowel_u,"
	@echo "               narrow_440, wide_1000"
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

.PHONY: all release debug clean distclean mrproper test-quick test-vowels \
        test-buzz-variants test-bandwidth test-frequencies test-suite \
        play-all list-output help info