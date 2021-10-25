cmake_minimum_required(VERSION 3.13)

set(NANOSTL_TARGET nanostl)

project(${NANOSTL_TARGET} CXX)

set(CMAKE_CXX_STANDARD 11)

set(NANOSTL_SOURCES
  src/nanothread.cc
  )

if (WIN32)
  # Win32 thread should be available
  find_package(Threads REQUIRED)
endif ()

# static
add_library(${NANOSTL_TARGET} STATIC ${NANOSTL_SOURCES})

target_include_directories(${NANOSTL_TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/include)

if (WIN32)
  target_link_libraries(${NANOSTL_TARGET} PUBLIC Threads::Threads)
endif ()

