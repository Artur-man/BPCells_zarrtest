cmake_minimum_required(VERSION 3.27)
Project(test)
include(FetchContent)

FetchContent_Declare(
  tensorstore
  URL "https://github.com/google/tensorstore/archive/refs/tags/v0.1.62.tar.gz")

# Additional FetchContent_Declare calls as needed...

FetchContent_MakeAvailable(tensorstore)

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
