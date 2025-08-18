# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

NanoSTL is a portable, lightweight subset of C++ STL designed for constrained environments including embedded systems, CUDA devices, and JIT compilers. It's a header-only library with selective implementation requirements.

## Build System and Commands

### Testing
```bash
# Build and run unit tests
cd test
make
./tester

# Alternative CMake build
cd test
cmake .
make
./test_nanostl
```

### Single Header Generation
```bash
# Generate consolidated single header file
python scripts/generateSingleHeader.py
# Output: single_include/nanostl.h
```

### CMake Build (Static Library)
```bash
mkdir build && cd build
cmake ..
make
```

## Architecture

### Core Structure
- **include/**: Main header files - each STL component is in nano*.h files
- **src/**: Implementation files for components requiring NANOSTL_IMPLEMENTATION
- **single_include/**: Generated consolidated header file
- **compat/**: Standard STL compatibility layer

### Key Components
- **nanovector.h**: STL vector implementation
- **nanostring.h**: String class with ryu-based float conversion
- **nanoalgorithm.h**: STL algorithms (sort, find, etc.)
- **nanomath.h**: Math functions (non-IEEE754 compliant approximations)
- **nanomap.h**: Associative container
- **nanolimits.h**: Numeric limits

### Header Organization
Each header follows this pattern for single-header generation:
```cpp
#ifndef NANOSTL_*_H_
#define NANOSTL_*_H_
// ... implementation ...
#endif // NANOSTL_*_H_
```

### Implementation Pattern
For functions requiring implementation, use:
```cpp
#define NANOSTL_IMPLEMENTATION
#include <nanostl.h>
```

## Platform Support

### Supported Platforms
- 64-bit architectures only
- Big and little endian
- CUDA device functions
- Windows, Linux, macOS
- C++11 compilers (gcc 4.8+, clang 3.4+)

### Compiler Macros
- `NANOSTL_BIG_ENDIAN`: Set for big endian systems
- `NANOSTL_NO_IO`: Disable I/O operations
- `NANOSTL_USE_EXCEPTION`: Enable exception support
- `NANOSTL_NO_THREAD`: Disable threading features
- `NANOSTL_PSTL`: Enable parallel STL (requires C++17)
- `NANOSTL_IMPLEMENTATION`: Include function implementations

## Important Limitations

### Thread Safety
- **Not thread-safe by default** - applications must handle synchronization
- Use external mutex/locks for concurrent access to containers

### Memory Management
- Returns NULL on allocation failure (no bad_alloc exception)
- No automatic exception handling unless NANOSTL_USE_EXCEPTION is defined

### Standards Compliance
- Not IEEE-754 compliant for math functions
- Limited STL compatibility - check feature matrix in README
- No RTTI or exception support by default

## Development Notes

### Code Style
- Use CUDA qualifiers for device compatibility
- Follow existing naming conventions (nano* prefixes)
- Implement both header and source file versions where needed
- Maintain compatibility with C++11

### Testing
- Unit tests require C++11 compiler
- Test both header-only and compiled modes
- Include CUDA platform testing where applicable