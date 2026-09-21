A lightweight, zero-dependency C++ implementation of the SHA-256 cryptographic hash function written from scratch and verified against official NIST test vectors. 
Built for maximum throughput and minimal overhead, this utility uses a stateful streaming architecture to hash massive files (40GB+) while maintaining a near-zero memory footprint.

## ✨ Features
* **Massive File Support:** Hashes files of any size using a 128 KB streaming buffer.
* **High Performance:** Achieves ~250-275+ MB/s single-core throughput in pure C++ through efficient buffered disk reads.
* **Dual Interface:** Supports standard CLI execution for scripting or a guided interactive menu.
* **Built-in Benchmarking:** Toggleable telemetry to measure execution time and disk/CPU throughput in MB/s.
* **Zero Dependencies:** Pure standard C++, requiring no external crypto libraries (OpenSSL, Libsodium, etc.).

<img width="1280" height="720" alt="GitLarp-ezgif com-video-to-gif-converter" src="https://github.com/user-attachments/assets/ef0ea523-eea5-4546-9bca-d43fd9288e53" />

---

## 🚀 Installation

### Option 1: Windows Quick Install (Recommended)
You can install the pre-compiled binary globally so you can use the `sha256` command in any terminal folder.

1. Download the latest **Release ZIP** from the Releases page.
2. Extract the folder to a permanent location (e.g., `C:\Tools\SHA256`).
3. Right-click and run `install_to_path.bat` as Administrator. 
4. Restart your terminal. You can now use the `sha256` command anywhere!

### Option 2: Build from Source
To compile the project yourself, clone the repository:
```bash
git clone https://github.com/SayanNnd/SHA-256-Generator
cd SHA-256-Generator
```

**Windows (PowerShell with MinGW / MSYS2):**
```bash
cmake -B build-release -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
.\build-release\sha256.exe
```

**Linux/macOS:**
```bash
cmake -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
./build-release/sha256
```

---

## 💻 Usage

### Basic Commands
Hash a file directly:
```bash
sha256 "C:\path\to\file.iso"
  or
sha256 -f "C:\path\to\file.iso"
```

Hash a text string:
```bash
sha256 -s "The quick brown fox jumps over the lazy dog"
```

### Benchmarking Mode (`-b`)
Append the `-b` flag to output detailed runtime metrics, including elapsed seconds and processing speed (MB/s). 

```bash
sha256 -f -b "massive_database_backup.sql"
```
*Output:*
```text
SHA-256: 8bd8a9ce06ab48eaf41ca1f55ab801e7e32cf1cec12cf68abdbf3f2698a81920
------------------------------------------------
Time taken   : 7.53177 seconds
Approx Speed : 274.917 MB/s
------------------------------------------------
```

### Interactive Mode
Running the tool without arguments launches the interactive menu:
```bash
sha256
```

---

## 📄 License
This project is open-source and available under the [MIT License](LICENSE).
