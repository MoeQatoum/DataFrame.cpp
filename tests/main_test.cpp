#include <gtest/gtest.h>

#include "df_tests.h"
#include "series_tests.h"

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}