A lightweight, zero-dependency C++ implementation of the SHA-256 cryptographic hash function written from scratch and verified against official NIST test vectors.

<ins>**Quick Start**</ins>

Clone the repository:
```bash
git clone https://github.com/SayanNnd/SHA-256-Generator
cd SHA-256-Generator
```

Configure and Build (Release Mode):
Choose **one** build mode depending on your use case:

```bash
# --- Option A: Release Mode (For high performance) ---
cmake -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release

# --- Option B: Debug Mode (For development & debugging) ---
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
```

Run the Executable:

```bash
Windows: .\build-release\sha256_app.exe
         .\build-debug\sha256_app.exe

Linux/macOS: ./build-release/sha256_app
             ./build-debug/sha256_app.exe
```





