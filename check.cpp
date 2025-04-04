#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <stdexcept>

using namespace std;

// Hash function for pair<int, int> to be used as a key in unordered_map
struct pair_hash {
    template <typename T1, typename T2>
    size_t operator()(const pair<T1, T2>& p) const {
        return hash<T1>()(p.first) ^ (hash<T2>()(p.second) << 1);
    }
};

// Function to parse and decode the transition table from the encoded binary string
unordered_map<pair<int, int>, tuple<int, int, int>, pair_hash> TransitionTable(const string& encodedString) {
    unordered_map<pair<int, int>, tuple<int, int, int>, pair_hash> transitions;
    unordered_map<string, char> mapping = {{"00", '0'}, {"01", '1'}, {"10", '#'}, {"11", ';'}};
    string decodedString = "";

    // Convert the binary-encoded string into readable format
    for (size_t i = 0; i < encodedString.length(); i += 2) {
        decodedString += mapping[encodedString.substr(i, 2)];
    }
    
    size_t track = 0;
    while ((track = decodedString.find(';')) != string::npos) {
        string transition = decodedString.substr(0, track);
        decodedString.erase(0, track + 1);

        // Finding positions of '#' separators
        size_t t1 = transition.find('#');
        size_t t2 = transition.find('#', t1 + 1);
        size_t t3 = transition.find('#', t2 + 1);
        size_t t4 = transition.find('#', t3 + 1);

        // If transition is not properly formatted, skip it
        if (t1 == string::npos || t2 == string::npos || t3 == string::npos || t4 == string::npos) {
            continue;
        }

        // Extract values and convert them from binary to integers
        int qi = stoi(transition.substr(0, t1), nullptr, 2);
        int sk = stoi(transition.substr(t1 + 1, t2 - t1 - 1), nullptr, 2);
        int qj = stoi(transition.substr(t2 + 1, t3 - t2 - 1), nullptr, 2);
        int sl = stoi(transition.substr(t3 + 1, t4 - t3 - 1), nullptr, 2);
        int D = stoi(transition.substr(t4 + 1), nullptr, 10);
        
        // Store transition rule in hash map
        transitions[{qi, sk}] = {qj, sl, D};
    }
    return transitions;
}

// Function to simulate the Turing Machine
string TuringMachine(const unordered_map<pair<int, int>, tuple<int, int, int>, pair_hash>& transitions, const string& inputString) {
    vector<int> tape;
    
    // Convert input string to tape representation (as integer values)
    for (size_t i = 0; i < inputString.length(); i++) {
        if (inputString[i] < '0' || inputString[i] > '9') {
            return "Error"; // Return error if input contains invalid characters
        }
        tape.push_back(inputString[i] - '0');
    }

    int head = 0;  // Position of the read/write head on the tape
    int state = 0; // Initial state of the Turing Machine
    const int accept_state = 1; // Accept state
    const int reject_state = 2; // Reject state
    const int blank = 10; // Representation for blank symbol
    
    tape.push_back(blank); // Add a blank at the end of the tape

    while (true) {
        int symbol;
        
        // Get the symbol at the current head position
        if (head >= 0 && head < tape.size()) {
            symbol = tape[head];
        } else {
            symbol = blank; // Default to blank if out of bounds
        }

        // Find transition rule for current state and symbol
        auto found = transitions.find({state, symbol});
        if (found == transitions.end()) {
            return "Error"; // Return error if no valid transition exists
        }

        // Extract transition values
        auto [newState, newSymbol, direction] = found->second;

        // Update tape and state
        tape[head] = newSymbol;
        state = newState;

        // If the machine reaches accept or reject state, return the result
        if (state == accept_state) {
            return "Accept";
        } else if (state == reject_state) {
            return "Reject";
        }

        // Move the head left or right
        if (direction == 0) {
            head -= 1;
        } else {
            head += 1;
        }
        
        // Expand tape if head moves beyond current boundaries
        if (head < 0) {
            tape.insert(tape.begin(), blank);
            head = 0;
        } else if (head >= tape.size()) {
            tape.push_back(blank);
        }
    }
}

int main() {
    string encodedString, inputString;
    
    // Read encoded transition table and input string from user
    cin >> encodedString;
    cin >> inputString;

    // Parse the transition table from encoded string
    auto transitions = TransitionTable(encodedString);
    
    // Simulate the Turing Machine and print the result
    cout << TuringMachine(transitions, inputString) << endl;
}
