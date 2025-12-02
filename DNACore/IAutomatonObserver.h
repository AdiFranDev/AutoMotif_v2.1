#pragma once

#include <string>
#include <vector>

namespace DNACore {

    /**
     * Observer interface for automaton operations
     * Allows external components to monitor PDA execution
     */
    class IAutomatonObserver {
    public:
        virtual ~IAutomatonObserver() = default;

        // Stack operations
        virtual void onPush(char symbol, int state, size_t position) = 0;
        virtual void onPop(char symbol, int state, size_t position) = 0;

        // State transitions
        virtual void onTransition(int fromState, int toState, char input, size_t position) = 0;

        // Match events
        virtual void onMatchFound(size_t position, const std::string& matched) = 0;
        virtual void onMatchRejected(size_t position, const std::string& reason) = 0;

        // Analysis lifecycle
        virtual void onAnalysisStart(const std::string& sequence, const std::string& pattern) = 0;
        virtual void onAnalysisComplete(size_t matchCount) = 0;
    };

} // namespace DNACore