cmake_minimum_required(VERSION 3.5)

project(KB_HIT LANGUAGES CXX)

include(GNUInstallDirs)
# build config
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# install config
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)
set(INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include)

# build
add_subdirectory(src)