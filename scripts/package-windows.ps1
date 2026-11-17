$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$dist = Join-Path $root "dist/windows"
New-Item -ItemType Directory -Force -Path $dist | Out-Null

$exe = Join-Path $root "chess.exe"
if (-not (Test-Path $exe)) {
    Write-Error "Build the project first with: make CC=gcc"
    exit 1
}

$runtimeDlls = @(
    "C:\msys64\mingw64\bin\SDL2.dll",
    "C:\msys64\mingw64\bin\SDL2_ttf.dll",
    "C:\msys64\mingw64\bin\libgcc_s_seh-1.dll",
    "C:\msys64\mingw64\bin\libstdc++-6.dll"
)

foreach ($dll in $runtimeDlls) {
    if (Test-Path $dll) {
        Copy-Item $dll $dist -Force
    }
}

Copy-Item $exe $dist -Force
Copy-Item (Join-Path $root "README.md") (Join-Path $dist "README.txt") -Force

Compress-Archive -Path (Join-Path $dist "*") -DestinationPath (Join-Path $dist "chess-windows.zip") -Force

Write-Host "Package created at $((Join-Path $dist 'chess-windows.zip'))"
