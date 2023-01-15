param(
    [switch]$clean = $false,
    [switch]$cleanBuild = $false,
    [switch]$norun = $false,
    [switch]$buildExamples = $false,
    [string]$COMPILER = "msvc" ,
    [string]$config = "Release",
    [string]$target = "install",
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
        -DCMAKE_BUILD_TYPE:STRING=$CONFIG `
        -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE `
        -DCMAKE_INSTALL_PREFIX:FILEPATH=$INSTALL_PREFIX `
        -DBUILD_CPP_DATA_FRAME_EXAMPLES:BOOL=$BUILD_EXAMPLES  
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
