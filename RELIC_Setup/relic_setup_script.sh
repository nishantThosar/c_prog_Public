#!/bin/bash

# Create toolchain file
cat > arm-gcc-toolchain.cmake <<EOF
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os")
set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -Wl,--gc-sections")

set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
EOF

# Create and enter build directory
mkdir -p build
cd build

# Run CMake with correct flags
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../arm-gcc-toolchain.cmake \
  -DRELIC_TARGET=STM32 \
  -DRELIC_ARCH=ARM \
  -DRELIC_CPU=CORTEXM4 \
  -DRELIC_PLATFORM=embedded \
  -DWSIZE=32 \
  -DWITH_EB=ON \
  -DEB_K163=ON \
  -DWITH_ECDH=ON \
  -DWITH_HASH=OFF \
  -DWITH_ED=OFF \
  -DWITH_PP=OFF \
  -DWITH_BN=ON \
  -DDEBUG=OFF

# Build
make -j$(nproc)
