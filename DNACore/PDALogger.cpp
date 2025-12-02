#include "PDALogger.h"

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

    void PDALogger::onPush(char symbol, int state, size_t position) {
        currentStackDepth_++;
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
        currentStackDepth_ = (currentStackDepth_ > 0) ? currentStackDepth_ - 1 : 0;
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
            "MATCH ACCEPTED at position " + std::to_string(position) + ": " + matched,
            currentStackDepth_
        );
    }

    void PDALogger::onMatchRejected(size_t position, const std::string& reason) {
        entries_.emplace_back(
            LogEntry::Type::MATCH_REJECTED,
            position,
            -1,
            '\0',
            "MATCH REJECTED: " + reason,
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

            if (entry.position > 0) {
                oss << "Pos " << entry.position << ": ";
            }

            // Indentation based on stack depth
            for (size_t d = 0; d < entry.stackDepth * 2; ++d) {
                oss << " ";
            }

            oss << entry.message << "\n";
        }

        return oss.str();
    }

    std::string PDALogger::formatLogFormatted() const {
        std::ostringstream oss;

        oss << "[1]  === PDA ANALYSIS START ===\n";

        // Extract sequence info from first INFO entry
        for (const auto& entry : entries_) {
            if (entry.type == LogEntry::Type::INFO &&
                entry.message.find("Sequence Length") != std::string::npos) {
                size_t lenPos = entry.message.find("Sequence Length: ");
                size_t patPos = entry.message.find("Pattern: ");

                if (lenPos != std::string::npos) {
                    size_t start = lenPos + 17;
                    size_t end = entry.message.find('\n', start);
                    if (end == std::string::npos) end = entry.message.length();
                    oss << "     Sequence Length: " << entry.message.substr(start, end - start) << "\n";
                }

                if (patPos != std::string::npos) {
                    size_t start = patPos + 9;
                    oss << "     Pattern: " << entry.message.substr(start) << "\n";
                }
                break;
            }
        }

        int lineNum = 2;

        for (const auto& entry : entries_) {
            if (entry.type == LogEntry::Type::INFO) continue;

            oss << "[" << lineNum++ << "] ";

            if (entry.position > 0) {
                char posStr[20];
                snprintf(posStr, sizeof(posStr), "Pos %3zu: ", entry.position);
                oss << posStr;
            }
            else {
                oss << "         ";
            }

            // Indentation
            for (size_t d = 0; d < entry.stackDepth * 2; ++d) {
                oss << " ";
            }

            // Type-specific formatting
            switch (entry.type) {
            case LogEntry::Type::PUSH: {
                char opStr[50];
                snprintf(opStr, sizeof(opStr), "PUSH '%c' [Stack: %zu]",
                    entry.symbol, entry.stackDepth);
                oss << opStr;
                break;
            }

            case LogEntry::Type::POP: {
                char opStr[50];
                snprintf(opStr, sizeof(opStr), "POP  '%c' [Stack: %zu]",
                    entry.symbol, entry.stackDepth);
                oss << opStr;
                break;
            }

            case LogEntry::Type::TRANSITION: {
                char opStr[100];
                snprintf(opStr, sizeof(opStr), "Transition: q%d -> q%d on '%c'",
                    entry.state - 1, entry.state, entry.symbol);
                oss << opStr;
                break;
            }

            case LogEntry::Type::MATCH_FOUND:
                oss << ">>> MATCH ACCEPTED <<<";
                break;

            case LogEntry::Type::MATCH_REJECTED:
                oss << ">>> REJECTED: " << entry.message;
                break;

            default:
                oss << entry.message;
                break;
            }

            oss << "\n";
        }

        oss << "[" << lineNum << "] === ANALYSIS COMPLETE ===\n";
        return oss.str();
    }

    size_t PDALogger::getPushCount() const {
        size_t count = 0;
        for (const auto& entry : entries_) {
            if (entry.type == LogEntry::Type::PUSH) {
                count++;
            }
        }
        return count;
    }

    size_t PDALogger::getPopCount() const {
        size_t count = 0;
        for (const auto& entry : entries_) {
            if (entry.type == LogEntry::Type::POP) {
                count++;
            }
        }
        return count;
    }

} // namespace DNACore