#include <iostream>
#include <string>
#include "DNACore/AhoCorasick.h"

using namespace DNACore;

int main() {
    // Create Aho-Corasick instance
    AhoCorasick ac;
    
    // Add pattern
    std::string pattern = "AAUAAA";
    std::string name = "Test RNA Poly-A";
    ac.addPattern(pattern, name);
    
    // Build automaton
    ac.buildAutomaton();
    
    // Search
    std::string text = "AAUAAA";
    auto results = ac.search(text);
    
    // Print results
    std::cout << "Pattern: " << pattern << std::endl;
    std::cout << "Text: " << text << std::endl;
    std::cout << "Matches found: " << results.size() << std::endl;
    
    for (const auto& result : results) {
        std::cout << "  Position " << result.position 
                  << ": " << result.matchedSequence 
                  << " (" << result.motifType << ")" << std::endl;
    }
    
    return 0;
}
