#include "sha256.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void runTests() {
    cout << "Running NIST Test Vectors...\n";
    
    struct TestCase {
        string name;
        string input;
        string expected;
    };
    
    vector<TestCase> tests = {
        {"Empty String", "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
        {"Short String", "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"},
        {"56 Characters", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"},
        {"Long String", "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1"}
    };
    
    SHA256 sha;
    int passed = 0;
    
    for (const auto& test : tests) {
        sha.update(test.input);
        string result = sha.digest();
        
        if (result == test.expected) {
            cout << "[PASS] " << test.name << endl;
            passed++;
        } else {
            cout << "[FAIL] " << test.name << "\n";
            cout << "       Expected: " << test.expected << "\n";
            cout << "       Got:      " << result << "\n";
        }
    }
    cout << "Tests Passed: " << passed << "/" << tests.size() << "\n\n";
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        string arg1 = argv[1];
        if (arg1 == "--test") {
            runTests();
            return 0;
        } else if (arg1 == "-f" || arg1 == "--file") {
            if (argc < 3) {
                cout << "Error: No file specified.\n";
                cout << "Usage: " << argv[0] << " -f <filename>\n";
                return 1;
            }
            string filename = argv[2];
            ifstream file(filename, ios::binary);
            if (!file) {
                cout << "Error: Could not open file " << filename << "\n";
                return 1;
            }
            
            SHA256 sha;
            const size_t bufferSize = 4096;
            char buffer[bufferSize];
            
            while (file.read(buffer, bufferSize)) {
                sha.update(reinterpret_cast<const uint8_t*>(buffer), file.gcount());
            }
            sha.update(reinterpret_cast<const uint8_t*>(buffer), file.gcount());
            
            cout << sha.digest() << "  " << filename << endl;
            return 0;
        } else {
            // Treat as string input
            SHA256 sha;
            string input = arg1;
            for (int i = 2; i < argc; ++i) {
                input += " ";
                input += argv[i];
            }
            sha.update(input);
            cout << sha.digest() << endl;
            return 0;
        }
    }
    
    // Interactive mode
    string message;
    cout << "Enter a sentence :- ";
    getline(cin, message);
    
    SHA256 sha;
    sha.update(message);
    cout << sha.digest() << endl;

    return 0;
}
