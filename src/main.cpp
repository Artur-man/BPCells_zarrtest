#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tensorstore/context.h"
#include "tensorstore/open.h"
#include "tensorstore/open_mode.h"
#include <tensorstore/tensorstore.h>
#include "tensorstore/index.h"
#include <tensorstore/index_space/dim_expression.h>
#include <typeinfo>
#include <cstdlib> // for getenv
#include <string>
#include <utility>
#include <vector>
#include <chrono>

// namespaces
using ::tensorstore::Context;
using namespace std;

::nlohmann::json GetZarr3JsonSpec(std::vector<int> shape, std::vector<int> chunk)
{
  return {
      {"driver", "zarr3"},
      {"kvstore", {{"driver", "file"}, {"path", "/tmp/testo/zarrV3"}}},
      {"metadata",
       {{"data_type", "int16"},
        {"shape", shape},
        {"chunk_grid",
         {{"name", "regular"},
          {"configuration", {{"chunk_shape", chunk}}}}}}},
  };
}

::nlohmann::json GetZarr2JsonSpec(std::vector<int> shape, std::vector<int> chunk)
{
  return {
      {"driver", "zarr"},
      {"kvstore", {{"driver", "file"}, {"path", "/tmp/testo/zarrV2"}}},
      {"metadata",
       {
        {"dtype", "<i2"},
        {"order", "C"},
        {"shape", shape},
        {"chunks", chunk},
        {"zarr_format", 2},
        {"compressor", {{"id", "blosc"}, {"shuffle", -1}, {"clevel", 5}, {"cname", "lz4"}}}
       }
      } 
  };
}

::nlohmann::json GetJsonSpec(std::vector<int> shape, std::vector<int> chunk)
{
  // Get environment variable, default to "2" if not set
  const char *zarr_version = std::getenv("ZARR_VERSION");
  std::string version = zarr_version ? zarr_version : "2";

  if (version == "3")
  {
    std::cout << "Using Zarr V3" << std::endl;
    return GetZarr3JsonSpec(shape, chunk);
  }
  else
  {
    std::cout << "Using Zarr V2" << std::endl;
    return GetZarr2JsonSpec(shape, chunk);
  }
}

// try to create a zarr and write a 100 random array in it
int main(int argc, char **argv) {

  // file
  ofstream MyFile("test.txt");

  // context
  auto context = Context::Default();

  // get arguements for executable: ./build/my_target <n shape> <chunk size> <n append size>
  if (argc > 4) {
    std::cerr << "You cannot have more than three arguments: <n shape> <chunk size> <n append size>" << std::endl;
    return -1;
  }
  std::vector<int> shape = {atoi(argv[1])};
  std::vector<int> chunk = {atoi(argv[2])};
  int add_size = atoi(argv[3]);
  std::cerr << shape[0] << " " << chunk[0] << " " << add_size << std::endl;

  // test create array
  auto store_result = tensorstore::Open(GetJsonSpec(shape, chunk), context,
                                        tensorstore::OpenMode::create |
                                            tensorstore::OpenMode::open,
                                        tensorstore::ReadWriteMode::read_write)
                          .result();
  if (!store_result.ok()) {
    std::cerr << "Error creating Zarr file: " << store_result.status() << std::endl;
    return -1;
  }
  std::cout << "Zarr file created successfully!" << std::endl;

  // test generate array
  srand(time(0));
  auto array = tensorstore::AllocateArray<int16_t>({shape[0]});
  for (tensorstore::Index i = 0; i < shape[0]; ++i) {
    array(i) = (rand() % 10) + 1;
  }
  auto write_result = tensorstore::Write(array, store_result).status();
  if (!write_result.ok()) {
    std::cerr << "Error creating Zarr file: " << write_result << std::endl;
    return -1;
  }
  std::cout << "Zarr file wrote successfully!" << std::endl;

  // read data
  auto read_test = tensorstore::Read(store_result).value();
  std::cout << read_test << std::endl;

  // create new array
  auto new_array = tensorstore::AllocateArray<int16_t>({add_size});
  for (tensorstore::Index i = 0; i < add_size; ++i) {
    new_array(i) = (rand() % 10) + 1;
  }

  // begin time
  using clock = std::chrono::system_clock;
  using sec = std::chrono::duration<double>;
  const auto before = clock::now();

  // test change array size
  auto change_result = tensorstore::Resize(store_result,
    tensorstore::span<const tensorstore::Index, 1>({tensorstore::kImplicit}),
    tensorstore::span<const tensorstore::Index, 1>({shape[0]+add_size})
  ).status();
  if (!change_result.ok()) {
    std::cerr << "Error changing Zarr file: " << change_result << std::endl;
    return -1;
  }

  // insert new array
  auto interval = tensorstore::Dims(0).SizedInterval(shape[0],add_size);
  auto rewrite_result = tensorstore::Write(new_array, store_result | interval).status();

  // end time
  const sec duration = clock::now() - before;
  std::cout << "It took " << duration.count() << "s" << std::endl;
  if (!rewrite_result.ok()) {
    std::cerr << "Error rewriting Zarr file: " << rewrite_result << std::endl;
    return -1;
  }
  std::cout << "Rewrote Zarr file size successfully!" << std::endl;

  // read array
  read_test = tensorstore::Read(store_result).value();
  std::cout << read_test << std::endl;

  // write to file
  MyFile << shape[0] << " " << chunk[0] << " " << add_size << " " << duration.count() << "\n";
  MyFile.close();

  return 0;
}