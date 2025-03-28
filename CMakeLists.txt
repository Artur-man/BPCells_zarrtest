cmake_minimum_required(VERSION 3.27)
Project(test)

# At the top (after project declaration)
include(GNUInstallDirs)

# fetch
include(FetchContent)
FetchContent_Declare(
  tensorstore
  URL "https://github.com/google/tensorstore/archive/refs/tags/v0.1.62.tar.gz"
)
message(STATUS "TensorStore build dir is: ${tensorstore_BINARY_DIR}")

# Additional FetchContent_Declare calls as needed...
FetchContent_MakeAvailable(tensorstore)

# Optionally force static libraries if supported
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries")

# Define a target that depends on TensorStore...
add_executable(my_target src/main.cpp)
target_link_libraries(
  my_target
  PRIVATE
    tensorstore::tensorstore 
    tensorstore::driver_zarr
    tensorstore::driver_zarr3 
    tensorstore::kvstore_file
    tensorstore::index_space_dim_expression
)

# Define install rules for the static lib
install(TARGETS my_target
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Automatically find and install all .a files built by TensorStore
file(GLOB TENSORSTORE_STATIC_LIBS
     "${tensorstore_BINARY_DIR}/libtensorstore_*.a")

# Manually install static .a files
install(FILES ${TENSORSTORE_STATIC_LIBS}
        DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Install headers from your source directory
install(DIRECTORY ${tensorstore_SOURCE_DIR}/tensorstore
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${tensorstore_SOURCE_DIR}/tensorstore/driver/zarr
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${tensorstore_SOURCE_DIR}/tensorstore/driver/zarr3
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${tensorstore_SOURCE_DIR}/tensorstore/kvstore/file
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${tensorstore_SOURCE_DIR}/tensorstore/index_space/dim_expression/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")