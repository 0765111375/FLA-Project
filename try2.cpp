nst {
        return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
    }
};

// Function to decode the binary-encoded transition table
std::unordered_map<std::pair<int, int>, std::tuple<int, int, int>, pair_hash> decodeTransitionTable(const std::string& encodedDelta) {
    std::unordered_map<std::pair<int, int>, std::tuple<int, int, int>, pair_hash> transitions;
    std::unordered_map<std::string, char> mapping = { {"00", '0'}, {"01", '1'}, {"10", '#'}, {"11", ';'} };
    std::string decoded;

    // Decode the binary transition table
    for (size_t i = 0; i < encodedDelta.length(); i += 2) {
        decoded += mapping[encodedDelta.substr(i, 2)];
    }

    // Split by ';' to extract transitions
    size_t start = 0, end;
    while ((end = decoded.find(';', start)) != std::string::npos) {
        std::string transition = decoded.substr(start, end - start);
        start = end + 1;

        // Split transition by '#'
        std::vector<std::string> parts;
        size_t pos = 0, found;
        while ((found = transition.find('#', pos)) != std::string::npos) {
            parts.push_back(transition.substr(pos, found - pos));
            pos = found + 1;
        }
        parts.push_back(transition.substr(pos));

        if (parts.size() != 5) continue;  // Invalid transition

        int qi = std::stoi(parts[0], nullptr, 2);
        int sk = std::stoi(parts[1], nullptr, 2);
        int qj = std::stoi(parts[2], nullptr, 2);
        int sl = std::stoi(parts[3], nullptr, 2);
        int D = std::stoi(parts[4]);

        transitions[{qi, sk}] = {qj, sl, D};
    }

    return transitions;
}

// Function to simulate the Turing Machine
std::string simulateTuringMachine(const std::string& encodedDelta, const std::string& inputString) {
    auto transitions = decodeTransitionTable(encodedDelta);

    // Convert input string to tape (ensuring it contains valid symbols)
    std::vector<int> tape;
    for (char ch : inputString) {
        if (ch < '0' || ch > '9') return "Error";  // Invalid symbol
        tape.push_back(ch - '0');
    }

    // Initialize the Turing Machine
    int headPosition = 0;
    int currentState = 0;  // qinit = q0
    const int BLANK_SYMBOL = 10;
    const int qaccept = 1;
    const int qreject = 2;

    tape.push_back(BLANK_SYMBOL);  // Ensure tape extends

    // Simulate the Turing Machine
    while (true) {
        int currentSymbol = (headPosition >= 0 && headPosition < tape.size()) ? tape[headPosition] : BLANK_SYMBOL;

        if (transitions.find({currentState, currentSymbol}) == transitions.end()) {
            return "Error";  // No valid transition
        }

        auto [nextState, writeSymbol, direction] = transitions[{currentState, currentSymbol}];

        // Write symbol and move head
        tape[headPosition] = writeSymbol;
        headPosition += (direction == 1) ? 1 : -1;
        currentState = nextState;

        // Extend tape if needed
        if (headPosition < 0) {
            tape.insert(tape.begin(), BLANK_SYMBOL);
            headPosition = 0;
        } else if (headPosition >= tape.size()) {
            tape.push_back(BLANK_SYMBOL);
        }

        if (currentState == qaccept) return "Accept";
        if (currentState == qreject) return "Reject";
    }
}

int main() {
    std::string encodedDelta, inputString;
    std::cin >> encodedDelta >> inputString;
    
    std::cout << simulateTuringMachine(encodedDelta, inputString) << std::endl;
    return 0;
}
