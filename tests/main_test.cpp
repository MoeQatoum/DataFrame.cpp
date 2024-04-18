#include <gtest/gtest.h>

#include "column_tests.hpp"
#include "df_tests.hpp"
#include "series_tests.hpp"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
