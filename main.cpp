#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <chrono>
#include "sha256.h"

#define KB_128 (128*1024)
#define bold "\033[1m"
#define underline "\033[4m"
#define reset "\033[0m"
static bool benchmark_mode = false;
static std::string enabler = "Enable";

using namespace std;


static void fileopener();
static void stringopener();
static void processFile(string filePath);
static void processString(string str);
static string cleanPath(string filePath);

//------------------------------------------------------------------------------------------------------

int main(int argc ,char **argv) {
    //CLI INTERFACE
    if (argc>1) {
        string arg1 = argv[1];
        string name = "sha256";
        cout << endl;
        if (arg1=="-h" || arg1=="--help" || arg1=="help") {
            cout << bold underline "SHA-256 Hasher CLI\n" reset
            << "By Sayan Nandi\n"
            << bold underline "USAGE\n" reset
            << " " << name << " <file-path>                               Hashes File Paths\n"
            << " " << name << " -f <file-path>                            Hashes File Paths\n"
            << " " << name << " -s \"string to hash\"                       Hashes Strings\n"
            << " " << name << " -f -b <file-path>                         Benchmark file hashing\n"
            << " " << name << " -s -b \"string\"                            Benchmark string hashing\n"
            << " " << name << "                                           Launches Interactive menu\n";
        }
        else if  (arg1=="-f" || arg1=="--file") {
            if (argc>3) {
                string arg2 = argv[2];
                if (arg2 == "-b") {
                    benchmark_mode = true;
                    processFile(cleanPath(argv[3]));
                }
                else {
                    cout << bold "ERROR: Invalid flag " << arg2 << "\n" reset;
                }
            }
            else if (argc==3) {
                processFile(cleanPath(argv[2]));
            }
            else {
                cout << bold "ERROR MISSING ARGUMENTS\n" reset;
            }
        }
        else if  (arg1=="-s" || arg1=="--string") {
            if (argc>3) {
                string arg2 = argv[2];
                if (arg2 == "-b") {
                    benchmark_mode = true;
                    processString(argv[3]);
                }
                else {
                    cout << bold "ERROR: Invalid flag " << arg2 << "\n" reset;
                }
            }
            else if (argc==3) {
                processString(argv[2]);
            }
            else {
                cout << bold "ERROR MISSING ARGUMENTS\n" reset;
            }
        }
        else if (arg1=="-b") {
            benchmark_mode = true;
            processFile(cleanPath(argv[2]));
        }
        else {
            processFile(cleanPath(arg1));
        }
        cout<<endl;
        return 0;
    }

    //INTERACTIVE WINDOW
    while (true) {
        cout << "\n----------------------------------------------------------------\n";
        cout << "Welcome to SHA-256 Hasher" << endl;
        cout << "----------------------------------------------------------------\n\n";
        cout << "Enter 1 for strings.\nEnter 2 for files.\nEnter 3 to " << enabler << " Benchmark mode\nEnter 0 to exit.\n\n";
        int x;
        cin >> x;
        cout << "\033[1A\033[2K\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (x == 1) {
            stringopener();
        } else if (x == 2) {
            fileopener();
        } else if (x == 0) {
            return 0;
        } else if (x == 3) {
            if (benchmark_mode) {
                benchmark_mode=false;
                enabler="Enable";
            }else {
                benchmark_mode=true;
                enabler="Disable";
            }
            cout << "\033[1A\033[2K\n";
        }else {
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

//MAIN FUNCTIONS
void processFile(string filePath) {
    FILE* file_ptr = fopen(filePath.c_str(), "rb");
    if (!file_ptr) {
        cout << "Unable to open file." << endl;
        return;
    }

    SHA256 hasher;
    vector<uint8_t> buffer(KB_128);
    size_t bytes_read = 0;
    uint64_t total_bytes_read = 0;

    auto start_time = chrono::high_resolution_clock::now();

    while ((bytes_read=fread(buffer.data(), 1, buffer.size(), file_ptr)) > 0) {
        hasher.update(buffer.data(), bytes_read);
        total_bytes_read += bytes_read;
    }
    fclose(file_ptr);

    string output = hasher.final();
    auto end_time = std::chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    double seconds = elapsed.count();
    double megabytes = static_cast<double>(total_bytes_read) / (1024.0 * 1024.0);
    double speed_mbps = megabytes / seconds;

    cout << "SHA-256: " << output << "\n";
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

string cleanPath(string filePath) {
    if (filePath!="" && filePath.front()=='"' && filePath.back()=='"') {
        filePath=filePath.substr(1,filePath.size()-2);
    }
    return filePath;
}
