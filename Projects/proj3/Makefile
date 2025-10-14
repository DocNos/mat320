# Project: MAT320 Project 3 - Fast Fourier Transform
# Version: 1.0.0
# Created: 2025-10-13
# Purpose: Non-recursive FFT implementation with bit reversal

################################################################################
# CONFIGURATION
################################################################################

# Directory structure (following genStandards naming conventions)
BINDIR = bin
OBJDIR = obj
INPUTDIR = input
OUTPUTDIR = output
TESTDIR = tests
DOCDIR = docs
SCRIPTDIR = scripts

# Build configuration
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
DEBUGFLAGS = -g -O0 -DDEBUG

# Primary targets
TARGET_BITREV = bitrev
TARGET_FFT2 = fft2
TARGET_TIMING = timing_test

# Source files
BITREV_SOURCES = bitrev.cpp
FFT2_SOURCES = fft2.cpp
TIMING_SOURCES = timing_test.cpp

# Object files
BITREV_OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(BITREV_SOURCES))
FFT2_OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(FFT2_SOURCES))
TIMING_OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(TIMING_SOURCES))

################################################################################
# BUILD RULES
################################################################################

.PHONY: all build clean distclean help test test-bitrev test-fft2 \
        generate-inputs generate-outputs generate-tests debug \
        verify verify-bitrev verify-fft check docs

# Default target - build all executables
all: $(BINDIR)/$(TARGET_BITREV) $(BINDIR)/$(TARGET_FFT2) $(BINDIR)/$(TARGET_TIMING)

# Alternative name for all
build: all

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean all

# Compile object files
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Link bitrev executable
$(BINDIR)/$(TARGET_BITREV): $(BITREV_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Link fft2 executable
$(BINDIR)/$(TARGET_FFT2): $(FFT2_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Link timing test executable
$(BINDIR)/$(TARGET_TIMING): $(TIMING_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

################################################################################
# TEST TARGETS
################################################################################

# Run all tests
test: test-bitrev test-fft2

# Test bit reversal with various sizes
test-bitrev: $(BINDIR)/$(TARGET_BITREV)
	@echo "Testing bit reversal..."
	@echo "N=8:"
	@./$(BINDIR)/$(TARGET_BITREV) 8
	@echo ""
	@echo "N=16:"
	@./$(BINDIR)/$(TARGET_BITREV) 16

# Test FFT with sample inputs
test-fft2: $(BINDIR)/$(TARGET_FFT2)
	@echo "Testing FFT implementation..."
	@if [ -f $(INPUTDIR)/sample_input.txt ]; then \
		./$(BINDIR)/$(TARGET_FFT2) 8 $(INPUTDIR)/sample_input.txt; \
	else \
		echo "No input file found. Run 'make generate-inputs' first."; \
	fi

# Run timing comparisons
test-timing: $(BINDIR)/$(TARGET_TIMING)
	@echo "Running timing tests (N=1024)..."
	@./$(BINDIR)/$(TARGET_TIMING) > timing.txt
	@echo "Results written to timing.txt"
	@cat timing.txt

################################################################################
# GENERATION TARGETS
################################################################################

# Generate test input files
generate-inputs:
	@echo "Generating test input files..."
	@mkdir -p $(INPUTDIR)
	@python3 $(SCRIPTDIR)/generate_inputs.py

# Generate expected output files using Python reference implementations
generate-outputs:
	@echo "Generating expected output files using NumPy reference..."
	@mkdir -p $(OUTPUTDIR)
	@python3 $(SCRIPTDIR)/generate_inputs.py outputs
	@echo "Expected outputs generated in $(OUTPUTDIR)/"
	@echo ""
	@echo "⚠️  IMPORTANT: These outputs were generated using NumPy FFT reference."
	@echo "   Review using: make verify-bitrev and make verify-fft"

# Generate both inputs and outputs
generate-tests: generate-inputs generate-outputs

################################################################################
# VERIFICATION TARGETS
################################################################################

# Verify bit reversal implementation
verify-bitrev:
	@echo "Running bit reversal verification..."
	@python3 $(SCRIPTDIR)/verify_bitrev.py

# Verify FFT implementation (requires numpy)
verify-fft:
	@echo "Running FFT verification..."
	@python3 $(SCRIPTDIR)/verify_fft.py

# Run all verification tests
verify: verify-bitrev verify-fft
	@echo ""
	@echo "✓ All verification tests complete"

# Static analysis and validation
check: all
	@echo "Running validation checks..."
	@echo "Checking that executables exist and are executable..."
	@test -x $(BINDIR)/$(TARGET_BITREV) && echo "✓ bitrev executable OK" || echo "✗ bitrev missing"
	@test -x $(BINDIR)/$(TARGET_FFT2) && echo "✓ fft2 executable OK" || echo "✗ fft2 missing"
	@test -x $(BINDIR)/$(TARGET_TIMING) && echo "✓ timing_test executable OK" || echo "✗ timing_test missing"

################################################################################
# CLEANUP TARGETS
################################################################################

# Remove build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(OBJDIR)/*.o
	@rm -rf $(BINDIR)/$(TARGET_BITREV) $(BINDIR)/$(TARGET_FFT2) $(BINDIR)/$(TARGET_TIMING)
	@echo "Clean complete."

# Full cleanup including generated files
distclean: clean
	@echo "Performing full cleanup..."
	@rm -rf $(BINDIR) $(OBJDIR)
	@rm -f timing.txt
	@echo "Distclean complete."

# Reset to pristine state
mrproper: distclean
	@echo "Resetting to pristine state..."
	@rm -rf $(OUTPUTDIR)/*.txt $(INPUTDIR)/*.txt
	@echo "Project reset complete."

################################################################################
# DOCUMENTATION TARGETS
################################################################################

# Generate documentation
docs:
	@echo "Documentation files:"
	@echo "  - Specification: specDoc.txt"
	@echo "  - Standards: ../genStandards/"
	@echo "  - Project PDF: proj3.pdf"
	@echo "  - Bit Reversal PDF: BitRev.pdf"

################################################################################
# HELP TARGET
################################################################################

help:
	@echo "MAT320 Project 3 - Fast Fourier Transform"
	@echo "=========================================="
	@echo ""
	@echo "Build Targets:"
	@echo "  make / make all     - Build all executables (bitrev, fft2, timing_test)"
	@echo "  make build          - Same as 'make all'"
	@echo "  make debug          - Build with debug symbols"
	@echo "  make clean          - Remove build artifacts"
	@echo "  make distclean      - Full cleanup including binaries"
	@echo "  make mrproper       - Reset to pristine state"
	@echo ""
	@echo "Test Targets:"
	@echo "  make test           - Run all tests"
	@echo "  make test-bitrev    - Test bit reversal with N=8,16"
	@echo "  make test-fft2      - Test FFT implementation"
	@echo "  make test-timing    - Run timing comparison tests"
	@echo ""
	@echo "Generation Targets:"
	@echo "  make generate-inputs   - Create test input files"
	@echo "  make generate-outputs  - Generate expected outputs"
	@echo "  make generate-tests    - Generate both inputs and outputs"
	@echo ""
	@echo "Verification Targets:"
	@echo "  make verify-bitrev  - Verify bit reversal math"
	@echo "  make verify-fft     - Verify FFT math (requires numpy)"
	@echo "  make verify         - Run all verification tests"
	@echo "  make check          - Validate build outputs"
	@echo ""
	@echo "Documentation:"
	@echo "  make docs           - Show documentation resources"
	@echo "  make help           - Show this help message"
	@echo ""
	@echo "Usage Examples:"
	@echo "  ./$(BINDIR)/$(TARGET_BITREV) 16              - Generate bit reversal for N=16"
	@echo "  ./$(BINDIR)/$(TARGET_FFT2) 8 input.txt       - Compute FFT for 8 samples"
	@echo "  ./$(BINDIR)/$(TARGET_TIMING)                 - Run timing tests (N=1024)"
	@echo ""
	@echo "Standards Compliance:"
	@echo "  - Follows genStandards/NAMING_STANDARDS.md"
	@echo "  - Use genStandards/VERIFICATION_CHECKLIST.md for test validation"
	@echo ""
	@echo "IMPORTANT: Always verify generated outputs using the verification checklist!"

# Default help if no target specified
.DEFAULT_GOAL := help