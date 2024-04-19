#!/bin/bash

#colors
W="\033[0;00m"
G="\033[0;92m"
R="\033[0;91m"
Y="\033[0;93m"

num_re='^[0-9]+$'

ROOT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")")
BUILD_DIR=$ROOT_DIR/.build
INSTALL_PREFIX=$ROOT_DIR/.out

CONFIG=RELEASE
RUN=true
JOBS="-j"
TARGETS="--target install"
CLEAN_BUILD=false
CLEAN_INSTALL=false
CXX_COMPILER=clang++-19
C_COMPILER=clang-19

BUILD_EXAMPLES=false
BUILD_TESTS=false
BUILD_BENCH_MARK=false
UPDATE_SUBMODULES=false

print_help() {
    printf "help:\n"
    printf "  --clean           Remove existing build folder.\n"
    printf "  --clean-build     Remove installed files.\n"
    printf "  --clean-install   Remove installed files.\n"
    printf "  --no-run          Build but don't run the app.\n"
    printf "  --update          pull submodules.\n"
    printf "  --example         Build example.\n"
    printf "  --bench           Build benchmarks.\n"
    printf "  --test            Build tests.\n"
    printf "  --prefix          Specify install prefix.\n"
    printf "  --config          Specify build type. Options RELEASE, DEBUG ...\n"
    printf "  --cxx-compiler    Specify CXX compiler. Default clang\n"
    printf "  --c-compiler      Specify C compiler. Default clang\n"
    printf "  --arch            NOY USED\n"
    printf "  --target          NOT USED. specify target.\n"
    printf "  -j                Allow N jobs at once\n"
    printf "  -h                this help.\n"
}

opts=("$@")
for ((i = 0; i < $#; i++)); do
    case "${opts[$i]}" in
    --clean)
        CLEAN_BUILD=true
        CLEAN_INSTALL=true
        ;;
    --clean-build)
        CLEAN_BUILD=true
        ;;
    --clean-install)
        CLEAN_INSTALL=true
        ;;
    --no-run)
        RUN=false
        ;;
    --update)
        UPDATE_SUBMODULES=true
        ;;
    --example)
        BUILD_EXAMPLES=true
        TARGETS=dataframe-example
        ;;
    --bench)
        BUILD_BENCH_MARK=true
        TARGETS=dataframe-benchmark
        ;;
    --test)
        BUILD_TESTS=true
        TARGETS=tests
        ;;
    --prefix)
        INSTALL_PREFIX=${opts[$((i + 1))]}
        ((i++))
        ;;
    --config)
        CONFIG=${opts[$((i + 1))]}
        ((i++))
        ;;
    --cxx-compiler)
        CXX_COMPILER=${opts[$((i + 1))]}
        ((i++))
        ;;
    --c-compiler)
        C_COMPILER=${opts[$((i + 1))]}
        ((i++))
        ;;
    --arch)
        ARCH=${opts[$((i + 1))]}
        ((i++))
        ;;
    --target)
        TARGETS=${opts[$((i + 1))]}
        ((i++))
        ;;
    -v | --verbose)
        CMAKE_VERBOSE="-v"
        ;;
    -j | --jobs)
        if [[ ${opts[$((i + 1))]} =~ $num_re ]]; then
            JOBS="-j ${opts[$((i + 1))]}"
            ((i++))
        else
            JOBS="-j"
        fi
        ;;
    -h)
        print_help
        exit 0
        ;;
    *)
        printf "\"${opts[$((i))]}\" is invalid option, use -h or --help for help\n" >&2
        exit 1
        ;;
    esac
done

if [[ $BUILD_EXAMPLES = false && $BUILD_TESTS = false && $BUILD_BENCH_MARK = false ]]; then
    BUILD_EXAMPLES=true
    TARGETS=dataframe-example
fi

if [ $CLEAN_BUILD == true ]; then
    printf -- "-- Removing previous build.\n"
    if [[ -d $BUILD_DIR ]]; then
        rm -rf $BUILD_DIR
    else
        printf "\t${Y}Build Destination: ${BOLD}${Y}${BUILD_DIR}${W}${Y} not found\n${W}" >&2
    fi
fi

if [ $CLEAN_INSTALL == true ]; then
    printf -- "-- Removing previous installation.\n"
    if [[ -d $INSTALL_PREFIX ]]; then
        rm -rf $INSTALL_PREFIX
    else
        printf "\t${Y}Install Destination: ${BOLD}${Y}${INSTALL_PREFIX}${W}${Y} not found\n${W}" >&2
    fi
fi

cmake -S . -B $BUILD_DIR \
    -D CMAKE_BUILD_TYPE:STRING=$CONFIG \
    -D CMAKE_CXX_COMPILER=$CXX_COMPILER \
    -D CMAKE_C_COMPILER=$C_COMPILER \
    -D CMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -D CMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
    -D DF_BUILD_EXAMPLES:BOOL=$BUILD_EXAMPLES \
    -D DF_BUILD_BENCH_MARKS:BOOL=$BUILD_BENCH_MARK \
    -D DF_BUILD_TESTS:BOOL=$BUILD_TESTS \
    -D DF_UPDATE_SUBMODULES:BOOL=$UPDATE_SUBMODULES \
    -D DF_INSTALL:BOOL=TRUE

if [[ $? -eq 1 ]]; then
    printf "${R}-- Cmake failed${W}\n" &&
        exit 1
fi

cmake --build $BUILD_DIR --target $TARGETS install $JOBS $CMAKE_VERBOSE
if [[ $? -eq 0 ]]; then
    printf "${G}-- Build successful.${W}\n"

    #run example
    if [[ $RUN = true && $BUILD_EXAMPLES = true ]]; then
        printf "${G}-- Running Application.${W}\n\n"
        ./.build/example/dataframe-example
        if [[ $? -eq 1 ]]; then
            printf "${R}-- run failed.${W}\n"
            exit 1
        fi
    fi

    #run tests
    if [ $BUILD_TESTS = true ]; then
        GTEST_COLOR=1 ./.build/tests/dataframe-tests --output-on-failure
        if [[ $? -eq 1 ]]; then
            printf "${R}-- run tests failed.${W}\n"
            exit 1
        fi
    fi

    #run Benchmark
    if [[ $RUN = true && $BUILD_BENCH_MARK = true ]]; then
        printf "${G}-- Running Application.${W}\n\n"
        ./.build/benchmark/dataframe-benchmark
        if [[ $? -eq 1 ]]; then
            printf "${R}-- run benchmark failed.${W}\n"
            exit 1
        fi
    fi
    exit 0
else
    printf "${R}-- Build failed.${W}\n" &&
        exit 1
fi
