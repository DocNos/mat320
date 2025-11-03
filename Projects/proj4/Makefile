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

# Input file to use for tests
TESTFILE = transrights.wav

# Basic test
test: all
	@echo "Running test with $(TESTFILE)..."
	@echo "Command: ./$(BINDIR)/$(TARGET) 0.02 100 $(TESTFILE)"
	./$(BINDIR)/$(TARGET) 0.2 100 $(TESTFILE)
	@echo ""
	@echo "Output written to output.wav"
	@if [ -f output.wav ]; then \
		echo "✓ output.wav created successfully"; \
		file output.wav; \
	else \
		echo "✗ output.wav not found"; \
	fi

# Test with weak filtering (small coefficient, few iterations)
test-weak: all
	@echo "Testing weak lowpass filter..."
	@echo "a1=0.01, n=10"
	./$(BINDIR)/$(TARGET) 0.01 10 $(TESTFILE)
	@mv output.wav output_weak.wav
	@echo "✓ Created output_weak.wav"

# Test with moderate filtering
test-moderate: all
	@echo "Testing moderate lowpass filter..."
	@echo "a1=0.5, n=50"
	./$(BINDIR)/$(TARGET) 0.5 50 $(TESTFILE)
	@mv output.wav output_moderate.wav
	@echo "✓ Created output_moderate.wav"

# Test with strong filtering (large coefficient, many iterations)
test-strong: all
	@echo "Testing strong lowpass filter..."
	@echo "a1=0.99, n=100"
	./$(BINDIR)/$(TARGET) 0.99 100 $(TESTFILE)
	@mv output.wav output_strong.wav
	@echo "✓ Created output_strong.wav"

# Test single iteration
test-single: all
	@echo "Testing single iteration..."
	@echo "a1=0.5, n=1"
	./$(BINDIR)/$(TARGET) 0.5 1 $(TESTFILE)
	@mv output.wav output_single.wav
	@echo "✓ Created output_single.wav"

# Test heavy iterations
test-heavy: all
	@echo "Testing heavy iterations..."
	@echo "a1=0.9, n=500"
	./$(BINDIR)/$(TARGET) 0.9 500 $(TESTFILE)
	@mv output.wav output_heavy.wav
	@echo "✓ Created output_heavy.wav"

# Test range of coefficients with fixed iterations
test-coeff-range: all
	@echo "Testing coefficient range (0.1 to 0.9 with n=50)..."
	@mkdir -p test_outputs
	@for a in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9; do \
		echo "Testing a1=$$a, n=50"; \
		./$(BINDIR)/$(TARGET) $$a 50 $(TESTFILE); \
		mv output.wav test_outputs/output_a$${a}_n50.wav; \
	done
	@echo "✓ Created 9 test files in test_outputs/"

# Test range of iterations with fixed coefficient
test-iter-range: all
	@echo "Testing iteration range (1, 10, 50, 100, 500 with a1=0.5)..."
	@mkdir -p test_outputs
	@for n in 1 10 50 100 500; do \
		echo "Testing a1=0.5, n=$$n"; \
		./$(BINDIR)/$(TARGET) 0.5 $$n $(TESTFILE); \
		mv output.wav test_outputs/output_a0.5_n$$n.wav; \
	done
	@echo "✓ Created 5 test files in test_outputs/"

# Full test matrix (small set)
test-matrix: all
	@echo "Running test matrix..."
	@mkdir -p test_outputs
	@for a in 0.1 0.5 0.9; do \
		for n in 1 10 100; do \
			echo "Testing a1=$$a, n=$$n"; \
			./$(BINDIR)/$(TARGET) $$a $$n $(TESTFILE); \
			mv output.wav test_outputs/output_a$${a}_n$$n.wav; \
		done; \
	done
	@echo "✓ Created 9 test files in test_outputs/"

# Run all individual tests
test-all: test-weak test-moderate test-strong test-single test-heavy
	@echo ""
	@echo "All tests completed!"
	@ls -lh output_*.wav

valgrind: debug
	@echo "Running valgrind memory check..."
	@echo "Command: valgrind --leak-check=full ./$(BINDIR)/$(TARGET) 0.02 10 $(TESTFILE)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		./$(BINDIR)/$(TARGET) 0.02 10 $(TESTFILE)
	@echo ""
	@echo "Check output above for memory leaks and errors"

################################################################################
# CLEANUP
################################################################################

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/$(TARGET) output.wav ./out/*

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
	@echo "  make test           - Basic test (a1=0.2, n=100)"
	@echo "  make test-weak      - Weak filtering (a1=0.01, n=10)"
	@echo "  make test-moderate  - Moderate filtering (a1=0.5, n=50)"
	@echo "  make test-strong    - Strong filtering (a1=0.99, n=100)"
	@echo "  make test-single    - Single iteration (a1=0.5, n=1)"
	@echo "  make test-heavy     - Heavy iterations (a1=0.9, n=500)"
	@echo "  make test-all       - Run all individual tests"
	@echo "  make test-coeff-range   - Test coefficient range 0.1-0.9"
	@echo "  make test-iter-range    - Test iteration range 1-500"
	@echo "  make test-matrix        - Full test matrix (3x3)"
	@echo "  make valgrind       - Run valgrind memory check"
	@echo ""
	@echo "Usage:"
	@echo "  ./$(BINDIR)/$(TARGET) <a1> <n> <input.wav>"
	@echo ""
	@echo "Example:"
	@echo "  ./$(BINDIR)/$(TARGET) 0.02 100 input/test.wav"

.PHONY: all debug release test test-weak test-moderate test-strong test-single \
        test-heavy test-coeff-range test-iter-range test-matrix test-all \
        valgrind clean distclean help