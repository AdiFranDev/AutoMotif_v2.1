#pragma once

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cctype>
#include "AnalysisTypes.h"
#include "MotifDatabase.h"
#include "KMPMatcher.h"
#include "AhoCorasick.h"
#include "PushdownAutomaton.h"
#include "PDALogger.h"
#include "DFATracer.h"

namespace DNACore {

    /**
     * Main sequence analysis coordinator
     * Manages all algorithms and provides unified interface
     */
    class SequenceAnalyzer {
    public:
        SequenceAnalyzer();
        ~SequenceAnalyzer();

        /**
         * Set the sequence to analyze
         * Automatically converts to uppercase
         */
        void setSequence(const std::string& sequence);

        /**
         * Get current sequence
         */
        const std::string& getSequence() const { return sequence_; }

        // ===== SEARCH METHODS =====

        /**
         * Exact pattern matching using KMP algorithm
         */
        std::vector<MatchResult> exactMatch(const std::string& pattern);

        /**
         * Approximate matching using edit distance
         */
        std::vector<MatchResult> approximateMatch(const std::string& pattern, int maxDistance);

        /**
         * Search for known motifs using Aho-Corasick
         */
        std::vector<MatchResult> searchMotif(int motifType);

        /**
         * Search for all known motifs
         */
        std::vector<MatchResult> searchAllMotifs();

        /**
         * PDA-based search with detailed tracing
         */
        std::vector<MatchResult> pushdownSearch(const std::string& pattern);

        // ===== NEW:  REGEX SEARCH ===== (ADD THIS SECTION)

        /**
         * Search using regex pattern (compiled to NFA)
         * @param pattern Regex pattern (e.g., "A(T|G)*C")
         * @return Vector of matches
         */
        std::vector<MatchResult> regexSearch(const std::string& pattern);

        /**
         * Get NFA description for visualization
         */
        std::string getNFADescription() const;

        /**
         * Get regex engine trace
         */
        std::string getRegexTrace() const;

        /**
         * Get regex compilation error (if any)
         */
        std::string getRegexError() const;

        // ===== STATISTICS =====

        /**
         * Calculate sequence statistics
         */
        SequenceStatistics getStatistics() const;

        /**
         * Calculate GC content percentage
         */
        double calculateGCContent() const;

        // ===== TRACERS =====

        DFATracer* getDFATracer() { return &dfaTracer_; }
        PDALogger* getPDALogger() { return &pdaLogger_; }
        PushdownAutomaton* getPDA() { return pda_.get(); }

    private:
        std::string sequence_;

        // Algorithm instances
        std::unique_ptr<KMPMatcher> kmpMatcher_;
        std::unique_ptr<AhoCorasick> ahoCorasick_;
        std::unique_ptr<PushdownAutomaton> pda_;

        // Tracers
        DFATracer dfaTracer_;
        PDALogger pdaLogger_;

        // Helper methods
        std::vector<MatchResult> fuzzySearch(const std::string& pattern, int maxDistance);
        int editDistance(const std::string& s1, const std::string& s2);
        std::string toUpperCase(const std::string& str) const;
    };

} // namespace DNACore