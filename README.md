A lightweight, zero-dependency C++ implementation of the SHA-256 cryptographic hash function written from scratch and verified against official NIST test vectors. 
Built for maximum throughput and minimal overhead, this utility uses a stateful streaming architecture and heap-based stack traversal to hash massive single files (40GB+) or entire directory trees (150GB+) while maintaining a near-zero memory footprint.
(God bless all my brothers using 16GB RAM T-T)

## Features
* **Multi-Threaded Folder Hashing:** Dynamically scales across your CPU cores to saturate modern NVMe SSDs (achieving 1.5+ GB/s throughput on multi-file reads). Includes custom thread limits and pre-configured performance presets.
* **Massive File Support:** Hashes files of any size using a 128 KB streaming buffer.
* **High Performance:** Achieves ~250-275+ MB/s single-core throughput in pure C++ through efficient buffered disk reads. (smth like that)
* **Dual Interface:** Supports standard CLI execution for scripting or a guided interactive menu. (No one using ts)
* **Folder & File Telemetry:** Built-in benchmarking flags (```-b```) measure total processed dataset size (MB), file count, execution time, and aggregate throughput (MB/s).
* **Iterative Directory Traversal:** Recursively scans and hashes entire folder hierarchies using an explicit heap stack—eliminating call-stack overflow risks on deeply nested directories.
* **Zero Dependencies:** Pure standard C++(```std::filesystem```), requiring no external crypto libraries (OpenSSL, Libsodium, etc.). (I love re-inventing da weheel)

<img width="1280" height="720" alt="ezgif com-speed" src="https://github.com/user-attachments/assets/d963e268-218d-4b53-872c-aa48fe6dcb64" />
(Video is sped up by 150%)

---

## Installation

### Option 1: Windows Quick Install (Recommended)
You can install the pre-compiled binary globally so you can use the `sha256` command in any terminal folder.

1. Download the latest **Release ZIP** from the Releases page.
2. Right-click and run `install_to_path.bat` as Administrator. 
3. Restart your terminal. You can now use the `sha256` command anywhere!

### Option 2: Build from Source
To compile the project yourself, clone the repository:
```bash
git clone https://github.com/SayanNnd/SHA-256-Generator
cd SHA-256-Generator
```
**Windows (PowerShell with MinGW / MSYS2):**
```PowerShell
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

## Usage

### Basic Commands
Hash a file directly:
```bash
sha256 ""C:\Nintendo Games\Mario Kart 8 Deluxe v3.0.5.nsp""
 or
sha256 -f ""C:\Nintendo Games\Mario Kart 8 Deluxe v3.0.5.nsp""
```
(Yes I had no other example :3)

Hash a text string:
```bash
sha256 -s "shinji crank that soulja boy"
```

Hash an entire folder tree (all subdirectories and regular files, Leave empty to scan current directory):

```PowerShell
sha256 -d -b "D:\Games\Gang Beasts"
```
```PowerShell
sha256 -d
```

### Multi-Threading (-t)
When hashing folders, the app defaults to a "Balanced" core count (50% of your max threads). You can manually override this using the -t flag to squeeze maximum performance out of your SSD.

```bash
sha256 -d -t 12 "D:\SteamLibrary\steamapps\common\Destiny 2"
```

### Benchmarking Mode (`-b`)
Append the `-b` flag to output detailed runtime metrics, including elapsed seconds and processing speed (MB/s). 

```bash
sha256 -f -b "Massive_Humongous_InsanelyHuge_CyberPunk_recording.mkv"
```
(Cyberpunk 2077 is very fun)
 
*Output:*
```text
SHA-256: 8bd8a9ce06ab48eaf41ca1f55ab801e7e32cf1cec12cf68abdbf3f2698a81920
------------------------------------------------
Time taken   : 20.53177 seconds
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
