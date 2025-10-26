# rr - Recursive Replace

`rr` is a high-performance command-line utility for **large-scale, recursive text replacements** in files and directories. It is designed to handle **multi-gigabyte files** efficiently, using memory-optimized pipelines and minimal I/O overhead.  

## Key Functionality

- **Recursive Replacement:** Traverse directories and perform replacements in multiple files simultaneously.
- **Pattern Matching:** Supports literal strings and regex patterns for flexible search-and-replace operations.
- **Dynamic Pipeline Selection:** Chooses the optimal replacement strategy based on file size and type, balancing memory usage and speed.
- **Verbose Mode:** Reports each replacement with file path, position, and context for debugging or logging.
- **Selective Skipping:** Can ignore lines, comments, or string literals based on rules to avoid unintended replacements.
- **Safe Operations:** Optional creation of temporary files or backups before overwriting originals.

# JSON Replacement System Refactoring (1 GB File)

## Background

We performed performance tests on a **1 GB JSON file** to replace all double quotes `"` with single quotes `'`.  

- **v0.3:** Original implementation, line-by-line processing with per-line flushes.
- **pre-v0.4:** Refactored version, optimized for large-file processing with block buffering.

### Terminal Output

**Original v0.3 script:**

```bash
replacing " to ' in movies1gb.json
time: real	0m10,775s
```

**refactored pre v0.4 script:**
```bash
replacing " to ' in movies1gb.json
time: real	0m7,766s
```

