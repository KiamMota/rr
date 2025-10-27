# `rr` - `recursive` `replace` 

`rr` is a high-performance cross-platform command-line utility designed for **large-scale, recursive text replacement** across files and directories. Written in C++, it is optimized to handle multi-gigabyte files efficiently, using memory-conscious pipelines and minimal I/O overhead while preserving safety and flexibility.

---

## Purpose and Design

The primary goal of `rr` is to allow developers and sysadmins to perform **complex, large-scale search-and-replace operations** without the bottlenecks associated with traditional line-by-line tools. Its architecture balances **speed, memory usage, and operational safety**:

- **Memory-Efficient Pipelines:** Reads files in blocks rather than line-by-line to reduce memory pressure and avoid frequent flushes.  
- **Dynamic Replacement Strategy:** Adapts method based on file size and type for optimal throughput.  
- **Safety Measures:** Optional temporary files or backups prevent data loss during replacement.  
- **Context Awareness:** Skips replacements inside comments, strings, or other user-defined patterns to prevent unintended changes.  

---

## Command Syntax

```
rr <OLD_WORD> <NEW_WORD> <FILE_OR_DIR> [flags]
```

### Flags

- `-v / --verbosity` : Displays every replacement as it happens, including file path, byte offset, and context. Useful for debugging or auditing changes.  

- `-e / --exceptions` : Skip or specially handle lines/patterns defined by the user. Ideal for avoiding replacements inside comments, quoted strings, or other sensitive contexts.  

- `-r / --recursive` : Traverse directories recursively and apply replacements across all contained files. Critical for large projects or folder structures.  

- `-t / --timestamp` : Create a timestamped backup or snapshot of the file before performing replacements. Provides fail-safe recovery for critical data.  

---

## Example: JSON Replacement in a 1 GB File

**Objective:** Replace all double quotes `"` with single quotes `'` in `movies1gb.json`.

- **Original v0.3 (line-by-line processing, per-line flushes):**
```
replacing " to ' in movies1gb.json
time: real	0m10,775s
```

- **Refactored pre-v0.4 (block-buffered processing, optimized for large files):**
```
replacing " to ' in movies1gb.json
time: real	0m7,766s
```

`rr` is therefore ideal for **high-volume, automated, and safe text replacement tasks**, providing control and performance unmatched by traditional tools like `sed` or `perl` in massive file scenarios.
