#include <cstdint>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <limits>
#include <chrono>
#include <stack>
#include <thread>
#include <atomic>
#include <mutex>
#include "sha256.h"

using namespace std;
namespace fs = std::filesystem;

#define KB_128 (128*1024)
#define bold "\033[1m"
#define underline "\033[4m"
#define reset "\033[0m"
static bool benchmark_mode = false;
static std::string enabler = "Enable";
static std::atomic<uint64_t> folder_bytes_read = 0;
static std::atomic<size_t> global_index(0);
static std::mutex cout_buffer;
static size_t total_count = std::thread::hardware_concurrency();
static size_t thread_count = std::max(1,static_cast<int>(total_count)/2);

enum class type {FILE_MODE,FOLDER_MODE,STRING_MODE,NONE};
struct Config{
    type Target = type::NONE;
    string path = "";
    int threads_no = thread_count;
    bool help = false;
};


static void processFolder(const fs::path& target);
static void worker_assign(const vector<fs::directory_entry> &entries, int worker_no, const fs::path &target);
static void fileopener();
static void stringopener();
static void processFile(const string& filePath);
static void processString(string str);
static void folderreader();
static void preset_selector();
static string cleanPath(string filePath);
static string process_thread_files(const string& filePath);


//------------------------------------------------------------------------------------------------------

int main(int argc ,char **argv) {
    Config config;
    if (thread_count == 0) {
        config.threads_no = 4;
        thread_count = 4;
    }
    //CLI INTERFACE
    if (argc>1) {
        bool invalid_argument = false;
        for (int i{1}; i<argc; i++) {
            string arg = argv[i];
            if (arg=="-b" || arg=="--benchmark") {
                benchmark_mode = !benchmark_mode;
            }
            else if (arg=="-h" || arg=="--help") {
                config.help = true;
                break;
            }
            else if (arg=="-f" || arg=="--file") {
                config.Target = type::FILE_MODE;
                if (invalid_argument) {
                    cout << "Invalid Arguments";
                    return 1;
                }
                if (i+1<argc) {
                    config.path = argv[i+1];
                    invalid_argument = true;
                    i++;
                }
            }
            else if (arg=="-s" || arg=="--string") {
                config.Target = type::STRING_MODE;
                if (invalid_argument) {
                    cout << "Invalid Arguments";
                    return 1;
                }
                if (i+1<argc) {
                    config.path = argv[i+1];
                    invalid_argument = true;
                    i++;
                }
            }
            else if (arg=="-d" || arg=="--directory") {
                config.Target = type::FOLDER_MODE;
                if (invalid_argument) {
                    cout << "Invalid Arguments";
                    return 1;
                }
                if (i+1<argc) {
                    config.path = argv[i+1];
                    invalid_argument = true;
                    i++;
                }
            }
            else if (arg=="-t" || arg=="--threads") {
                if (i+1<argc) {
                    const int no = atoi(argv[i+1]);
                    if (no>0 && no<=static_cast<int>(total_count)) {
                        config.threads_no = atoi(argv[i+1]);
                        i++;
                    }
                    else {
                        cout << "Please enter Thread Count from 1 to " << total_count;
                        return 1;
                    }
                }
                else {
                    cout << "Invalid Arguments";
                    return 1;
                }
            }
            else {
                config.path = arg;
            }
        }

        thread_count=config.threads_no;
        if (config.help) {
            const string name ="sha256";
            cout << bold underline "SHA-256 Hasher CLI\n" reset
            << "By Sayan Nandi\n"
            << bold underline "USAGE\n" reset
            << " " << name << " \"<file-path>\"                         Hashes File Paths\n"
            << " " << name << " -f \"<file-path>\"                      Hashes File Paths (Leave empty argument to hash current folder)\n"
            << " " << name << " -s \"string to hash\"                   Hashes Strings\n"
            << " " << name << " -d \"<folder-path>\"                    Hashes Folder\n"
            << " " << name << " -b                                    Benchmark mode\n"
            << " " << name << " -t \"<thread-count>\"                   Custom Threads\n"
            << " " << name << "                                       Launches Interactive menu\n";
            return 0;
        }

        switch (config.Target) {
            case type::NONE:
                processFile(config.path);
                break;
            case type::FILE_MODE:
                if (config.path.size()==0) {
                    cout << "Invalid Arguments";
                    return 1;
                }
                else processFile(cleanPath(config.path));
                break;
            case type::STRING_MODE:
                if (config.path.size()==0) {
                    cout << "Invalid Arguments";
                    return 1;
                }
                else processString(config.path);
                break;
            case type::FOLDER_MODE:
                if (config.path.size()==0) processFolder(fs::current_path());
                else processFolder(fs::path(config.path));
                break;
            default:
                cout << "Invalid Arguments";
                return 1;
        }
        return 0;
    }

    //INTERACTIVE WINDOW
    while (true) {
        cout << "\n----------------------------------------------------------------\n";
        cout << "Welcome to SHA-256 Hasher" << endl;
        cout << "----------------------------------------------------------------\n\n";
        cout << "Enter 1 for strings.\nEnter 2 for files.\nEnter 3 to scanning directories.\nEnter 4 to " << enabler
        << " Benchmark mode\nEnter 5 to Select Preset for Folder Hashing\nEnter 0 to exit.\n\n";
        int x;
        cin >> x;
        cout << "\033[1A\033[2K\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (x == 1) {
            stringopener();
        } else if (x == 2) {
            fileopener();
        } else if (x == 3) {
            folderreader();
        } else if (x == 4) {
            if (benchmark_mode) {
                benchmark_mode=false;
                enabler="Enable";
            }else {
                benchmark_mode=true;
                enabler="Disable";
            }
            cout << "\033[1A\033[2K\n";
        }else if (x == 5) {
            preset_selector();
        }else if (x == 0) {
            return 0;
        } else {
            cout << "Please enter a valid input";
        }
    }
}

//------------------------------------------------------------------------------------------------------

//INTERACTIVE HELPERS
void fileopener() {
    string fileName;
    cout << "Enter the name of the file :- ";
    getline(cin, fileName);
    cout << endl;
    processFile(fileName);

}
void stringopener() {
    string message;
    cout << "Enter a sentence :- ";
    getline(cin, message);
    processString(message);
}

static void folderreader() {
    string folderPath;
    cout << "Enter the folder path (Type \"/\" for current folder):- ";
    getline(cin, folderPath);
    if (folderPath == "/") {
        processFolder(fs::current_path());
    }
    else {
        processFolder(fs::path(folderPath));
    }
}

void preset_selector() {
    int x;
    cout << "Choose one of the presets :- (Current Thread Count :- " << thread_count << ")"
    "\nEnter 1 for Silent.\nEnter 2 for Balanced\nEnter 3 for Performance    (Unstable in some cases)\nEnter 4 for Max            (Unstable in some cases)\nEnter 5 to choose custom Thread count\n";
    cin >> x;
    cout << "\033[1A\033[2K\n";
    if (x==1) {
        thread_count = std::max(1,static_cast<int>(total_count)/4);
    }
    if (x==2) {
        thread_count = std::max(1,static_cast<int>(total_count)/2);
    }
    if (x==3) {
        thread_count = std::max(1,static_cast<int>(total_count*3)/4);
    }
    if (x==4) {
        thread_count = std::max(1,static_cast<int>(total_count));
    }
    if (x==5) {
        size_t new_count=1;
        while (true) {
            cout << "Enter the number of threads you want to use (Your Max Thread Count = " << total_count << "):- ";
            cin >> new_count;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (new_count>0 && new_count<=total_count) {
                thread_count = new_count;
                break;
            }
            else {
                cout << "Please Enter a valid thread count.\n";
            }
        }
    }
}

//MAIN FUNCTIONS
static string cleanPath(string filePath) {
    if (filePath.empty() && filePath.front()=='"' && filePath.back()=='"') {
        filePath=filePath.substr(1,filePath.size()-2);
    }
    return filePath;
}

void processFile(const string& filePath) {
    FILE* file_ptr = fopen(filePath.c_str(), "rb");
    if (!file_ptr) {
        cout << "Unable to open file." << endl;
        return;
    }

    SHA256 hasher;
    vector<uint8_t> buffer(KB_128);
    size_t bytes_read = 0;
    uint64_t total_bytes_read = 0;

    const auto start_time = chrono::high_resolution_clock::now();

    while ((bytes_read=fread(buffer.data(), 1, buffer.size(), file_ptr)) > 0) {
        hasher.update(buffer.data(), bytes_read);
        total_bytes_read += bytes_read;
        folder_bytes_read += bytes_read;
    }
    fclose(file_ptr);

    const string output = hasher.final();
    const auto end_time = std::chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    const double seconds = elapsed.count();
    const double megabytes = static_cast<double>(total_bytes_read) / (1024.0 * 1024.0);
    const double speed_mbps = megabytes / seconds;

    cout << output << "\n";
    if (benchmark_mode) {
        cout << "------------------------------------------------\n";
        cout << "Time taken : " << seconds << " seconds\n";
        cout << "Approx Speed : " << speed_mbps << " MB/s\n";
        cout << "------------------------------------------------\n";
    }
}

void processString(string str) {
    vector<uint8_t> sentence(str.begin(),str.end());

    SHA256 hasher;
    auto start_time = chrono::high_resolution_clock::now();

    hasher.update(sentence.data(), sentence.size());
    string output = hasher.final();
    auto end_time = std::chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    double seconds = elapsed.count();
    double megabytes = static_cast<double>(sentence.size()) / (1024.0 * 1024.0);
    double speed_mbps = megabytes / seconds;
    cout << "SHA-256: " << output << "\n";
    if (benchmark_mode) {
        cout << "------------------------------------------------\n";
        cout << "Time taken : " << seconds << " seconds\n";
        cout << "Approx Speed : " << speed_mbps << " MB/s\n";
        cout << "------------------------------------------------\n";
    }
}

static void processFolder(const fs::path& target) {
    global_index = 0;
    folder_bytes_read = 0;
    auto start_time = chrono::high_resolution_clock::now();

    stack<fs::path> sub_targets;
    sub_targets.push(target);
    vector<fs::directory_entry> entries;
    while (!sub_targets.empty()) {
        fs::path curr_path = sub_targets.top();
        sub_targets.pop();
        for (auto& entry : fs::directory_iterator(curr_path)) {
            if (entry.is_directory()) {
                sub_targets.push(entry.path());
            }
            else if (entry.is_regular_file()) {
                if (entry.path().filename()!= "sha256.exe")
                entries.push_back(entry);
            }
        }
    }
    const size_t fileNo = entries.size();

    bool folder_bench = false;
    if (benchmark_mode) folder_bench = true;
    benchmark_mode = false;

    vector<thread> working_threads;
    for (size_t i = 1; i <= thread_count; i++) {
        working_threads.emplace_back(worker_assign,ref(entries),i, target);
    }

    for (auto& t : working_threads) {
        t.join();
    }

    const auto end_time = std::chrono::high_resolution_clock::now();
    const chrono::duration<double> elapsed = end_time - start_time;
    const double seconds = elapsed.count();
    const double megabytes = static_cast<double>(folder_bytes_read) / (1024.0 * 1024.0);
    const double speed_mbps = megabytes / seconds;

    if (folder_bench) {
        cout << "------------------------------------------------\n";
        cout << "Folder Size : " << megabytes << " MegaBytes\n";
        cout << "Number of Files : " << fileNo << "\n";
        cout << "Time taken : " << seconds << " seconds\n";
        cout << "Approx Speed : " << speed_mbps << " MB/s\n";
        cout << "------------------------------------------------\n";
        benchmark_mode = true;
    }
}

static void worker_assign(const vector<fs::directory_entry> &entries, const int worker_no, const fs::path &target) {
    const size_t n =entries.size();
    while (true) {
        size_t curr_no = global_index.fetch_add(1);
        if (curr_no >= n) break;

        const string output = process_thread_files(entries[curr_no].path().string());
        {
            lock_guard<mutex> lock(cout_buffer);
            if (output=="T-T") cout << "[THREAD NO - " << worker_no << "] " << fs::relative(entries[curr_no],target) << " :- Couldn't read file" << "\n";
            else cout << "[THREAD NO - " << worker_no << "] " << fs::relative(entries[curr_no],target) << " :- " << output << "\n";
        }
    }
}

static string process_thread_files(const string& filePath) {
    FILE* file_ptr = fopen(filePath.c_str(), "rb");
    if (!file_ptr) {
        return "T-T";
    }
    SHA256 hasher;
    vector<uint8_t> buffer(KB_128);
    size_t bytes_read = 0;
    while ((bytes_read=fread(buffer.data(), 1, buffer.size(), file_ptr)) > 0) {
        hasher.update(buffer.data(), bytes_read);
        folder_bytes_read+=bytes_read;
    }
    fclose(file_ptr);
    const string output = hasher.final();
    return output;
}