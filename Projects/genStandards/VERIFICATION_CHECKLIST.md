# Mathematical Test Output Verification Checklist

**Purpose:** Prevent calculation errors when generating expected outputs for test cases in mathematical projects.

## 1. Before Generating Expected Outputs - ALWAYS:

- [ ] **Slow down** - Don't rush mathematical calculations
- [ ] **Show work** - Write out each step explicitly  
- [ ] **Double-check signs** - Especially with complex numbers, negative numbers, trigonometry
- [ ] **Verify identities** - (e.g., i² = -1, cos(-x) = cos(x), sin(-x) = -sin(x))
- [ ] **Use systematic methods** - FOIL for complex multiplication, step-by-step for matrices
- [ ] **Check units** - Radians vs degrees, ensure dimensional consistency
- [ ] **Verify each term** - Don't skip intermediate steps

## 2. Create Verification Documentation Template:

```markdown
# [PROJECT_NAME] - Expected Output Calculations

## Test Case: [test_name]
**Date:** [DATE]
**Input:** [specific inputs with exact values]
**Formula:** [mathematical formula being implemented]

**Step-by-step calculation:**
1. Step 1: [show work with intermediate results]
2. Step 2: [show work with intermediate results]  
3. Step 3: [show work with intermediate results]
4. Final result: [answer with full precision]

**Verification methods used:**
- [ ] Manual calculation (shown above)
- [ ] Alternative calculation method
- [ ] External tool verification: [tool_name]
- [ ] Cross-reference with literature/textbook

**Confidence level:** [High/Medium/Low]
```

## 3. Generate Verification Script Template:

```python
#!/usr/bin/env python3
"""
Verification script for [PROJECT_NAME]
Generated on: [DATE]
Purpose: Verify expected outputs using standard libraries
"""

import numpy as np
import cmath
import math

def verify_[operation_name]():
    """
    Verify expected outputs for [operation_name]
    Returns True if all tests pass, False otherwise
    """
    print(f"Verifying [operation_name]...")
    
    # Test case 1: [description]
    inputs = [specific_test_inputs]
    expected = [calculated_result]
    
    # Calculate using standard library
    actual = [library_calculation]
    
    # Check with tolerance for floating point
    tolerance = 1e-10
    if abs(expected - actual) < tolerance:
        print(f"✓ Test 1 passed: Expected {expected}, Got {actual}")
        return True
    else:
        print(f"✗ Test 1 failed: Expected {expected}, Got {actual}")
        return False

def verify_all_tests():
    """Run all verification tests"""
    tests = [
        verify_[operation_name],
        # Add more test functions here
    ]
    
    results = [test() for test in tests]
    passed = sum(results)
    total = len(results)
    
    print(f"\nResults: {passed}/{total} tests passed")
    return all(results)

if __name__ == "__main__":
    success = verify_all_tests()
    exit(0 if success else 1)
```

## 4. Makefile Comment Template:

```makefile
# Expected outputs verified on [DATE]
# Verification methods:
# 1. Manual calculation documented in docs/calculations.md
# 2. Python verification script: scripts/verify_outputs.py  
# 3. Cross-checked with [external_tool/library/reference]
# Confidence: [High/Medium/Low]

generate-output:
	@echo "Generating mathematically verified expected outputs..."
	# Part 1: [Brief description] - Expected: [result]
	@echo "[calculated_result]" > $(OUTPUTDIR)/[test_name].txt
```

## 5. Domain-Specific Reminders:

### Complex Numbers:
- Always use FOIL: (a+bi)(c+di) = ac + adi + bci + bdi²
- Remember: i² = -1, i³ = -i, i⁴ = 1
- Complex conjugate: conj(a+bi) = a-bi
- Magnitude: |a+bi| = √(a² + b²)

### Trigonometry:
- Verify angle units (radians vs degrees)
- Key identities: sin²θ + cos²θ = 1, e^(iθ) = cos(θ) + i·sin(θ)
- Check signs for negative angles: sin(-θ) = -sin(θ), cos(-θ) = cos(θ)

### Linear Algebra:
- Verify matrix dimensions before operations
- Transpose operations: (AB)ᵀ = BᵀAᵀ
- Inner product: u·v = Σ uᵢv̄ᵢ (with conjugate for complex vectors)

### Floating Point:
- Use appropriate tolerance for equality (typically 1e-10 to 1e-15)
- Be aware of accumulated rounding errors
- Consider relative vs absolute error for very large/small numbers

### Roots of Unity:
- N-th root of unity: e^(2πik/N) for k = 0,1,...,N-1
- Sum of all N-th roots of unity = 0 (except for N=1)
- Geometric series: Σ r^k = (1-r^N)/(1-r) for r≠1

### DSP/Signal Processing:
- **Distinguish source vs filter parameters**: Source frequency (buzz/oscillator) ≠ filter center frequency
  - Source: Creates harmonics at fundamental and multiples (f, 2f, 3f, ...)
  - Filter: Emphasizes specific frequency band from those harmonics
  - Example: 110 Hz buzz creates harmonics up to Nyquist; 730 Hz reson emphasizes the 730 Hz region
- **Sample rate considerations**: Maximum frequency = sample_rate / 2 (Nyquist limit)
- **Period calculation**: period_samples = sample_rate / frequency
- **Coefficient calculations**: Verify angle units (radians), exponential arguments, trigonometric identities

## 6. Emergency Recovery Process:

If errors are discovered in expected outputs:

1. **Stop** - Don't guess or rush corrections
2. **Document** - Record what was wrong and why
3. **Recalculate** - Follow this checklist from the beginning
4. **Verify** - Use at least two independent methods
5. **Update** - Fix all affected test files and documentation
6. **Test** - Run the actual program to confirm fixes

## 7. Conversation Logging for AI Collaboration:

### Purpose:
Maintain continuity across long conversations and multiple sessions with AI assistants who have limited memory.

### Log Structure Template:
```markdown
# [PROJECT_NAME]_CONVERSATION_LOG.md

## Session [DATE] (AI Instance: [model_name])

### [TIME] [Task Description]
- **Action:** [What was accomplished]
- **Key Decisions:** [Important choices made and rationale]
- **Issues Found:** [Problems discovered]
- **Resolution:** [How issues were resolved]
- **Files Modified:** [List of changed files]
- **Status:** [Current state: Complete/In Progress/Blocked]

### [TIME] Current Status Summary
- **Completed:** [List of finished components]
- **In Progress:** [Current work items]
- **Next Steps:** [What needs to be done next]
- **Blockers:** [Any impediments]
```

### Logging Guidelines:
- **Log at natural breakpoints** (after completing major tasks)
- **Include rationale** for design decisions  
- **Document issues and their resolutions** for future reference
- **Update status regularly** to track progress
- **Note file modifications** to understand change history

### Benefits:
- **Continuity across sessions** - New AI instances can understand project history
- **Avoid rework** - Track what's been completed and decided
- **Design decision history** - Remember why certain approaches were chosen
- **Debugging aid** - Reference past issues and solutions

## 8. Quality Assurance Questions:

Before committing expected outputs, ask:
- [ ] Did I show my work step-by-step?
- [ ] Did I verify using a second method?
- [ ] Are the inputs and outputs clearly documented?
- [ ] Did I check edge cases and sign changes?
- [ ] Would another person be able to reproduce my calculation?
- [ ] Have I updated the conversation log with key decisions?

---

**Remember:** It's better to spend extra time getting expected outputs right than to debug incorrect test cases later. Keep a running log to maintain continuity across AI collaboration sessions.