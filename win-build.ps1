param(
    [switch]$clean = $false,
    [switch]$cleanBuild = $false,
    [switch]$norun = $false,
    [string]$compiler = "msvc" ,
    [string]$config = "Release",
    [string]$target = "install",
    [Int16]$parallel = 20,
    [switch]$h = $false
)

if ($h -eq $true) {
    Write-Host "help:"
    Write-Host "  -clean       Remove existing build."
    Write-Host "  -norun       Build but don't run."
    Write-Host "  -compiler    Specify compiler. default msvc. options MingWG ot msvc"
    Write-Host "  -config      Specify build type RELEASE, DEBUG ..."
    Write-Host "  -target      NOT USED. specify target."
    Write-Host "  -parallel    Allow N jobs at once"
    Write-Host "  -h           this help."
    exit 0
}

[string]$BUILD_PATH = "./build"
[string]$INSTALL_PATH = [string](Get-Location) + "\out"

if ($clean -eq $true) {
    if (Test-Path -Path $BUILD_PATH) { 
        Remove-Item $BUILD_PATH -Recurse -Force 
    }
}

if ($cleanBuild -eq $true) {
    if (Test-Path -Path $INSTALL_PATH) { 
        Remove-Item $INSTALL_PATH -Recurse -Force 
    }
}

if ($compiler -eq "msvc") {
    Write-Host $INSTALL_PATH
    cmake -S . -B $BUILD_PATH -G "Visual Studio 17 2022" `
        -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=true `
        -DCMAKE_INSTALL_PREFIX:PATH=$INSTALL_PATH `
        -DBUILD_CPP_DATA_FRAME_EXAMPLES:BOOL=true 
    if ( -not $? ) {
        Write-Error "Cmake configraion failed"
        exit 1
    }
    cmake --build $BUILD_PATH --config $config --target $target --parallel 
}
elseif ($compiler -eq "clang-cl") {
    cmake -S . -B $BUILD_PATH -G  "MinGW Makefiles" `
        -DCMAKE_C_COMPILER=clang `
        -DCMAKE_CXX_COMPILER=clang++ `
        -DCMAKE_BUILD_TYPE:STRING=$CONFIG `
        -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=true `
        -DCMAKE_INSTALL_PREFIX:PATH=$INSTALL_PATH `
        -DBUILD_CPP_DATA_FRAME_EXAMPLES:BOOL=true  
    if ( -not $? ) {
        Write-Error "Cmake configraion failed"
        exit 1
    }
    cmake --build $BUILD_PATH --target $target -j
}

if ( -not $? ) {
    Write-Error "Build failed."
    exit 1
}
else {
    if ($norun -eq $false) {
        .\out\dataframe-example.exe
    }
}
