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
        while (!stack_.empty()) {
            stack_.pop();
        }
        stack_.push(StackSymbol::BOTTOM);
        notifyPush('$', 0);
        currentState_ = 0;
    }

    PushdownAutomaton::StackSymbol PushdownAutomaton::charToSymbol(char c) {
        switch (c) {
        case 'A': return StackSymbol::A;
        case 'T': return StackSymbol::T;
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
        case StackSymbol::C: return 'C';
        case StackSymbol::U: return 'U';
        case StackSymbol::BOTTOM: return '$';
        default: return '?';
        }
    }

    // ===== PATTERN MATCHING =====

    bool PushdownAutomaton::matchWithStack(const std::string& text, size_t start, const std::string& pattern) {
        for (size_t i = 0; i < pattern.length(); ++i) {
            size_t textPos = start + i;

            if (textPos >= text.length()) {
                notifyMatchRejected(textPos, "Sequence too short");
                return false;
            }

            char textChar = text[textPos];
            char patternChar = pattern[i];

            if (textChar != patternChar) {
                notifyMatchRejected(textPos,
                    std::string("Mismatch: expected ") + patternChar + ", got " + textChar);
                return false;
            }

            // State transition
            notifyTransition(currentState_, currentState_ + 1, textChar, textPos);
            currentState_++;

            // Push to stack
            auto sym = charToSymbol(textChar);
            stack_.push(sym);
            notifyPush(textChar, textPos);
        }

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

        // Search all positions
        for (size_t i = 0; i <= n - m; ++i) {
            reset();

            if (matchWithStack(text, i, pattern)) {
                results.emplace_back(
                    i,
                    text.substr(i, m),
                    0,
                    "PDA Match",
                    "PDA"
                );
                notifyMatchFound(i, text.substr(i, m));
            }
        }

        notifyAnalysisComplete(results.size());
        return results;
    }

} // namespace DNACore