#include "SequenceAnalyzer.h"

namespace DNACore {

    SequenceAnalyzer::SequenceAnalyzer()
        : kmpMatcher_(std::make_unique<KMPMatcher>()),
          ahoCorasick_(std::make_unique<AhoCorasick>()),
          pda_(std::make_unique<PushdownAutomaton>()),
    }

    SequenceAnalyzer::~SequenceAnalyzer() {
    }

    // ===== SEQUENCE MANAGEMENT =====

    void SequenceAnalyzer::setSequence(const std::string& sequence) {
        sequence_ = toUpperCase(sequence);
    }

    std::string SequenceAnalyzer::toUpperCase(const std::string& str) const {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    // ===== EXACT MATCHING (KMP) =====

    std::vector<MatchResult> SequenceAnalyzer::exactMatch(const std::string& pattern) {
        if (sequence_.empty() || pattern.empty()) {
            return {};
        }

        std::string upperPattern = toUpperCase(pattern);

        // Record trace start
        dfaTracer_.recordStart("KMP", sequence_, upperPattern);

        // Perform KMP search
        auto results = kmpMatcher_->search(sequence_, upperPattern);

        // Record matches
        for (const auto& result : results) {
            dfaTracer_.recordMatch(result.position, result.matchedSequence);
        }

        // Record completion
        dfaTracer_.recordComplete(results.size(), kmpMatcher_->getComparisons());

        return results;
    }

    // ===== APPROXIMATE MATCHING =====

    std::vector<MatchResult> SequenceAnalyzer::approximateMatch(const std::string& pattern, int maxDistance) {
        if (sequence_.empty() || pattern.empty()) {
            return {};
        }

        std::string upperPattern = toUpperCase(pattern);

        // Record trace
        dfaTracer_.recordStart("Levenshtein", sequence_, upperPattern);

        // Perform fuzzy search
        auto results = fuzzySearch(upperPattern, maxDistance);

        // Record matches
        for (const auto& result : results) {
            dfaTracer_.recordMatch(result.position, result.matchedSequence);
        }

        dfaTracer_.recordComplete(results.size());

        return results;
    }

    std::vector<MatchResult> SequenceAnalyzer::fuzzySearch(const std::string& pattern, int maxDistance) {
        std::vector<MatchResult> results;

        size_t n = sequence_.length();
        size_t m = pattern.length();

        // Check all substrings of similar length
        for (size_t i = 0; i < n; ++i) {
            // Try different lengths around pattern length
            for (size_t len = (m > maxDistance ? m - maxDistance : 1);
                len <= m + maxDistance && i + len <= n;
                ++len) {

                std::string substring = sequence_.substr(i, len);
                int dist = editDistance(substring, pattern);

                if (dist <= maxDistance) {
                    results.emplace_back(
                        i,
                        substring,
                        dist,
                        "Approx Match (dist=" + std::to_string(dist) + ")",
                        "Levenshtein"
                    );
                }
            }
        }

        return results;
    }

    int SequenceAnalyzer::editDistance(const std::string& s1, const std::string& s2) {
        size_t m = s1.length();
        size_t n = s2.length();

        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

        // Initialize base cases
        for (size_t i = 0; i <= m; ++i) dp[i][0] = static_cast<int>(i);
        for (size_t j = 0; j <= n; ++j) dp[0][j] = static_cast<int>(j);

        // Fill DP table
        for (size_t i = 1; i <= m; ++i) {
            for (size_t j = 1; j <= n; ++j) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else {
                    dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
                }
            }
        }

        return dp[m][n];
    }

    // ===== MOTIF SEARCH (AHO-CORASICK) =====

    std::vector<MatchResult> SequenceAnalyzer::searchMotif(int motifType) {
        if (sequence_.empty()) {
            return {};
        }

        // Clear previous patterns
        ahoCorasick_->clear();

        // Get motif
        auto motif = MotifDatabase::getMotif(static_cast<MotifDatabase::MotifType>(motifType));

        if (motif.pattern.empty()) {
            return {};
        }

        // Add pattern to Aho-Corasick
        ahoCorasick_->addPattern(motif.pattern, motif.name);
        ahoCorasick_->buildAutomaton();

        // Record trace
        dfaTracer_.recordStart("Aho-Corasick", sequence_, motif.pattern);

        // Search
        auto results = ahoCorasick_->search(sequence_);

        // Record matches
        for (const auto& result : results) {
            dfaTracer_.recordMatch(result.position, result.matchedSequence);
        }

        dfaTracer_.recordComplete(results.size(), ahoCorasick_->getStateTransitions());

        return results;
    }

    std::vector<MatchResult> SequenceAnalyzer::searchAllMotifs() {
        if (sequence_.empty()) {
            return {};
        }

        // Clear and build automaton with all motifs
        ahoCorasick_->clear();

        auto allMotifs = MotifDatabase::getAllMotifs();
        
        // Add each motif pattern
        for (const auto& motif : allMotifs) {
            if (!motif.pattern.empty()) {
                ahoCorasick_->addPattern(motif.pattern, motif.name);
            }
        }

        ahoCorasick_->buildAutomaton();

        // Record trace
        dfaTracer_.recordStart("Aho-Corasick (Multi-Pattern)", sequence_, "Multiple Motifs");

        // Search
        auto results = ahoCorasick_->search(sequence_);

        // Record matches
        for (const auto& result : results) {
            dfaTracer_.recordMatch(result.position, result.matchedSequence);
        }

        dfaTracer_.recordComplete(results.size(), ahoCorasick_->getStateTransitions());

        return results;
    }

    // ===== PDA SEARCH =====

    std::vector<MatchResult> SequenceAnalyzer::pushdownSearch(const std::string& pattern) {
        if (sequence_.empty() || pattern.empty()) {
            return {};
        }

        std::string upperPattern = toUpperCase(pattern);

        // PDA uses its own observer pattern
        // The observer (PDALogger) should be attached externally
        return pda_->search(sequence_, upperPattern);
    }

    // ===== STATISTICS =====

    double SequenceAnalyzer::calculateGCContent() const {
        if (sequence_.empty()) {
            return 0.0;
        }

        size_t gcCount = 0;
        for (char c : sequence_) {
            if (c == 'G' || c == 'C') {
                gcCount++;
            }
        }

        return (gcCount * 100.0) / sequence_.length();
    }

    SequenceStatistics SequenceAnalyzer::getStatistics() const {
        SequenceStatistics stats;

        stats.totalLength = sequence_.length();

        if (sequence_.empty()) {
            return stats;
        }

        // Count nucleotides
        for (char c : sequence_) {
            switch (c) {
            case 'A': stats.adenineCount++; break;
            case 'T': stats.thymineCount++; break;
            case 'G': stats.guanineCount++; break;
            case 'C': stats.cytosineCount++; break;
            case 'U': stats.uracilCount++; break;
            }
        }

        // Calculate percentages
        if (stats.totalLength > 0) {
            stats.adeninePercent = (stats.adenineCount * 100.0) / stats.totalLength;
            stats.thyminePercent = (stats.thymineCount * 100.0) / stats.totalLength;
            stats.guaninePercent = (stats.guanineCount * 100.0) / stats.totalLength;
            stats.cytosinePercent = (stats.cytosineCount * 100.0) / stats.totalLength;
            stats.uracilPercent = (stats.uracilCount * 100.0) / stats.totalLength;
        }

        // GC content
        stats.gcContent = calculateGCContent();

        // Determine sequence type
        if (stats.uracilCount > 0 && stats.thymineCount == 0) {
            stats.sequenceType = "RNA";
        }
        else if (stats.thymineCount > 0 && stats.uracilCount == 0) {
            stats.sequenceType = "DNA";
        }
        else if (stats.uracilCount > 0 && stats.thymineCount > 0) {
            stats.sequenceType = "Mixed (DNA/RNA)";
        }
        else {
            stats.sequenceType = "Unknown";
        }

        // Find motifs using Aho-Corasick
        auto tempAC = std::make_unique<AhoCorasick>();
        auto allMotifs = MotifDatabase::getAllMotifs();

        for (const auto& motif : allMotifs) {
            tempAC->addPattern(motif.pattern, motif.name);
        }

        tempAC->buildAutomaton();
        auto motifResults = tempAC->search(sequence_);

        // Group by motif type
        std::unordered_map<std::string, MotifOccurrence> motifMap;

        for (const auto& result : motifResults) {
            auto& occurrence = motifMap[result.motifType];
            if (occurrence.motifName.empty()) {
                occurrence.motifName = result.motifType;
                occurrence.motifPattern = result.matchedSequence;
            }
            occurrence.positions.push_back(result.position);
            occurrence.count++;
        }

        // Convert to vector
        for (const auto& pair : motifMap) {
            stats.motifs.push_back(pair.second);
        }

        return stats;
    }

} // namespace DNACore