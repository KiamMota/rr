# rr - Recursive Replace

`rr` is a **high-performance command-line utility** designed for **large-scale, recursive text replacement** across files and directories. It is optimized to handle **multi-gigabyte files** efficiently, using memory-conscious pipelines and minimal I/O overhead while preserving safety and flexibility.

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

**Impact:** Refactoring to block-based reading reduces I/O operations and memory churn, yielding a ~28% speed improvement without changing the replacement output.

---

## Internal Workflow

1. **Target Identification:** Determines whether the input is a single file or a directory; applies recursion if specified.  
2. **Optimized Reading:** Processes files in memory-optimized blocks rather than line-by-line.  
3. **Pattern Replacement:** Applies literal or regex-based replacements, respecting exception rules.  
4. **Logging:** Outputs detailed replacement info when verbosity is enabled.  
5. **Safety Operations:** Creates backups or timestamps prior to overwriting files if the timestamp flag is active.  

`rr` is therefore ideal for **high-volume, automated, and safe text replacement tasks**, providing control and performance unmatched by traditional tools like `sed` or `perl` in massive file scenarios.
```
