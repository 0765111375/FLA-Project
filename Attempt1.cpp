#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

// Function to decode the binary-encoded transition table
map<pair<int, int>, tuple<int, int, int>> decodeTransitionTable(const string &encodedTransition) {
    // Mapping binary chunks to characters
    map<string, char> mapping = {{"00", '0'}, {"01", '1'}, {"10", '#'}, {"11", ';'}};
    string decodedStr;

    // Decode the binary string into readable format
    for (size_t i = 0; i < encodedTransition.size(); i += 2) {
        string chunk = encodedTransition.substr(i, 2);
        decodedStr += mapping[chunk];
    }

    // Split transitions by ';'
    stringstream ss(decodedStr);
    string transition;
    map<pair<int, int>, tuple<int, int, int>> transitionDict;

    while (getline(ss, transition, ';')) {
        if (transition.empty()) continue;

        // Split each transition by '#'
        vector<string> parts;
        stringstream ts(transition);
        string part;
        while (getline(ts, part, '#')) {
            parts.push_back(part);
        }

        if (parts.size() != 5) continue; // Invalid transition

        // Parse components of the transition
        try {
            int currentState = stoi(parts[0], nullptr, 2);
            int currentSymbol = stoi(parts[1], nullptr, 2);
            int nextState = stoi(parts[2], nullptr, 2);
            int writeSymbol = stoi(parts[3], nullptr, 2);
            char direction = parts[4] == "0" ? 0 : 1;

            transitionDict[{currentState, currentSymbol}] = {nextState, writeSymbol, direction};
        } catch (...) {
            continue; // Handle invalid binary parsing
        }
    }

    return transitionDict;
}

// Function to simulate the Turing Machine
string simulateTuringMachine(const string &encodedTransition, const string &inputStr) {
    // Decode the transition table
    auto transitionDict = decodeTransitionTable(encodedTransition);

    // Initialize the tape with input symbols
    vector<int> tape;
    for (char c : inputStr) {
        tape.push_back(c - '0' + 1); // Convert input to symbol indices (si = i + 1)
    }

    int head = 0;
    int currentState = 0; // qinit = q0

    while (true) {
        if (currentState == 1) return "Accept";   // qaccept = q1
        if (currentState == 2) return "Reject";   // qreject = q2

        // Read current symbol
        int currentSymbol = (head < 0 || head >= tape.size()) ? 0 : tape[head]; // s0=⊔

        auto key = make_pair(currentState, currentSymbol);

        if (transitionDict.find(key) == transitionDict.end()) return "Error"; // No valid transition

        auto [nextState, writeSymbol, direction] = transitionDict[key];

        // Write symbol to tape
        if (head < 0) {
            tape.insert(tape.begin(), writeSymbol);
            head = 0;
        } else if (head >= tape.size()) {
            tape.push_back(writeSymbol);
            head = tape.size() - 1;
        } else {
            tape[head] = writeSymbol;
        }

        // Move head
        head += (direction == 1 ? 1 : -1);

        // Update state
        currentState = nextState;
    }
}

int main() {
    // Sample inputs from Table 2
    vector<pair<string, string>> testCases = {
        {"00100010010010001001110010011000100100100111001001001001100100100111", "0"},
        {"00100010010010001001110010011000100100100111001001001001100100100111", "1"},
        {"0010001001001000100111001001001001100100100111", "0"}
    };

    for (auto &[encodedTransition, inputStr] : testCases) {
        cout << simulateTuringMachine(encodedTransition, inputStr) << endl;
    }

    return 0;
}

