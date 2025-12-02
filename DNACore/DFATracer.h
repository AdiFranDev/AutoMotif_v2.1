#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "AnalysisTypes.h"

namespace DNACore {

    /**
     * DFA Tracer for summary-level tracing
     * Used by KMP and Aho-Corasick algorithms
     * Does NOT track individual state transitions - only high-level events
     */
    class DFATracer {
    public:
        DFATracer();
        ~DFATracer();

        /**
         * Record the start of analysis
         */
        void recordStart(const std::string& algorithm, const std::string& sequence,
            const std::string& pattern);

        /**
         * Record a match found
         */
        void recordMatch(size_t position, const std::string& matched);

        /**
         * Record analysis completion
         */
        void recordComplete(size_t totalMatches, size_t comparisons = 0);

        /**
         * Clear all trace data
         */
        void clear();

        /**
         * Get formatted trace summary
         */
        std::string getTraceSummary() const;

        /**
         * Get statistics
         */
        size_t getMatchCount() const { return matchCount_; }
        size_t getComparisons() const { return comparisons_; }

    private:
        std::string algorithm_;
        size_t sequenceLength_;
        std::string pattern_;
        std::vector<std::pair<size_t, std::string>> matches_;  // position, matched string
        size_t matchCount_;
        size_t comparisons_;
    };

} // namespace DNACore