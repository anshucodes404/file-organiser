$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ScriptDir

# Build the binary.
if (Get-Command mingw32-make -ErrorAction SilentlyContinue -and (Test-Path "Makefile")) {
  mingw32-make
} elseif (Get-Command make -ErrorAction SilentlyContinue -and (Test-Path "Makefile")) {
  make
} elseif (Get-Command gcc -ErrorAction SilentlyContinue) {
  gcc -Wall -Wextra -O2 -o org.exe org.c
} else {
  Write-Error "gcc (MinGW-w64) not found. Install MinGW-w64 and ensure gcc is on PATH."
  exit 1
}

# Install location (override with ORG_HOME).
$InstallDir = if ($env:ORG_HOME) { $env:ORG_HOME } else { Join-Path $env:LOCALAPPDATA "Programs\org" }
New-Item -ItemType Directory -Force -Path $InstallDir | Out-Null
Copy-Item -Force "org.exe" (Join-Path $InstallDir "org.exe")

# Add to user PATH if needed.
$UserPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($UserPath -notlike "*$InstallDir*") {
  [Environment]::SetEnvironmentVariable("Path", "$UserPath;$InstallDir", "User")
  $Env:Path = "$Env:Path;$InstallDir"
}

Write-Host "Installed to $InstallDir"
Write-Host "Open a new terminal if 'org' is not found."
