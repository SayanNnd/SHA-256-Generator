#pragma once
#include <string>
#include <cstdint>
#include <vector>

using namespace std;

string sha256(vector<uint8_t> &message);
string sha256(string &message);