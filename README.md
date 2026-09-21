A lightweight, zero-dependency C++ implementation of the SHA-256 cryptographic hash function written from scratch and verified against official NIST test vectors.

<ins>**Quick Start**</ins>

Clone the repository:
```bash
git clone https://github.com/SayanNnd/SHA-256-Generator
cd SHA-256-Generator
```

Configure and Build (Release Mode):
```bash
# Windows (PowerShell with MinGW / MSYS2):
cmake -B build-release -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release
cmake --build build-release

# Linux/macOS:
cmake -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

Run the Executable:
```bash
Windows: .\build-release\sha256_app.exe

Linux/macOS: ./build-release/sha256_app
```





