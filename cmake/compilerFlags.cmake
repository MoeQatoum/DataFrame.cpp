
set(DF_TCMALLOC_FLAGS -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
set(DF_TCMALLOC_LINKER_FLAGS -ltcmalloc_minimal)

set(DF_STD_LIB_CXX_FLAGS -stdlib=libc++)
set(DF_STD_LIB_CXX_LINKER_FLAGS -lc++ -nostdlib++) # or -stdlib=libc++)

set(DF_REL_BUILD_FLAGS -O3 -DNDEBUG -s)
set(DF_REL_WARN_FLAGS -Wall -Wextra -Wreorder-ctor -Wpedantic -Wdouble-promotion)

set(DF_DBG_BUILD_FLAGS -O0 -ggdb3)
set(DF_DBG_WARN_FLAGS -Wall -Wextra -Wreorder-ctor -Wconversion -Wpedantic -Wdouble-promotion -Wswitch-enum)
