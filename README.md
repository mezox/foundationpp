# foundationpp

A modern C++23 module wrapper for Apple's Foundation framework.

foundationpp provides type-safe, idiomatic C++ interfaces to Foundation classes while maintaining full compatibility with Objective-C memory management. It leverages C++20/23 features including modules, concepts, and `std::expected` for a clean, macro-free API.

## Features

- **C++23 Modules** - No headers, clean `import foundationpp;` syntax
- **Type-safe** - Concepts-based API with compile-time checks
- **Modern idioms** - RAII, smart pointers, `std::expected` error handling
- **Zero macros** - All selector/class caching hidden in implementation
- **ABI stable** - Versioned inline namespace (`NS::v1`)

## Requirements

- **Platform**: macOS, iOS, tvOS, watchOS (Apple platforms only)
- **Compiler**: Clang 16+ or Apple Clang 15+ with C++23 support
- **CMake**: 3.28+
- **Standard Library**: libc++ with module support (`std.cppm`)

## Quick Start

```cpp
import foundationpp;

auto main() -> int {
    using namespace NS;
    
    // RAII autorelease pool
    AutoreleasePoolScope pool;
    
    // Create a string
    auto str = TransferPtr(String::string("Hello, foundationpp!"));
    
    // Create an array
    auto array = TransferPtr(Array<String>::arrayWithObject(str.get()));
    
    // Iterate
    for (UInteger i = 0; i < array->count(); ++i) {
        auto* s = array->objectAtIndex(i);
        printf("%s\n", s->UTF8String());
    }
    
    return 0;
}
```

## Building

### Prerequisites

You need a C++ standard library with module support. Options:

1. **Build LLVM/libc++ with modules** (recommended for development)
2. **System libc++ with modules** (if your distro provides it)
3. **CMake 3.30+ experimental** (automatic `import std` support)

### Configure and Build

```bash
# Using a toolchain file (recommended)
cmake --preset debug

# Or manually specify paths
cmake -B build \
    -DLIBCXX_MODULE_PATH=/path/to/llvm/modules/c++/v1 \
    -DLIBCXX_INCLUDE_PATH=/path/to/llvm/include/c++/v1

# Build
cmake --build build
```

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `FOUNDATIONPP_BUILD_SHARED` | `ON` | Build shared library |
| `FOUNDATIONPP_BUILD_STATIC` | `OFF` | Build static library |
| `FOUNDATIONPP_SAFE_MESSAGES` | `OFF` | Assert on missing selectors |
| `FOUNDATIONPP_BUILD_TESTS` | `OFF` | Build test suite |

### Std Module Provider

foundationpp uses `StdModuleProvider.cmake` to locate or build the C++23 `std` module. Configuration options:

| Variable | Description |
|----------|-------------|
| `STD_MODULE_PROVIDER` | `auto`, `cmake_experimental`, `libcxx_manual`, `libcxx_system` |
| `LIBCXX_MODULE_PATH` | Path to directory containing `std.cppm` |
| `LIBCXX_INCLUDE_PATH` | Path to libc++ headers (optional) |

## API Overview

### Namespace

All types are in the `NS` namespace with a versioned inline namespace `v1`:

```cpp
NS::String*           // or NS::v1::String*
NS::Array<NS::String> // templated collections
NS::SharedPtr<T>      // smart pointer
```

### Core Types

```cpp
// Basic types (types.cppm)
NS::Integer          // NSInteger
NS::UInteger         // NSUInteger  
NS::TimeInterval     // double
NS::Range            // {location, length}
NS::StringEncoding   // UTF8, UTF16, etc.
NS::ComparisonResult // OrderedAscending/Same/Descending

// Smart pointer (memory.cppm)
NS::SharedPtr<T>     // Automatic retain/release
NS::RetainPtr(ptr)   // Factory: retains pointer
NS::TransferPtr(ptr) // Factory: takes ownership
```

### Object Model

All Foundation wrappers inherit from `NS::Object`:

```cpp
class Object {
    auto retain() -> Object*;
    auto release() -> void;
    auto autorelease() -> Object*;
    auto hash() const -> UInteger;
    auto isEqual(const Object*) const -> bool;
    auto description() const -> String*;
};
```

CRTP mixins provide typed methods:

```cpp
// Referencing<Derived> - typed retain/release
// Copying<Derived>     - adds copy()
// Locking<Derived>     - adds lock/unlock with Guard
```

### Collections

```cpp
// Array
auto arr = TransferPtr(Array<String>::array());
arr->count();
arr->objectAtIndex(0);
arr->operator[](0);  // subscript

// Dictionary
auto dict = TransferPtr(Dictionary<String, Object>::dictionary());
dict->objectForKey(key);
dict->operator[](key);  // subscript

// Set
auto set = TransferPtr(Set<String>::alloc()->init());
set->allObjects();
```

### Strings

```cpp
auto str = TransferPtr(String::string("Hello"));
str->length();
str->UTF8String();
str->isEqualToString(other);
str->compare(other);
str->toStdString();      // -> std::string
str->toStringView();     // -> std::string_view
```

### Error Handling

Uses `std::expected` for fallible operations:

```cpp
// Result types
NS::Result<T>    // std::expected<T, Error*>
NS::VoidResult   // std::expected<void, Error*>

// Example
auto result = bundle->load();
if (!result) {
    auto* error = result.error();
    printf("Error: %s\n", error->localizedDescription()->UTF8String());
}
```

### Concurrency

```cpp
// Autorelease pool (RAII)
{
    AutoreleasePoolScope pool;
    // autoreleased objects collected here
} // pool drained

// Condition variable
auto cond = TransferPtr(Condition::alloc()->init());
{
    auto guard = cond->scopedLock();  // RAII lock
    cond->wait();
    cond->signal();
}
```

### Notifications

```cpp
auto center = NotificationCenter::defaultCenter();

// Add observer with std::function
auto token = center->addObserver(
    String::string("MyNotification"),
    nullptr,
    [](Notification* note) {
        printf("Received: %s\n", note->name()->UTF8String());
    }
);

// RAII guard for automatic removal
ObserverGuard guard{center, token};
```

### Bundle & Process Info

```cpp
// Bundle
auto bundle = Bundle::mainBundle();
bundle->bundleIdentifier();
bundle->infoDictionary();

// Process
auto info = ProcessInfo::processInfo();
info->processName();
info->environment();
info->operatingSystemVersion();
```

## Module Structure

```
foundationpp (primary interface)
├── :types         - Integer, Range, enums
├── :object        - Object base, CRTP mixins
├── :memory        - SharedPtr
├── :string        - String
├── :collections   - Array, Dictionary, Set
├── :data          - Data, URL
├── :error         - Error, Result types
├── :time          - Date
├── :bundle        - Bundle
├── :process       - ProcessInfo
├── :concurrency   - AutoreleasePool, Condition
└── :notification  - Notification, NotificationCenter
```

Internal partitions (not exported):
- `:detail` - Selector/class caching
- `:runtime` - Message dispatch

## Using as a CMake Subdirectory

```cmake
# Provide std module first
set(LIBCXX_MODULE_PATH "/path/to/modules/c++/v1")
include(path/to/foundationpp/cmake/StdModuleProvider.cmake)

# Add foundationpp
add_subdirectory(foundationpp)

# Link
target_link_libraries(myapp PRIVATE foundationpp)
```

## Safe Message Dispatch

Enable runtime checks for missing selectors:

```bash
cmake -DFOUNDATIONPP_SAFE_MESSAGES=ON ..
```

When enabled, `sendMessage` asserts if the target doesn't respond to the selector. Useful for debugging.

## Project Structure

```
foundationpp/
├── CMakeLists.txt
├── CMakePresets.json
├── conanfile.py
├── cmake/
│   ├── StdModuleProvider.cmake   # Std module auto-detection
│   ├── SymbolExports.cmake       # Shared library exports
│   └── Toolchain-LLVM21.cmake    # Example toolchain
└── modules/
    ├── foundationpp.cppm         # Primary module interface
    ├── detail/
    │   ├── detail.cppm           # Selector/class caching
    │   └── runtime.cppm          # Message dispatch
    ├── types.cppm
    ├── object.cppm
    ├── memory.cppm
    ├── string.cppm
    ├── collections.cppm
    ├── data.cppm
    ├── error.cppm
    ├── time.cppm
    ├── bundle.cppm
    ├── process.cppm
    ├── concurrency.cppm
    ├── notification.cppm
    └── foundationpp.exports      # Symbol export list
```
