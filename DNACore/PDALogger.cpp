#include "PDALogger.h"
#include <iomanip>

namespace DNACore {

    PDALogger::PDALogger()
        : currentStackDepth_(0) {
    }

    PDALogger::~PDALogger() {
    }

    void PDALogger::clear() {
        entries_.clear();
        currentStackDepth_ = 0;
    }

    void PDALogger::resetStackDepth() {
        currentStackDepth_ = 0;
    }

    void PDALogger::onPush(char symbol, int state, size_t position) {
        // RESET stack depth when we see '$' (bottom marker - new attempt)
        if (symbol == '$') {
            currentStackDepth_ = 0;  // Start from 0, will increment below
        }
        
        currentStackDepth_++;  // Increment for this push
        
        entries_.emplace_back(
            LogEntry::Type::PUSH,
            position,
            state,
            symbol,
            "PUSH '" + std::string(1, symbol) + "'",
            currentStackDepth_
        );
    }

    void PDALogger::onPop(char symbol, int state, size_t position) {
        // Decrement stack depth BEFORE logging (so we show the depth after pop)
        currentStackDepth_ = (currentStackDepth_ > 0) ? currentStackDepth_ - 1 : 0;
        
        // Add POP entry - this MUST show in output
        entries_.emplace_back(
            LogEntry::Type::POP,
            position,
            state,
            symbol,
            "POP  '" + std::string(1, symbol) + "'",
            currentStackDepth_
        );
    }

    void PDALogger::onTransition(int fromState, int toState, char input, size_t position) {
        entries_.emplace_back(
            LogEntry::Type::TRANSITION,
            position,
            toState,
            input,
            "Transition: q" + std::to_string(fromState) + " -> q" + std::to_string(toState) +
            " on '" + std::string(1, input) + "'",
            currentStackDepth_
        );
    }

    void PDALogger::onMatchFound(size_t position, const std::string& matched) {
        entries_.emplace_back(
            LogEntry::Type::MATCH_FOUND,
            position,
            -1,
            '\0',
            ">>> MATCH ACCEPTED <<<",
            currentStackDepth_
        );
    }

    void PDALogger::onMatchRejected(size_t position, const std::string& reason) {
        entries_.emplace_back(
            LogEntry::Type::MATCH_REJECTED,
            position,
            -1,
            '\0',
            ">>> REJECTED: " + reason,
            currentStackDepth_
        );
    }

    void PDALogger::onAnalysisStart(const std::string& sequence, const std::string& pattern) {
        entries_.clear();
        currentStackDepth_ = 0;
        entries_.emplace_back(
            LogEntry::Type::INFO,
            0,
            0,
            '\0',
            "=== PDA ANALYSIS START ===\nSequence Length: " + std::to_string(sequence.length()) +
            "\nPattern: " + pattern,
            0
        );
    }

    void PDALogger::onAnalysisComplete(size_t matchCount) {
        entries_.emplace_back(
            LogEntry::Type::INFO,
            0,
            -1,
            '\0',
            "=== ANALYSIS COMPLETE ===\nTotal Matches: " + std::to_string(matchCount),
            0
        );
    }

    std::string PDALogger::formatLog() const {
        std::ostringstream oss;

        for (size_t i = 0; i < entries_.size(); ++i) {
            const auto& entry = entries_[i];
            oss << "[" << (i + 1) << "] ";

            // Add position prefix
            if (entry.type == LogEntry::Type::INFO) {
                oss << "          ";
            }
            else if (entry.type == LogEntry::Type::PUSH && entry.symbol == '$') {
                oss << "          ";  // '$' PUSH doesn't show position
            }
            else if (entry.type == LogEntry::Type::POP && entry.symbol == '$') {
                oss << "          ";  // '$' POP doesn't show position
            }
            else if (entry.type == LogEntry::Type::MATCH_FOUND || entry.type == LogEntry::Type::MATCH_REJECTED) {
                oss << "          ";  // Match results don't show position
            }
            else {
                // Show position for transitions and character PUSH/POP
                oss << "Pos " << std::setw(3) << entry.position << ": ";
            }

            // Add indentation based on stack depth
            size_t indent = (entry.stackDepth < 20) ? entry.stackDepth : 20;
            for (size_t j = 0; j < indent; ++j) {
                oss << " ";  // Single space per indent level
            }

            oss << entry.message;

            // Add stack depth for PUSH/POP with extra spacing for alignment
            if (entry.type == LogEntry::Type::PUSH || entry.type == LogEntry::Type::POP) {
                oss << " [Stack: " << entry.stackDepth << "]";
            }

            oss << "\n";
        }

        return oss.str();
    }

    std::string PDALogger::formatLogFormatted() const {
        return formatLog();
    }

    size_t PDALogger::getPushCount() const {
        size_t count = 0;
        for (const auto& entry : entries_) {
            if (entry.type == LogEntry::Type::PUSH) count++;
        }
        return count;
    }

    size_t PDALogger::getPopCount() const {
        size_t count = 0;
        for (const auto& entry : entries_) {
            if (entry.type == LogEntry::Type::POP) count++;
        }
        return count;
    }

} // namespace DNACore