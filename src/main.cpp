#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tensorstore/context.h"
#include "tensorstore/index.h"
#include "tensorstore/open.h"
#include "tensorstore/open_mode.h"
#include <tensorstore/tensorstore.h>
#include <tensorstore/util/status.h>
#include <typeinfo>
#include <cstdlib> // for getenv
#include <string>

using ::tensorstore::Context;
::nlohmann::json GetZarr3JsonSpec()
{
  return {
      {"driver", "zarr3"},
      {"kvstore", {{"driver", "file"}, {"path", "/tmp/testo/zarrV3"}}},
      {"metadata",
       {{"data_type", "int16"},
        {"shape", {100, 100}},
        {"chunk_grid",
         {{"name", "regular"},
          {"configuration", {{"chunk_shape", {10, 10}}}}}}}},
  };
}

::nlohmann::json GetZarr2JsonSpec()
{
  return {
      {"driver", "zarr"},
      {"kvstore", {{"driver", "file"}, {"path", "/tmp/testo/zarrV2"}}},
      {"metadata",
       {//{"compressor", nullptr},
        {"dtype", "<i2"},
        // {"fill_value", nullptr},
        // {"filters", nullptr},
        {"order", "C"},
        {"shape", {100, 100}},
        {"chunks", {10, 10}},
        {"zarr_format", 2}}},
  };
}

::nlohmann::json GetJsonSpec()
{
  // Get environment variable, default to "2" if not set
  const char *zarr_version = std::getenv("ZARR_VERSION");
  std::string version = zarr_version ? zarr_version : "2";

  if (version == "3")
  {
    return GetZarr3JsonSpec();
  }
  else
  {
    return GetZarr2JsonSpec();
  }
}

// try to create a zarr and write a 100*100 random array in it
int main() {
  auto context = Context::Default();
  auto store_result = tensorstore::Open(GetJsonSpec(), context,
                                        tensorstore::OpenMode::create |
                                            tensorstore::OpenMode::open,
                                        tensorstore::ReadWriteMode::read_write)
                          .result();
  if (!store_result.ok()) {
    std::cerr << "Error creating Zarr file: " << store_result.status() << std::endl;
    return -1;
  }
  std::cout << "Zarr file created successfully!" << std::endl;

  srand(time(0));
  auto array = tensorstore::AllocateArray<int16_t>({100, 100});
  for (tensorstore::Index i = 0; i < 100; ++i) {
    for (tensorstore::Index j = 0; j < 100; ++j) {
      array(i, j) = (rand() % 10) + 1;
    }
  }

  auto write_result = tensorstore::Write(array, store_result).status();
  if (!write_result.ok()) {
    std::cerr << "Error creating Zarr file: " << write_result << std::endl;
    return -1;
  }

  std::cout << "Zarr file wrote successfully!" << std::endl;
  return 0;
}