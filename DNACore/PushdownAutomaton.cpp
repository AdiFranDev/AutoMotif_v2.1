#include "PushdownAutomaton.h"
#include <algorithm>

namespace DNACore {

    PushdownAutomaton::PushdownAutomaton()
        : currentState_(0) {
    }

    PushdownAutomaton::~PushdownAutomaton() {
        clearObservers();
    }

    // ===== OBSERVER MANAGEMENT =====

    void PushdownAutomaton::attachObserver(IAutomatonObserver* observer) {
        if (observer) {
            observers_.push_back(observer);
        }
    }

    void PushdownAutomaton::detachObserver(IAutomatonObserver* observer) {
        observers_.erase(
            std::remove(observers_.begin(), observers_.end(), observer),
            observers_.end()
        );
    }

    void PushdownAutomaton::clearObservers() {
        observers_.clear();
    }

    // ===== NOTIFICATION HELPERS =====

    void PushdownAutomaton::notifyPush(char symbol, size_t position) {
        for (auto* obs : observers_) {
            obs->onPush(symbol, currentState_, position);
        }
    }

    void PushdownAutomaton::notifyPop(char symbol, size_t position) {
        for (auto* obs : observers_) {
            obs->onPop(symbol, currentState_, position);
        }
    }

    void PushdownAutomaton::notifyTransition(int fromState, int toState, char input, size_t position) {
        for (auto* obs : observers_) {
            obs->onTransition(fromState, toState, input, position);
        }
    }

    void PushdownAutomaton::notifyMatchFound(size_t position, const std::string& matched) {
        for (auto* obs : observers_) {
            obs->onMatchFound(position, matched);
        }
    }

    void PushdownAutomaton::notifyMatchRejected(size_t position, const std::string& reason) {
        for (auto* obs : observers_) {
            obs->onMatchRejected(position, reason);
        }
    }

    void PushdownAutomaton::notifyAnalysisStart(const std::string& sequence, const std::string& pattern) {
        for (auto* obs : observers_) {
            obs->onAnalysisStart(sequence, pattern);
        }
    }

    void PushdownAutomaton::notifyAnalysisComplete(size_t matchCount) {
        for (auto* obs : observers_) {
            obs->onAnalysisComplete(matchCount);
        }
    }

    // ===== STACK MANAGEMENT =====

    void PushdownAutomaton::reset() {
        // Clear the actual stack
        while (!stack_.empty()) {
            stack_.pop();
        }
        // Push bottom marker
        stack_.push(StackSymbol::BOTTOM);
        currentState_ = 0;
    }

    PushdownAutomaton::StackSymbol PushdownAutomaton::charToSymbol(char c) {
        switch (c) {
        case 'A': return StackSymbol::A;
        case 'T':  return StackSymbol::T;
        case 'G': return StackSymbol::G;
        case 'C': return StackSymbol::C;
        case 'U': return StackSymbol::U;
        default: return StackSymbol::BOTTOM;
        }
    }

    char PushdownAutomaton::symbolToChar(StackSymbol sym) {
        switch (sym) {
        case StackSymbol::A: return 'A';
        case StackSymbol::T: return 'T';
        case StackSymbol::G: return 'G';
        case StackSymbol::C:  return 'C';
        case StackSymbol::U: return 'U';
        case StackSymbol::BOTTOM:  return '$';
        default: return '?';
        }
    }

    // ===== PATTERN MATCHING WITH PROPER PUSH/POP =====

    bool PushdownAutomaton::matchWithStack(const std::string& text, size_t start, const std::string& pattern) {
        // Notify PUSH of bottom marker '$' - this signals new attempt
        notifyPush('$', start);

        // Track pushed characters for POP
        size_t pushedCount = 0;

        for (size_t i = 0; i < pattern.length(); ++i) {
            size_t textPos = start + i;

            if (textPos >= text.length()) {
                // POP everything before rejecting (in reverse order)
                for (size_t j = pushedCount; j > 0; --j) {
                    StackSymbol top = stack_.top();
                    stack_.pop();
                    notifyPop(symbolToChar(top), textPos);
                }
                // ALWAYS POP the bottom marker '$'
                notifyPop('$', start);
                notifyMatchRejected(textPos, "Sequence too short");
                return false;
            }

            char textChar = text[textPos];
            char patternChar = pattern[i];

            if (textChar != patternChar) {
                // POP everything before rejecting (in reverse order)
                for (size_t j = pushedCount; j > 0; --j) {
                    StackSymbol top = stack_.top();
                    stack_.pop();
                    notifyPop(symbolToChar(top), start + j - 1);
                }
                // ALWAYS POP the bottom marker '$'
                notifyPop('$', start);
                notifyMatchRejected(start,
                    std::string("Mismatch: expected ") + patternChar + ", got " + textChar);
                return false;
            }

            // Match!  Transition and push
            notifyTransition(currentState_, currentState_ + 1, textChar, textPos);
            currentState_++;

            auto sym = charToSymbol(textChar);
            stack_.push(sym);
            notifyPush(textChar, textPos);
            pushedCount++;
        }

        // SUCCESS! POP all characters in reverse order FIRST
        size_t popPosition = start + pattern.length() - 1;
        for (size_t j = 0; j < pushedCount; ++j) {
            StackSymbol top = stack_.top();
            stack_.pop();
            notifyPop(symbolToChar(top), popPosition - j);
        }
        
        // ALWAYS POP the bottom marker '$'
        notifyPop('$', start);
        
        // THEN notify match found (so POPs appear before "MATCH ACCEPTED")
        notifyMatchFound(start, text.substr(start, pattern.length()));

        return true;
    }

    std::vector<MatchResult> PushdownAutomaton::search(const std::string& text, const std::string& pattern) {
        std::vector<MatchResult> results;

        notifyAnalysisStart(text, pattern);

        if (pattern.empty() || text.empty()) {
            notifyAnalysisComplete(0);
            return results;
        }

        size_t n = text.length();
        size_t m = pattern.length();

        if (m > n) {
            notifyAnalysisComplete(0);
            return results;
        }

        // Search at all valid positions
        for (size_t i = 0; i <= n - m; ++i) {
            reset();  // Reset stack and state for each new position

            if (matchWithStack(text, i, pattern)) {
                results.emplace_back(
                    i,
                    text.substr(i, m),
                    0,
                    "PDA Match",
                    "PDA"
                );
                // notifyMatchFound is now called inside matchWithStack
            }
        }

        notifyAnalysisComplete(results.size());
        return results;
    }

} // namespace DNACore