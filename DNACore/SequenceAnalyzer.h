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
         * @param pattern The pattern to search for
         * @return Vector of exact matches
         */
        std::vector<MatchResult> exactMatch(const std::string& pattern);

        /**
         * Approximate matching using edit distance
         * @param pattern The pattern to search for
         * @param maxDistance Maximum edit distance allowed
         * @return Vector of approximate matches
         */
        std::vector<MatchResult> approximateMatch(const std::string& pattern, int maxDistance);

        /**
         * Search for known motifs using Aho-Corasick
         * @param motifType Motif type from MotifDatabase::MotifType (1-5)
         * @return Vector of motif matches
         */
        std::vector<MatchResult> searchMotif(int motifType);

        /**
         * Search for all known motifs
         * @return Vector of all motif matches found
         */
        std::vector<MatchResult> searchAllMotifs();

        /**
         * PDA-based search with detailed tracing
         * @param pattern The pattern to search for
         * @return Vector of matches
         */
        std::vector<MatchResult> pushdownSearch(const std::string& pattern);

        // ===== STATISTICS =====

        /**
         * Calculate sequence statistics
         * @return SequenceStatistics structure
         */
        SequenceStatistics getStatistics() const;

        /**
         * Calculate GC content percentage
         */
        double calculateGCContent() const;

        // ===== TRACERS =====

        /**
         * Get DFA tracer for summary traces (KMP, Aho-Corasick)
         */
        DFATracer* getDFATracer() { return &dfaTracer_; }

        /**
         * Get PDA logger for detailed traces
         */
        PDALogger* getPDALogger() { return &pdaLogger_; }

        /**
         * Get PDA instance for observer attachment
         */
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