#include "DFATracer.h"

namespace DNACore {

    DFATracer::DFATracer()
        : sequenceLength_(0), matchCount_(0), comparisons_(0) {
    }

    DFATracer::~DFATracer() {
    }

    void DFATracer::clear() {
        algorithm_.clear();
        pattern_.clear();
        matches_.clear();
        sequenceLength_ = 0;
        matchCount_ = 0;
        comparisons_ = 0;
    }

    void DFATracer::recordStart(const std::string& algorithm, const std::string& sequence,
        const std::string& pattern) {
        clear();
        algorithm_ = algorithm;
        sequenceLength_ = sequence.length();
        pattern_ = pattern;
    }

    void DFATracer::recordMatch(size_t position, const std::string& matched) {
        matches_.emplace_back(position, matched);
        matchCount_++;
    }

    void DFATracer::recordComplete(size_t totalMatches, size_t comparisons) {
        matchCount_ = totalMatches;
        comparisons_ = comparisons;
    }

    std::string DFATracer::getTraceSummary() const {
        std::ostringstream oss;

        oss << "[1] === DFA ANALYSIS START ===\n";
        oss << "    Sequence Length: " << sequenceLength_ << "\n";
        oss << "    Pattern: " << pattern_ << "\n";
        oss << "    Algorithm: " << algorithm_ << "\n\n";

        oss << "[2] Building automaton...\n";

        if (algorithm_ == "KMP") {
            oss << "[3] LPS array computed\n\n";
        }
        else if (algorithm_ == "Aho-Corasick") {
            oss << "[3] Trie built with " << pattern_.length() << " nodes\n";
            oss << "[4] Failure links computed\n\n";
        }

        oss << "[" << (algorithm_ == "KMP" ? 4 : 5) << "] Processing sequence...\n";

        if (matches_.empty()) {
            oss << "    No matches found\n";
        }
        else {
            int lineNum = (algorithm_ == "KMP" ? 5 : 6);
            for (const auto& match : matches_) {
                oss << "[" << lineNum++ << "] Pos " << match.first
                    << ": ? MATCH FOUND - " << match.second << "\n";
            }
        }

        oss << "\n[" << (algorithm_ == "KMP" ? 5 : 6) + static_cast<int>(matches_.size())
            << "] === ANALYSIS COMPLETE ===\n";
        oss << "    Total Matches: " << matchCount_ << "\n";

        if (comparisons_ > 0) {
            oss << "    Comparisons: " << comparisons_ << "\n";
        }

        oss << "    Complexity: O(n+m)\n";

        return oss.str();
    }

} // namespace DNACore