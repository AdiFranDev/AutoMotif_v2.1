#include <iostream>
#include <string>
#include "DNACore/AhoCorasick.h"
#include "DNACore/MotifDatabase.h"

using namespace DNACore;

int main() {
    // EXACT sequence from the screenshot
    std::string sequence = "TATAAAGGCCAATCTGGGCGGGCCACCATGGAATAAAAAAAAATATAGGGCGGAAUAAA";
    
    std::cout << "Testing sequence from screenshot:" << std::endl;
    std::cout << "Length: " << sequence.length() << std::endl;
    std::cout << "Sequence: " << sequence << std::endl;
    
    // Find "AAUAAA" manually
    size_t pos = sequence.find("AAUAAA");
    if (pos != std::string::npos) {
        std::cout << "\n'AAUAAA' found manually at position: " << pos << std::endl;
        std::cout << "Substring: '" << sequence.substr(pos, 6) << "'" << std::endl;
    } else {
        std::cout << "\n'AAUAAA' NOT found in sequence!" << std::endl;
    }
    
    // Find "AATAAA" manually
    pos = sequence.find("AATAAA");
    if (pos != std::string::npos) {
        std::cout << "\n'AATAAA' found manually at position: " << pos << std::endl;
        std::cout << "Substring: '" << sequence.substr(pos, 6) << "'" << std::endl;
    }
    
    // Now test with Aho-Corasick
    std::cout << "\n=== Aho-Corasick Test ===" << std::endl;
    AhoCorasick ac;
    
    auto allMotifs = MotifDatabase::getAllMotifs();
    for (const auto& motif : allMotifs) {
        std::cout << "Adding: " << motif.name << " = '" << motif.pattern << "'" << std::endl;
        ac.addPattern(motif.pattern, motif.name);
    }
    ac.buildAutomaton();
    
    auto results = ac.search(sequence);
    std::cout << "\nTotal matches: " << results.size() << std::endl;
    for (const auto& result : results) {
        std::cout << "  Position " << result.position 
                  << ": '" << result.matchedSequence << "'"
                  << " (" << result.motifType << ")" << std::endl;
    }
    
    return 0;
}
