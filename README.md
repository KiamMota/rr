# rr - Recursive Replace

`rr` is a lightweight and efficient command-line utility for performing **massive text replacements** across files and directories. It is designed to handle large file structures with speed and precision.

## Features

- Replace words or patterns recursively in files and directories.
- Intelligent pipeline selection optimized based on file size for maximum performance.
- Verbose mode to track replacements with position info.
- Exception handling to skip specific lines or patterns (e.g., comments or strings).
- Optional backup/timestamp before performing replacements.

## Installation

Compile from source (Linux x86-64):

```bash
git clone <repo_url>
cd rr/build
make
