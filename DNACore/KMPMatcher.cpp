#include "KMPMatcher.h"
#include <algorithm>

namespace DNACore {

    KMPMatcher::KMPMatcher()
        : comparisons_(0), shifts_(0) {
    }

    KMPMatcher::~KMPMatcher() {
    }

    void KMPMatcher::computeLPSArray(const std::string& pattern) {
        size_t m = pattern.length();
        lps_.clear();
        lps_.resize(m, 0);

        size_t len = 0;  // Length of previous longest prefix suffix
        size_t i = 1;

        // lps_[0] is always 0
        while (i < m) {
            if (pattern[i] == pattern[len]) {
                len++;
                lps_[i] = static_cast<int>(len);
                i++;
            }
            else {
                if (len != 0) {
                    len = lps_[len - 1];
                }
                else {
                    lps_[i] = 0;
                    i++;
                }
            }
        }
    }

    std::vector<MatchResult> KMPMatcher::search(const std::string& text, const std::string& pattern) {
        std::vector<MatchResult> results;

        if (pattern.empty() || text.empty()) {
            return results;
        }

        size_t n = text.length();
        size_t m = pattern.length();

        if (m > n) {
            return results;
        }

        // Reset statistics
        comparisons_ = 0;
        shifts_ = 0;

        // Preprocess pattern
        computeLPSArray(pattern);

        // Search
        size_t i = 0;  // Index for text
        size_t j = 0;  // Index for pattern

        while (i < n) {
            comparisons_++;

            if (pattern[j] == text[i]) {
                i++;
                j++;
            }

            if (j == m) {
                // Match found
                size_t matchPos = i - j;
                results.emplace_back(
                    matchPos,
                    text.substr(matchPos, m),
                    0,           // editDistance = 0 (exact match)
                    "Exact Match",
                    "KMP"
                );

                j = lps_[j - 1];
                shifts_++;
            }
            else if (i < n && pattern[j] != text[i]) {
                if (j != 0) {
                    j = lps_[j - 1];
                    shifts_++;
                }
                else {
                    i++;
                }
            }
        }

        return results;
    }

} // namespace DNACore