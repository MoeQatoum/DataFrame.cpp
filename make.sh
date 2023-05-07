#!/bin/bash

#colors
W="\033[0;00m"
G="\033[0;92m"
R="\033[0;91m"
Y="\033[0;93m"

num_re='^[0-9]+$'

BUILD_DIR="./build"
QT_DIR="$HOME/Qt/6.5.0/gcc_64"
CONFIG=RELEASE
RUN=true
JOBS="-j"
TARGET=
CLEAN=false
CXX_COMPILER=clang++

BUILD_EXAMPLES=false
BUILD_TESTS=false
BUILD_BENCH_MARK=false
UPDATE_SUBMODULES=false
USE_QT_IMPLEMENTATION=false

INSTALL_PREFIX=$("pwd")/out

opts=("$@")
for ((i = 0; i < $#; i++)); do
  case "${opts[$i]}" in
  --clean)
    CLEAN=true
    ;;
  --no-run)
    RUN=false
    ;;
  --update)
    UPDATE_SUBMODULES=true
    ;;
  --example)
    BUILD_EXAMPLES=true
    TARGET=dataframe-example
    ;;
  --bench)
    BUILD_BENCH_MARK=true
    TARGET=dataframe-benchmark
    ;;
  --test)
    BUILD_TESTS=true
    TARGET=tests
    ;;
  --qt-impl)
    USE_QT_IMPLEMENTATION=true
    ;;
  --qt-dir)
    QT_DIR=${opts[$((i + 1))]}
    ((i++))
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
    printf "  --example         Build example.\n"
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

if [ -d "$QT_DIR" ]; then
  printf "${G}-- Qt dir found: $QT_DIR${W}\n"
else
  printf "${R}-- Qt dir was not found: $QT_DIR${W}\n"
  exit 1
fi

QTDIR=$QT_DIR cmake -S . -B $BUILD_DIR \
  -DDF_BUILD_EXAMPLES:BOOL=$BUILD_EXAMPLES \
  -DDF_BUILD_BENCH_MARKS:BOOL=$BUILD_BENCH_MARK \
  -DDF_BUILD_TESTS:BOOL=$BUILD_TESTS \
  -DDF_UPDATE_SUBMODULES:BOOL=$UPDATE_SUBMODULES \
  -DDF_QT_IMPLEMENTATION:BOOL=$USE_QT_IMPLEMENTATION \
  -DCMAKE_BUILD_TYPE:STRING=$CONFIG \
  -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
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
    GTEST_COLOR=1 ./build/tests/tests --output-on-failure
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
