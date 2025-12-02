#include "AhoCorasick.h"
#include <algorithm>

namespace DNACore {

    AhoCorasick::AhoCorasick()
        : stateTransitions_(0), isBuilt_(false) {
        root_ = std::make_shared<TrieNode>();
    }

    AhoCorasick::~AhoCorasick() {
    }

    void AhoCorasick::clear() {
        root_ = std::make_shared<TrieNode>();
        patterns_.clear();
        stateTransitions_ = 0;
        isBuilt_ = false;
    }

    void AhoCorasick::addPattern(const std::string& pattern, const std::string& motifName) {
        if (pattern.empty()) {
            return;
        }

        int patternId = static_cast<int>(patterns_.size());
        patterns_.emplace_back(pattern, motifName, patternId);
        insertPattern(pattern, motifName, patternId);
        isBuilt_ = false;  // Need to rebuild automaton
    }

    void AhoCorasick::insertPattern(const std::string& pattern, const std::string& motifName, int patternId) {
        auto current = root_;
        int depth = 0;

        for (char ch : pattern) {
            depth++;

            // Create child node if it doesn't exist
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = std::make_shared<TrieNode>();
                current->children[ch]->depth = depth;
            }

            current = current->children[ch];
        }

        // Mark this node as end of pattern
        current->output.emplace_back(pattern, motifName, patternId);
    }

    void AhoCorasick::buildAutomaton() {
        if (isBuilt_) {
            return;  // Already built
        }

        buildFailureLinks();
        isBuilt_ = true;
    }

    void AhoCorasick::buildFailureLinks() {
        std::queue<std::shared_ptr<TrieNode>> q;

        // Initialize: all children of root have failure link to root
        for (auto& pair : root_->children) {
            pair.second->failure = root_;
            q.push(pair.second);
        }

        // BFS to build failure links
        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            for (auto& pair : current->children) {
                char ch = pair.first;
                auto child = pair.second;

                q.push(child);

                // Find failure link
                auto failureNode = current->failure;

                while (failureNode != nullptr &&
                    failureNode->children.find(ch) == failureNode->children.end()) {
                    if (failureNode == root_) {
                        break;
                    }
                    failureNode = failureNode->failure;
                }

                if (failureNode == nullptr || failureNode == root_) {
                    // Check if root has this character
                    if (root_->children.find(ch) != root_->children.end() &&
                        root_->children[ch] != child) {
                        child->failure = root_->children[ch];
                    }
                    else {
                        child->failure = root_;
                    }
                }
                else {
                    child->failure = failureNode->children[ch];
                }

                // Merge output from failure node
                if (child->failure != nullptr && child->failure != root_) {
                    child->output.insert(
                        child->output.end(),
                        child->failure->output.begin(),
                        child->failure->output.end()
                    );
                }
            }
        }
    }

    std::vector<MatchResult> AhoCorasick::search(const std::string& text) {
        std::vector<MatchResult> results;

        if (text.empty() || patterns_.empty()) {
            return results;
        }

        if (!isBuilt_) {
            buildAutomaton();
        }

        // Reset statistics
        stateTransitions_ = 0;

        auto current = root_;

        for (size_t i = 0; i < text.length(); ++i) {
            char ch = text[i];

            // Follow failure links until we find a match or reach root
            while (current != root_ &&
                current->children.find(ch) == current->children.end()) {
                current = current->failure;
                stateTransitions_++;
            }

            // Transition to next state
            if (current->children.find(ch) != current->children.end()) {
                current = current->children[ch];
                stateTransitions_++;
            }
            else {
                current = root_;
            }

            // Check for matches at current state
            for (const auto& patternInfo : current->output) {
                size_t matchPos = i - patternInfo.pattern.length() + 1;

                results.emplace_back(
                    matchPos,
                    patternInfo.pattern,
                    0,  // editDistance = 0 (exact match)
                    patternInfo.motifName,
                    "Aho-Corasick"
                );
            }
        }

        return results;
    }

} // namespace DNACore