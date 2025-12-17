#pragma once

#include "IAutomatonObserver.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

namespace DNACore {

    /**
     * Concrete logger implementation for PDA detailed tracing
     * Captures all stack operations and state transitions
     */
    class PDALogger : public IAutomatonObserver {
    public:
        struct LogEntry {
            enum class Type {
                PUSH,
                POP,
                TRANSITION,
                MATCH_FOUND,
                MATCH_REJECTED,
                INFO
            };

            Type type;
            size_t position;
            int state;
            char symbol;
            std::string message;
            size_t stackDepth;

            LogEntry(Type t, size_t pos, int st, char sym, const std::string& msg, size_t depth)
                : type(t), position(pos), state(st), symbol(sym), message(msg), stackDepth(depth) {
            }
        };

        PDALogger();
        virtual ~PDALogger();

        // IAutomatonObserver implementation
        void onPush(char symbol, int state, size_t position) override;
        void onPop(char symbol, int state, size_t position) override;
        void onTransition(int fromState, int toState, char input, size_t position) override;
        void onMatchFound(size_t position, const std::string& matched) override;
        void onMatchRejected(size_t position, const std::string& reason) override;
        void onAnalysisStart(const std::string& sequence, const std::string& pattern) override;
        void onAnalysisComplete(size_t matchCount) override;

        // Access log
        const std::vector<LogEntry>& getLog() const { return entries_; }
        void clear();

        // Reset stack depth (called when PDA resets for new position)
        void resetStackDepth();

        // Formatted output
        std::string formatLog() const;
        std::string formatLogFormatted() const;

        // Statistics
        size_t entryCount() const { return entries_.size(); }
        size_t getPushCount() const;
        size_t getPopCount() const;

    private:
        std::vector<LogEntry> entries_;
        size_t currentStackDepth_;
    };

} // namespace DNACore