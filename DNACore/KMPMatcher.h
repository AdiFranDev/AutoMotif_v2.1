#pragma once

#include <string>
#include <vector>
#include "AnalysisTypes.h"

namespace DNACore {

    /**
     * Knuth-Morris-Pratt String Matching Algorithm
     * Time Complexity: O(n + m) where n = text length, m = pattern length
     * Space Complexity: O(m) for LPS array
     */
    class KMPMatcher {
    public:
        KMPMatcher();
        ~KMPMatcher();

        /**
         * Search for a single pattern in text using KMP algorithm
         * @param text The sequence to search in
         * @param pattern The pattern to find
         * @return Vector of MatchResult containing all occurrences
         */
        std::vector<MatchResult> search(const std::string& text, const std::string& pattern);

        /**
         * Get the LPS (Longest Proper Prefix which is also Suffix) array
         * Useful for debugging and trace output
         */
        const std::vector<int>& getLPSArray() const { return lps_; }

        /**
         * Get statistics about the last search
         */
        size_t getComparisons() const { return comparisons_; }
        size_t getShifts() const { return shifts_; }

    private:
        std::vector<int> lps_;        // LPS array for current pattern
        size_t comparisons_;          // Number of character comparisons
        size_t shifts_;               // Number of pattern shifts

        /**
         * Compute the LPS (Longest Proper Prefix which is also Suffix) array
         * This is the preprocessing step of KMP
         */
        void computeLPSArray(const std::string& pattern);
    };

} // namespace DNACore