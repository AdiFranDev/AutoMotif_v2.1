#pragma once

#include <string>
#include <vector>

namespace DNACore {

    // ===== MATCH RESULT =====
    struct MatchResult {
        size_t position;              // 0-based position in sequence
        std::string matchedSequence;  // The actual matched substring
        int editDistance;             // 0 for exact, >0 for approximate
        std::string motifType;        // "TATA Box", "Exact Match", etc.
        std::string algorithm;        // "KMP", "Aho-Corasick", "PDA", etc. 

        MatchResult()
            : position(0), editDistance(0), motifType(""), algorithm("") {
        }

        MatchResult(size_t pos, const std::string& seq, int dist = 0,
            const std::string& type = "", const std::string& algo = "")
            : position(pos), matchedSequence(seq), editDistance(dist),
            motifType(type), algorithm(algo) {
        }
    };

    // ===== MOTIF OCCURRENCE =====
    struct MotifOccurrence {
        std::string motifName;        // "TATA Box", "CAAT Box", etc.
        std::string motifPattern;     // "TATAAA", "GGCCAATCT", etc.
        int count;                    // Number of occurrences
        std::vector<size_t> positions; // All positions found

        MotifOccurrence()
            : motifName(""), motifPattern(""), count(0) {
        }
    };

    // ===== SEQUENCE STATISTICS =====
    struct SequenceStatistics {
        // Base counts
        size_t adenineCount;
        size_t thymineCount;
        size_t guanineCount;
        size_t cytosineCount;
        size_t uracilCount;

        // Percentages
        double adeninePercent;
        double thyminePercent;
        double guaninePercent;
        double cytosinePercent;
        double uracilPercent;

        // Summary
        size_t totalLength;
        double gcContent;
        std::string sequenceType;     // "DNA", "RNA", "Mixed"

        // Motifs found
        std::vector<MotifOccurrence> motifs;

        SequenceStatistics()
            : adenineCount(0), thymineCount(0), guanineCount(0),
            cytosineCount(0), uracilCount(0),
            adeninePercent(0.0), thyminePercent(0.0), guaninePercent(0.0),
            cytosinePercent(0.0), uracilPercent(0.0),
            totalLength(0), gcContent(0.0), sequenceType("Unknown") {
        }
    };

    // ===== ANALYSIS OPTIONS =====
    struct AnalysisOptions {
        int maxEditDistance;          // For approximate matching
        bool caseSensitive;           // Currently unused (always uppercase)
        bool findOverlapping;         // Allow overlapping matches
        size_t maxResults;            // 0 = unlimited

        AnalysisOptions()
            : maxEditDistance(2), caseSensitive(false),
            findOverlapping(true), maxResults(0) {
        }
    };

    // ===== TRACE MODE =====
    enum class TraceMode {
        NONE,              // No tracing
        DFA_SUMMARY,       // High-level summary (for KMP, Aho-Corasick)
        PDA_DETAILED       // Full stack trace (for PDA)
    };

} // namespace DNACore