
set(TCMALLOC_FLAGS -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
set(TCMALLOC_LINKER_FLAGS -ltcmalloc_minimal)

set(STD_LIB_CXX_FLAGS -stdlib=libc++)
set(STD_LIB_CXX_LINKER_FLAGS -lc++ -lc++abi) # -nostdlib++)

set(LIB_DF_REL_BUILD_FLAGS -O3 -DNDEBUG -s)
set(LIB_DF_REL_WARN_FLAGS -Wall -Wextra -Wreorder-ctor -Wpedantic -Wdouble-promotion)

set(LIB_DF_DBG_BUILD_FLAGS -O0 -ggdb3)
set(LIB_DF_DBG_WARN_FLAGS -Wall -Wextra -Wreorder-ctor -Wconversion -Wpedantic -Wdouble-promotion -Wswitch-enum)
