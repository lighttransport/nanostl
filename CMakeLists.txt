cmake_minimum_required(VERSION 3.13)

set(NANOSTL_TARGET nanostl)

project(${NANOSTL_TARGET} CXX)

set(NANOSTL_SOURCES
  src/nanothread.cc
  src/hash.cc
  )

if (WIN32)
  # Win32 thread should be available
  find_package(Threads REQUIRED)
endif ()

# static
add_library(${NANOSTL_TARGET} STATIC ${NANOSTL_SOURCES})

target_include_directories(${NANOSTL_TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Disable standard stl
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(${NANOSTL_TARGET} PRIVATE "-nostdinc++")
endif ()

if (MSVC)
  target_compile_options(${NANOSTL_TARGET} PRIVATE "/NODEFAULTLIB")
endif ()

if (WIN32)
  target_compile_definitions(${NANOSTL_TARGET} PRIVATE "NOMINMAX")
  target_link_libraries(${NANOSTL_TARGET} PUBLIC Threads::Threads)
endif ()

