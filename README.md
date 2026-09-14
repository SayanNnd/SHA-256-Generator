# SHA-256 Engine in C++

A lightweight, zero-dependency C++ implementation of the SHA-256 cryptographic hash function written from scratch and verified against official NIST test vectors. It supports hashing arbitrary-length strings as well as large files in a memory-efficient manner.

## Features

- **Object-Oriented Design:** Clean and reusable `SHA256` class.
- **Arbitrary Length Support:** Processes strings of any length via proper multi-block chunking and padding.
- **File Hashing:** Hashes files efficiently by streaming them in 4KB chunks.
- **Test Suite:** Built-in verification against official NIST test vectors.

## Quick Start

Compile and run using any C++11 or newer compiler:

```bash
# Compile
g++ main.cpp sha256.cpp -o sha256
```

### Usage

**1. Interactive String Hashing (Default)**
```bash
./sha256
```
*(Prompts you to enter a sentence via standard input)*

**2. Command-Line String Hashing**
```bash
./sha256 "your string here"
```

**3. File Hashing**
```bash
./sha256 -f path/to/your/file.txt
# or
./sha256 --file path/to/your/file.txt
```

**4. Run Built-in NIST Test Suite**
```bash
./sha256 --test
```
