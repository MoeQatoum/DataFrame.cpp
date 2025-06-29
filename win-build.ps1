param(
    [switch]$clean = $false,
    [switch]$cleanBuild = $false,
    [switch]$norun = $false,
    [switch]$buildExamples = $false,
    [switch]$test = $false,
    [switch]$bench = $false,
    [switch]$updateSubmodules = $false,
    [switch]$qtImpl = $false,
    [string]$QTDIR = "",
    [string]$COMPILER = "msvc" ,
    [string]$config = "Release",
    [string]$target = "dataframe-example",
    [Int16]$parallel = 20,
    [switch]$h = $false
)

if ($h -eq $true) {
    Write-Host "help:"
    Write-Host "  -clean       Remove existing build."
    Write-Host "  -norun       Build but don't run."
    Write-Host "  -compiler    Specify COMPILER. default msvc. options MingWG ot msvc"
    Write-Host "  -config      Specify build type RELEASE, DEBUG ..."
    Write-Host "  -target      NOT USED. specify target."
    Write-Host "  -parallel    Allow N jobs at once"
    Write-Host "  -h           this help."
    exit 0
}

[string]$BUILD_PATH = "./build"
[string]$INSTALL_PREFIX = [string](Get-Location) + "\out"
[string]$CONFIG = [string]$config
[bool]$BUILD_EXAMPLES = !$buildExamples
[bool]$BUILD_TESTS = $test
[bool]$BUILD_BENCH_MARK = $bench
[bool]$UPDATE_SUBMODULES = $updateSubmodules
[bool]$USE_QT_IMPLEMENTATION = $qtImpl

if ($clean -eq $true) {
    if (Test-Path -Path $BUILD_PATH) {
        Remove-Item $BUILD_PATH -Recurse -Force
    }
}

if ($cleanBuild -eq $true) {
    if (Test-Path -Path $INSTALL_PREFIX) {
        Remove-Item $INSTALL_PREFIX -Recurse -Force
    }
}

if ($qtImpl -eq $true) {
    if ($QTDIR -eq "") {
        if ($COMPILER -eq "msvc" ) {
            $QTDIR = "C:\Qt\6.4.22msvc2019_64"
        }
        elseif ($COMPILER -eq "clang") {
            $QTDIR = "C:\Qt\6.4.2\mingw_64"
        }
    }

    if (-not (Test-Path -Path $QTDIR)) {
        Write-Error "QTDIR: $QTDIR is invalid."
        exit 1
    }
    $env:QTDIR = $QTDIR
    $env:path = $ORGIN_PATH + ";" + "$QTDIR\bin" + ";" + "$QTDIR\lib"
}

if ($BUILD_EXAMPLES -eq $true) {
    $target = "dataframe-example"
}
elseif ($BUILD_TESTS -eq $true) {
    $target = "dataframe-test"
}
elseif ($BUILD_TESTS -eq $true) {
    $target = "dataframe-bench"
}


if ($COMPILER -eq "msvc") {
    Write-Host $INSTALL_PREFIX
    cmake -S . -B $BUILD_PATH -G "Visual Studio 17 2022" -T "host=x64" -A x64 `
        -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE `
        -DCMAKE_INSTALL_PREFIX:PATH=$INSTALL_PREFIX `
        -DBUILD_CPP_DATA_FRAME_EXAMPLES:BOOL=$BUILD_EXAMPLES
    if ( -not $? ) {
        Write-Error "Cmake configraion failed"
        exit 1
    }
    cmake --build $BUILD_PATH --config $CONFIG --target $target --parallel -v
}
elseif ($COMPILER -eq "clang") {
    cmake -S . -B $BUILD_PATH -G "Unix Makefiles" `
        "-DCMAKE_C_COMPILER:FILEPATH=C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\Llvm\x64\bin\clang.exe" `
        "-DCMAKE_CXX_COMPILER:FILEPATH=C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\Llvm\x64\bin\clang.exe" `
        -DDF_BUILD_EXAMPLES:BOOL=$BUILD_EXAMPLES `
        -DDF_BUILD_BENCH_MARKS:BOOL=$BUILD_BENCH_MARK `
        -DDF_BUILD_TESTS:BOOL=$BUILD_TESTS `
        -DDF_UPDATE_SUBMODULES:BOOL=$UPDATE_SUBMODULES `
        -DDF_QT_IMPLEMENTATION:BOOL=$USE_QT_IMPLEMENTATION `
        -DCMAKE_BUILD_TYPE:STRING=$CONFIG `
        -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=$true
    if ( -not $? ) {
        Write-Error "Cmake configraion failed"
        exit 1
    }
    cmake --build $BUILD_PATH --config $CONFIG --target $target -j
}

if ( -not $? ) {
    Write-Error "Build failed."
    exit 1
}
else {
    if ($norun -eq $false) {
        .$INSTALL_PREFIX\dataframe-example.exe
    }
}
