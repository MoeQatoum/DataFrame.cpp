#!/bin/bash

#colors
W="\033[0;00m"
G="\033[0;92m"
R="\033[0;91m"
Y="\033[0;93m"

num_re='^[0-9]+$'

BUILD_DIR="./build"
BUILD_EXAMPLES=true
CONFIG=RELEASE
RUN=true
JOBS="-j"
TARGET=
CLEAN=false
BENCH=false
COMPILER=clang++
C_COMPILER=clang

INSTALL_PREFIX=$("pwd")/out

opts=("$@")
for ((i = 0; i < $#; i++)); do
  case "${opts[$i]}" in
  --clean)
    CLEAN=true
    ;;
  --include-examples)
    BUILD_EXAMPLES=true
    ;;
  --no-run)
    RUN=false
    ;;
  --bench)
    BENCH=true
    ;;
  --prefix)
    INSTALL_PREFIX=${opts[$((i + 1))]}
    ((i++))
    ;;
  --config)
    CONFIG=${opts[$((i + 1))]}
    ((i++))
    ;;
  --compiler)
    COMPILER=${opts[$((i + 1))]}
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
    printf "  --clean       Remove existing build folder.\n"
    printf "  --no-run      Build but don't run the app.\n"
    printf "  --bench       Build benchmarks.\n"
    printf "  --prefix      Specify install prefix.\n"
    printf "  --config      Specify build type. Options RELEASE, DEBUG ...\n"
    printf "  --compiler    Specify compiler. Default clang\n"
    printf "  --target      NOT USED. specify target.\n"
    printf "  -j            Allow N jobs at once\n"
    printf "  -h            this help.\n"
    exit 0
    ;;
  *)
    printf "\"${opts[$((i))]}\" is invalid option, -h for help.\n"
    exit 1
    ;;
  esac
done

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
  -DDATA_FRAME_EXAMPLES:BOOL=$BUILD_EXAMPLES \
  -DCMAKE_BUILD_TYPE:STRING=$CONFIG \
  -DCMAKE_C_COMPILER=$C_COMPILER \
  -DCMAKE_CXX_COMPILER=$COMPILER \
  -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
  -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
  -DBUILD_BENCH_MARKS:BOOL=$BENCH

if [[ $? -eq 1 ]]; then
  printf "${R}-- Cmake failed${W}\n" &&
    exit 1
fi

cmake --build $BUILD_DIR --target install $JOBS
if [[ $? -eq 0 ]]; then
  printf "${G}-- Build successful.${W}\n"
  if [[ $RUN = true && $BUILD_EXAMPLES = true ]]; then
    printf "${G}-- Running Application.${W}\n\n"
    cd $INSTALL_PREFIX && ./dataframe-example
    if [[ $? -eq 1 ]]; then
      printf "${R}-- run failed.${W}\n"
      exit 1
    fi
  fi
  exit 0
else
  printf "${R}-- Build failed.${W}\n" &&
    exit 1
fi
