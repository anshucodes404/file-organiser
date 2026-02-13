# File Organiser

A small utility that organizes files in one or more directories into subfolders by file extension, and can undo the moves using a log file.

## What it does

- Scans each provided directory for regular files.
- Creates subfolders named after each file extension (for files without an extension, it uses `others`).
- Moves files into their corresponding extension subfolder.
- Writes a log of every move to `$HOME/.file_org.txt` so it can be undone later.

## Get the code

```bash
git clone https://github.com/anshucodes404/file-organiser.git
cd file-organiser
```

## Build

```bash
gcc org.c -o org
```

## Add to PATH

Choose one of the following approaches.

Option A: Install into a local bin directory

```bash
mkdir -p "$HOME/bin"
cp ./org "$HOME/bin/"
```

Ensure `$HOME/bin` is in your `PATH`. If not, add this line to your shell profile (for example `~/.bashrc` or `~/.zshrc`):

```bash
export PATH="$HOME/bin:$PATH"
```

## Usage

### Organize files

```bash
./org <directory1> [<directory2> ...]
```

Example:

```bash
org ~/Downloads ~/Desktop
```

### Undo (deorganise)

```bash
org -u
```

The undo flow asks for a timestamp range. Use the format `dd:mm:yyyy hh:mm:ss`.

- Start timestamp is required.
- End timestamp is optional. Enter `-` to undo everything from the start timestamp onward.

Example:

```text
Enter start timestamp in format (dd:mm:yyyy hh:mm:ss): 13:02:2026 18:26:00
Enter end timestamp in format (dd:mm:yyyy hh:mm:ss): -
```

## Notes

- The log file is stored at `$HOME/.file_org.txt`.
- Only regular files are moved. Directories are ignored.
