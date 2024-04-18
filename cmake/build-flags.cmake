
set(STD_LIB_CPP_FLAGS -stdlib=libc++)
set(STD_LIB_CXX_LINKER_FLAGS -lc++ -nostdlib++ -lc++abi)

set(LIB_DF_REL_BUILD_FLAGS -O3 -DNDEBUG -s)
set(LIB_DF_REL_WARN_FLAGS -Wall -Wextra -Wreorder-ctor -Wpedantic -Wdouble-promotion)

set(LIB_DF_DBG_BUILD_FLAGS -O0 -ggdb3)
set(LIB_DF_DBG_WARN_FLAGS -Wall -Wextra -Wreorder-ctor -Wconversion -Wpedantic -Wdouble-promotion -Wswitch-enum)
