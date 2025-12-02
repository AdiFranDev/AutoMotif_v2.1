#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>
#include "AnalysisTypes.h"

namespace DNACore {

    /**
     * Aho-Corasick Algorithm for Multi-Pattern String Matching
     * Time Complexity: O(n + m + z) where:
     *   n = text length
     *   m = sum of all pattern lengths
     *   z = number of matches
     * Space Complexity: O(m * alphabet_size) for trie
     */
    class AhoCorasick {
    public:
        /**
         * Pattern information for tracking multiple patterns
         */
        struct PatternInfo {
            std::string pattern;
            std::string motifName;
            int patternId;

            PatternInfo(const std::string& pat, const std::string& name, int id)
                : pattern(pat), motifName(name), patternId(id) {
            }
        };

        AhoCorasick();
        ~AhoCorasick();

        /**
         * Add a pattern to the trie
         * @param pattern The pattern string
         * @param motifName The name/type of this pattern (e.g., "TATA Box")
         */
        void addPattern(const std::string& pattern, const std::string& motifName);

        /**
         * Build the automaton (compute failure links)
         * Must be called after adding all patterns and before searching
         */
        void buildAutomaton();

        /**
         * Search for all patterns in the text
         * @param text The sequence to search in
         * @return Vector of MatchResult containing all occurrences
         */
        std::vector<MatchResult> search(const std::string& text);

        /**
         * Clear all patterns and reset the automaton
         */
        void clear();

        /**
         * Get the number of patterns currently stored
         */
        size_t getPatternCount() const { return patterns_.size(); }

        /**
         * Get statistics from last search
         */
        size_t getStateTransitions() const { return stateTransitions_; }

    private:
        /**
         * Trie node structure
         */
        struct TrieNode {
            std::unordered_map<char, std::shared_ptr<TrieNode>> children;
            std::shared_ptr<TrieNode> failure;  // Failure link
            std::vector<PatternInfo> output;     // Patterns that end at this node
            int depth;                           // Depth in trie (for debugging)

            TrieNode() : failure(nullptr), depth(0) {}
        };

        std::shared_ptr<TrieNode> root_;
        std::vector<PatternInfo> patterns_;
        size_t stateTransitions_;
        bool isBuilt_;

        /**
         * Insert a pattern into the trie
         */
        void insertPattern(const std::string& pattern, const std::string& motifName, int patternId);

        /**
         * Build failure links using BFS
         */
        void buildFailureLinks();
    };

} // namespace DNACore