# Toolchain file for LLVM 21.1.0-rc1 with C++23 modules support
# This toolchain uses custom-built clang and libc++ with full module support

set(LLVM_ROOT "/Users/tomaskubovcik/dev/thirdparty/llvm-project/build/llvm-21")

# Compilers
set(CMAKE_C_COMPILER "${LLVM_ROOT}/bin/clang")
set(CMAKE_CXX_COMPILER "${LLVM_ROOT}/bin/clang++")

# Tools
set(CMAKE_AR "${LLVM_ROOT}/bin/llvm-ar")
set(CMAKE_RANLIB "${LLVM_ROOT}/bin/llvm-ranlib")
set(CMAKE_NM "${LLVM_ROOT}/bin/llvm-nm")
set(CMAKE_OBJDUMP "${LLVM_ROOT}/bin/llvm-objdump")
set(CMAKE_STRIP "${LLVM_ROOT}/bin/llvm-strip")

# Use libc++ from LLVM build instead of system libstdc++
set(CMAKE_CXX_FLAGS_INIT "-nostdinc++ -isystem ${LLVM_ROOT}/include/c++/v1")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-nostdlib++ -L${LLVM_ROOT}/lib -Wl,-rpath,${LLVM_ROOT}/lib -lc++ -lc++abi")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-nostdlib++ -L${LLVM_ROOT}/lib -Wl,-rpath,${LLVM_ROOT}/lib -lc++ -lc++abi")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-nostdlib++ -L${LLVM_ROOT}/lib -Wl,-rpath,${LLVM_ROOT}/lib -lc++ -lc++abi")

# Enable C++20 module scanning
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# For StdModuleProvider - point to libc++ module sources
set(LIBCXX_MODULE_PATH "${LLVM_ROOT}/modules/c++/v1" CACHE PATH "Path to directory containing std.cppm")
set(LIBCXX_INCLUDE_PATH "${LLVM_ROOT}/include/c++/v1" CACHE PATH "Path to libc++ headers")
