#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <tuple>
#include <unordered_map>

using namespace std;

// Decode the transition table from the binary string encoding
map<pair<int, int>, tuple<int, int, int>> DecodingTransitionTable(const string& encodedTransition) {
    map<string, char> mapping = {
        {"00", '0'},
        {"01", '1'},
        {"10", '#'},
        {"11", ';'}
    };

    string decodedString;
    for (size_t i = 0; i < encodedTransition.size(); i += 2) {
        decodedString += mapping[encodedTransition.substr(i, 2)];
    }

    stringstream ss(decodedString);
    string transitions;
    map<pair<int, int>, tuple<int, int, int>> TransitionTable;

    // Parsing transitions
    while (getline(ss, transitions, ';')) {
        if (transitions.empty()) {
            continue;
        }
        vector<string> subStrings;
        stringstream tc(transitions);
        string subString;

        while (getline(tc, subString, '#')) {
            subStrings.push_back(subString);
        }

        if (subStrings.size() != 5) {
            continue;
        }

        try {
            int qi = stoi(subStrings[0], nullptr, 2);
            int sk = stoi(subStrings[1], nullptr, 2);
            int qj = stoi(subStrings[2], nullptr, 2);
            int sl = stoi(subStrings[3], nullptr, 2);
            int D = (subStrings[4] == "0") ? 0 : 1;
            TransitionTable[{qi, sk}] = {qj, sl, D};
        }
        catch (...) {
            continue;
        }
    }
    return TransitionTable;
}

// Simulate the Turing Machine with the given input
string TuringMachine(const string& encodedTransition, const string& inputString) {
    auto TransitionTable = DecodingTransitionTable(encodedTransition);

    // Initialize tape and head
    unordered_map<int, int> tape;
    for (size_t i = 0; i < inputString.length(); i++) {
        tape[i] = inputString[i] - '0'+1;
    }

    int head = 0;  // Starting position of the tape head
    int state = 0;  // Starting state q0
    const int acceptState = 1;
    const int rejectState = 2;
    const int blank = 0;  // Blank symbol

    // Simulate the Turing machine's execution
    int stepCount = 0;  // Prevent infinite loops by limiting steps
    const int maxSteps = 1000;  // Limit the maximum steps to avoid infinite loops

    while (true) {
        stepCount++;
        if (stepCount > maxSteps) {
            return "Error";  // Exceeded step limit, possible infinite loop
        }

        int symbol = tape.count(head) ? tape[head] : blank;  // Current symbol under the head

        // Check for transition from current state and symbol
        auto key = make_pair(state, symbol);
        if (TransitionTable.find(key) == TransitionTable.end()) {
            return "Error";  // No transition found, halt with error
        }

        auto [newState, writeSymbol, direction] = TransitionTable[key];
        tape[head] = writeSymbol;  // Write the new symbol to the tape

        state = newState;  // Update state

        if (state == acceptState) {
            return "Accept";  // Machine reached accept state
        }
        if (state == rejectState) {
            return "Reject";  // Machine reached reject state
        }

        // Move the head based on the direction (1 = Right, 0 = Left)
        if (direction == 1) {
            head++;
        } else {
            head--;
        }

        // Ensure tape is treated as infinite by handling out-of-bounds accesses
        if (head < 0) {
            tape[head] = blank;  // Add blank symbol to the left
            head = 0;
        } else if (head >= tape.size()) {
            tape[head] = blank;  // Add blank symbol to the right
        }
    }
}

int main() {
    string encodingTransition, x;
    cin >> encodingTransition;
    cin >> x;

    // Run the Turing Machine simulation
    cout << TuringMachine(encodingTransition, x) << endl;

    return 0;
}
