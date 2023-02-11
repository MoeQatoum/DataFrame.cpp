#!/bin/bash

#colors
W="\033[0;00m"
G="\033[0;92m"
R="\033[0;91m"
Y="\033[0;93m"

num_re='^[0-9]+$'

BUILD_DIR="./build"
CONFIG=RELEASE
RUN=true
JOBS="-j"
TARGET=
CLEAN=false
CXX_COMPILER=clang++

BUILD_EXAMPLES=false
BUILD_TESTS=false
BUILD_BENCH_MARK=false

INSTALL_PREFIX=$("pwd")/out

opts=("$@")
for ((i = 0; i < $#; i++)); do
  case "${opts[$i]}" in
  --clean)
    CLEAN=true
    ;;
  --build-examples)
    BUILD_EXAMPLES=true
    TARGET=dataframe-example
    ;;
  --no-run)
    RUN=false
    ;;
  --bench)
    BUILD_BENCH_MARK=true
    TARGET=dataframe-benchmark
    ;;
  --test)
    BUILD_TESTS=true
    TARGET=dataframe-tests
    ;;
  --prefix)
    INSTALL_PREFIX=${opts[$((i + 1))]}
    ((i++))
    ;;
  --config)
    CONFIG=${opts[$((i + 1))]}
    ((i++))
    ;;
  --cpp-compiler)
    CXX_COMPILER=${opts[$((i + 1))]}
    ((i++))
    ;;
  --arch)
    ARCH=${opts[$((i + 1))]}
    ((i++))
    ;;
  --target)
    TARGET=${opts[$((i + 1))]}
    ((i++))
    ;;
  -j)
    if [[ ${opts[$((i + 1))]} =~ $num_re ]]; then
      JOBS="-j ${opts[$((i + 1))]}"
      ((i++))
    else
      JOBS="-j"
    fi
    ;;
  -h)
    printf "help:\n"
    printf "  --clean           Remove existing build folder.\n"
    printf "  --no-run          Build but don't run the app.\n"
    printf "  --build-examples  Build benchmarks.\n"
    printf "  --bench           Build benchmarks.\n"
    printf "  --prefix          Specify install prefix.\n"
    printf "  --config          Specify build type. Options RELEASE, DEBUG ...\n"
    printf "  --cpp-compiler    Specify CXX compiler. Default clang\n"
    printf "  --c-compiler      Specify C compiler. Default clang\n"
    printf "  --target          NOT USED. specify target.\n"
    printf "  -j                Allow N jobs at once\n"
    printf "  -h                this help.\n"
    exit 0
    ;;
  *)
    printf "\"${opts[$((i))]}\" is invalid option, -h for help.\n"
    exit 1
    ;;
  esac
done

if [[ $BUILD_EXAMPLES = false && $BUILD_TESTS = false && $BUILD_BENCH_MARK = false ]]; then
  BUILD_EXAMPLES=true
  TARGET=dataframe-example
fi

if [ $CLEAN == true ]; then
  printf "${Y}-- Removing build folder${W}\n"
  rm -rf $BUILD_DIR
  if [[ $? -eq 1 ]]; then
    printf "${R}-- Clean Error: Build folder not found $BUILD_DIR.${W}\n"
    exit 1
  fi
  rm -rf $INSTALL_PREFIX
  if [[ $? -eq 1 ]]; then
    printf "${R}-- Clean Error: install folder not found $BUILD_DIR.${W}\n"
    exit 1
  fi
fi

cmake -S . -B $BUILD_DIR \
  -DDF_BUILD_EXAMPLES:BOOL=$BUILD_EXAMPLES \
  -DDF_BUILD_BENCH_MARKS:BOOL=$BUILD_BENCH_MARK \
  -DDF_BUILD_TESTS:BOOL=$BUILD_TESTS \
  -DCMAKE_BUILD_TYPE:STRING=$CONFIG \
  -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
  -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
  -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE

if [[ $? -eq 1 ]]; then
  printf "${R}-- Cmake failed${W}\n" &&
    exit 1
fi

cmake --build $BUILD_DIR --target $TARGET $JOBS
if [[ $? -eq 0 ]]; then
  printf "${G}-- Build successful.${W}\n"

  #run example
  if [[ $RUN = true && $BUILD_EXAMPLES = true ]]; then
    printf "${G}-- Running Application.${W}\n\n"
    ./build/example/dataframe-example
    if [[ $? -eq 1 ]]; then
      printf "${R}-- run failed.${W}\n"
      exit 1
    fi
  fi

  #run tests
  if [[ $BUILD_TESTS = true ]]; then
    GTEST_COLOR=1 ctest --test-dir build --output-in-failure -j
    if [[ $? -eq 1 ]]; then
      printf "${R}-- run tests failed.${W}\n"
      exit 1
    fi
  fi

  #run Benchmark
  if [[ $RUN = true && $BUILD_BENCH_MARK = true ]]; then
    printf "${G}-- Running Application.${W}\n\n"
    ./build/benchmark/dataframe-benchmark
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
