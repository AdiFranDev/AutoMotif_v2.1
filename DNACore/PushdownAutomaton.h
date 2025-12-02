#pragma once

#include <stack>
#include <string>
#include <vector>
#include "AnalysisTypes.h"
#include "IAutomatonObserver.h"

namespace DNACore {

    /**
     * Pushdown Automaton for pattern matching with stack operations
     * Provides detailed tracing through observer pattern
     */
    class PushdownAutomaton {
    public:
        PushdownAutomaton();
        ~PushdownAutomaton();

        /**
         * Search for pattern in text using PDA with stack
         * @param text The sequence to search
         * @param pattern The pattern to match
         * @return Vector of matches found
         */
        std::vector<MatchResult> search(const std::string& text, const std::string& pattern);

        /**
         * Observer management
         */
        void attachObserver(IAutomatonObserver* observer);
        void detachObserver(IAutomatonObserver* observer);
        void clearObservers();

    private:
        /**
         * Stack symbol types
         */
        enum class StackSymbol {
            BOTTOM,
            A, T, G, C, U,
            MARKER
        };

        std::stack<StackSymbol> stack_;
        int currentState_;
        std::vector<IAutomatonObserver*> observers_;

        /**
         * Notification helpers
         */
        void notifyPush(char symbol, size_t position);
        void notifyPop(char symbol, size_t position);
        void notifyTransition(int fromState, int toState, char input, size_t position);
        void notifyMatchFound(size_t position, const std::string& matched);
        void notifyMatchRejected(size_t position, const std::string& reason);
        void notifyAnalysisStart(const std::string& sequence, const std::string& pattern);
        void notifyAnalysisComplete(size_t matchCount);

        /**
         * Stack management
         */
        void reset();
        StackSymbol charToSymbol(char c);
        char symbolToChar(StackSymbol sym);

        /**
         * Pattern matching with stack
         */
        bool matchWithStack(const std::string& text, size_t start, const std::string& pattern);
    };

} // namespace DNACore