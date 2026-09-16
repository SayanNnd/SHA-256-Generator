#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include "sha256.h"

using namespace std;

int main() {
    string message;
    cout << "Enter a sentence :- ";
    getline(cin,message);
    
    string Hex = sha256(message);
    cout << Hex;

    return 0;
}
