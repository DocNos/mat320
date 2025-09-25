# Project 2 - DFT and FFT Implementation
# MAT 320 - Digital Signal Processing
# Matt Klassen, Instructor
# Nixx Varboncoeur
##### AI Disclosure
# Makefile generated following genStandards template
# Future development may include Claude Code assistance

################################################################################
# CONFIGURATION
################################################################################

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
DEBUGFLAGS = -g -O0 -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG

# Directory Variables (genStandards compliant)
SRCDIR = src
DFT_SRCDIR = $(SRCDIR)/dft
FFT_SRCDIR = $(SRCDIR)/fft
INCDIR = include
BINDIR = bin
OBJDIR = obj
DFT_OBJDIR = $(OBJDIR)/dft
FFT_OBJDIR = $(OBJDIR)/fft
INPUTDIR = input
OUTPUTDIR = output
TESTDIR = tests
DOCDIR = docs

# Build Configuration Variables
DFT_TARGET = dft1
FFT_TARGET = fft1
DFT_SOURCES = $(wildcard $(DFT_SRCDIR)/*.cpp)
FFT_SOURCES = $(wildcard $(FFT_SRCDIR)/*.cpp)
DFT_OBJECTS = $(patsubst $(DFT_SRCDIR)/%.cpp,$(DFT_OBJDIR)/%.o,$(DFT_SOURCES))
FFT_OBJECTS = $(patsubst $(FFT_SRCDIR)/%.cpp,$(FFT_OBJDIR)/%.o,$(FFT_SOURCES))
HEADERS = $(wildcard $(INCDIR)/*.h $(INCDIR)/*.hpp $(DFT_SRCDIR)/*.h $(FFT_SRCDIR)/*.h)

# Build tools
CC = gcc
AR = ar
LD = ld

# Universal flags
LDFLAGS = -lm
ARFLAGS = rcs

################################################################################
# BUILD RULES
################################################################################

# Default target
all: release

# Build both programs
both: directories $(BINDIR)/$(DFT_TARGET) $(BINDIR)/$(FFT_TARGET)

# Release build
release: CXXFLAGS += $(RELEASEFLAGS)
release: both

# Debug build  
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: directories $(BINDIR)/$(DFT_TARGET)_debug $(BINDIR)/$(FFT_TARGET)_debug

# Individual program builds
dft1: directories $(BINDIR)/$(DFT_TARGET)
fft1: directories $(BINDIR)/$(FFT_TARGET)

# Create necessary directories
directories:
	@mkdir -p $(DFT_OBJDIR)
	@mkdir -p $(FFT_OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(INPUTDIR)
	@mkdir -p $(OUTPUTDIR)

# Compilation patterns for each subdirectory
$(DFT_OBJDIR)/%.o: $(DFT_SRCDIR)/%.cpp
	@mkdir -p $(DFT_OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -I$(DFT_SRCDIR) -c -o $@ $<

$(FFT_OBJDIR)/%.o: $(FFT_SRCDIR)/%.cpp
	@mkdir -p $(FFT_OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -I$(FFT_SRCDIR) -c -o $@ $<

# Linking patterns for individual programs
$(BINDIR)/$(DFT_TARGET): $(DFT_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build complete: $@"

$(BINDIR)/$(FFT_TARGET): $(FFT_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build complete: $@"

# Debug linking patterns
$(BINDIR)/$(DFT_TARGET)_debug: $(DFT_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Debug build complete: $@"

$(BINDIR)/$(FFT_TARGET)_debug: $(FFT_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Debug build complete: $@"

################################################################################
# TEST TARGETS
################################################################################

test: test-both

test-dft: $(BINDIR)/$(DFT_TARGET)
	@echo "Testing DFT implementation..."
	./$(BINDIR)/$(DFT_TARGET) $(INPUTDIR)/Input01.txt 8
	./$(BINDIR)/$(DFT_TARGET) $(INPUTDIR)/Input02.txt 8

test-fft: $(BINDIR)/$(FFT_TARGET)
	@echo "Testing FFT implementation..."
	./$(BINDIR)/$(FFT_TARGET) $(INPUTDIR)/Input01.txt 8
	./$(BINDIR)/$(FFT_TARGET) $(INPUTDIR)/Input02.txt 8

test-both: test-dft test-fft
	@echo "Comparing DFT and FFT outputs..."
	@echo "=== Input01.txt Comparison ==="
	@echo "DFT Output:"
	@./$(BINDIR)/$(DFT_TARGET) $(INPUTDIR)/Input01.txt 8
	@echo "FFT Output:"  
	@./$(BINDIR)/$(FFT_TARGET) $(INPUTDIR)/Input01.txt 8
	@echo "=== Input02.txt Comparison ==="
	@echo "DFT Output:"
	@./$(BINDIR)/$(DFT_TARGET) $(INPUTDIR)/Input02.txt 8
	@echo "FFT Output:"
	@./$(BINDIR)/$(FFT_TARGET) $(INPUTDIR)/Input02.txt 8

test-quick: both
	@echo "Running quick smoke tests..."
	@echo "DFT Test:"
	@./$(BINDIR)/$(DFT_TARGET) $(INPUTDIR)/Input01.txt 8 | head -2
	@echo "FFT Test:"
	@./$(BINDIR)/$(FFT_TARGET) $(INPUTDIR)/Input01.txt 8 | head -2

# Note: File generation, analysis, and documentation targets removed
# as they are not needed for this assignment

help:
	@echo "DFT and FFT Implementation"
	@echo "============================="
	@echo ""
	@echo "Build Targets:"
	@echo "  make / make all     - Build both programs (release)"
	@echo "  make both           - Build both dft1 and fft1"
	@echo "  make dft1           - Build DFT program only"
	@echo "  make fft1           - Build FFT program only"
	@echo "  make debug          - Debug build with symbols"
	@echo "  make clean          - Remove build artifacts"
	@echo "  make distclean      - Complete cleanup"
	@echo ""
	@echo "Test Targets:"
	@echo "  make test           - Run all tests"
	@echo "  make test-dft       - Test DFT only"
	@echo "  make test-fft       - Test FFT only"
	@echo "  make test-both      - Test both and compare"
	@echo "  make test-quick     - Fast smoke tests"
	@echo ""
	@echo "Usage Examples:"
	@echo "  ./$(BINDIR)/$(DFT_TARGET) input/Input01.txt 8"
	@echo "  ./$(BINDIR)/$(FFT_TARGET) input/Input01.txt 8"

################################################################################
# CLEANUP TARGETS
################################################################################

clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(DFT_OBJDIR)/*.o
	rm -rf $(FFT_OBJDIR)/*.o
	rm -f $(BINDIR)/$(DFT_TARGET)
	rm -f $(BINDIR)/$(FFT_TARGET)
	rm -f $(BINDIR)/$(DFT_TARGET)_debug
	rm -f $(BINDIR)/$(FFT_TARGET)_debug

distclean: clean
	@echo "Full cleanup..."
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -rf $(OUTPUTDIR)/*

mrproper: distclean
	@echo "Nuclear cleanup - resetting to pristine state..."
	rm -rf $(BUILDDIR)

################################################################################
# PHONY TARGETS
################################################################################

.PHONY: all both release debug clean distclean mrproper \
        test test-dft test-fft test-both test-quick \
        help directories dft1 fft1