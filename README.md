
# File Organiser

Simple C utility that organises files in one or more directories into
subfolders based on file extension. Files without an extension go into
an `others` folder.

## Features

- Groups files by extension inside each provided directory.
- Creates destination folders on demand.
- Skips non-regular files and `.`/`..` entries.

## Requirements

- A C compiler (GCC or Clang)

## Quick Install (Recommended)

### Linux and macOS

One command:

```bash
./install.sh
```

Manual:

```bash
make
sudo make install
```

This installs the `org` command to `/usr/local/bin` by default.

### Windows (MinGW-w64)

One command (PowerShell):

```powershell
powershell -ExecutionPolicy Bypass -File .\install.ps1
```

Manual:

```bash
mingw32-make
```

Then move `org.exe` to a folder already on your PATH (for example
`C:\\Windows\\System32` or a custom tools folder), or add the project
directory to your PATH.

### Optional install locations

- Linux/macOS: set `PREFIX` or `BINDIR`, for example:

```bash
PREFIX=$HOME/.local ./install.sh
```

- Windows: set `ORG_HOME`, for example:

```powershell
$env:ORG_HOME = "$env:USERPROFILE\bin"
powershell -ExecutionPolicy Bypass -File .\install.ps1
```

## Build (Manual)

### Linux

```bash
gcc -Wall -Wextra -O2 -o org org.c
```

### macOS

```bash
clang -Wall -Wextra -O2 -o org org.c
```

### Windows (MinGW-w64)

1. Install MinGW-w64 and ensure `gcc` is on your PATH.
2. Build:

```bash
gcc -Wall -Wextra -O2 -o org.exe org.c
```

## Usage

Run the executable with one or more directories:

```bash
./org /path/to/dir1 /path/to/dir2
```

On Windows (PowerShell or CMD):

```bash
org.exe C:\\path\\to\\dir1 C:\\path\\to\\dir2
```

### Examples

Organise the current directory:

```bash
./org .
```

Organise multiple folders at once:

```bash
./org ~/Downloads ~/Desktop
```

## Notes

- The tool moves files into folders named after their extensions.
- Files without an extension are moved into the `others` folder.
- If a destination folder already exists, it is reused.

## Troubleshooting

- If you see permission errors, ensure you have write access to the
	target directory.
- On Windows, verify that the MinGW-w64 `bin` directory is in your PATH.
