#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "sha256.h"

using namespace std;

void fileopener() {
    string fileName;
    cout << "Enter the name of the file :- ";
    cin >> fileName;
    cout << endl;
    ifstream file (fileName,ios::ate | ios::binary);
    if (!file.is_open()) {
        cout << "File could not open or doesn't exist\n";
        return;
    }
    size_t fileSize = file.tellg();
    vector<uint8_t> buffer(fileSize);
    file.seekg(0,ios::beg);
    if (file.read(reinterpret_cast<char*>(buffer.data()),fileSize)) {
        const string Hex = sha256(buffer);
        cout << Hex << "\n";
    }
    else {
        cout<<"There was an error in reading the file...\n";
    }
}

void stringopener() {
    string message;
    cout << "Enter a sentence :- ";
    getline(cin, message);
    const string Hex = sha256(message);
    cout << Hex << "\n";
}

int main() {
  while (true) {
    cout << "\n----------------------------------------------------------------\n";
    cout << "Welcome to SHA-256 Hasher" << endl;
    cout << "----------------------------------------------------------------\n\n";
    cout << "Enter 1 for strings.\nEnter 2 for files.\nEnter 0 to exit.\n";
    int x;
    cin >> x;
    cout << "\033[1A\033[2K\n";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (x == 1) {
      stringopener();
    } 
    else if (x == 2) {
      fileopener();
    } 
    else if (x == 0) {
      return 0;
    } 
    else {
      cout << "Please enter a valid input";
    }
  }
}
