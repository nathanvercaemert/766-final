cmake_minimum_required(VERSION 3.13.4)

project(InputAnalysisPass)

# Set the path to your LLVM build
set(LLVM_DIR "/root/llvm-project/build/lib/cmake/llvm")

find_package(LLVM REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

# Set the MLIR source and build directories
set(MLIR_SOURCE_DIR "/root/llvm-project/mlir")
set(MLIR_BINARY_DIR "/root/llvm-project/build")

include_directories(
    ${LLVM_INCLUDE_DIRS}
    ${MLIR_SOURCE_DIR}/include
    ${MLIR_BINARY_DIR}/include
)

link_directories(${LLVM_LIBRARY_DIRS})

add_definitions(${LLVM_DEFINITIONS})

add_executable(input-analysis-pass InputAnalysisPass.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(input-analysis-pass PRIVATE ${llvm_libs})
