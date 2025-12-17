#define _WINSOCKAPI_
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#pragma once

#include "ManagedResults.h"
#include "TypeConverters.h"
#include "SequenceAnalyzer.h"
#include "InputValidator.h"
#include "FASTAParser.h"
#include <memory>

using namespace System;
using namespace System::Collections::Generic;

namespace DNACoreBridge {

    /**
     * .NET wrapper for DNACore::SequenceAnalyzer
     * Provides managed interface to native C++ functionality
     */
    public ref class ManagedSequenceAnalyzer {
    public:
        ManagedSequenceAnalyzer();
        ~ManagedSequenceAnalyzer();
        !ManagedSequenceAnalyzer();  // Finalizer

        // ===== SEQUENCE MANAGEMENT =====

        /**
         * Set the sequence to analyze
         */
        void SetSequence(String^ sequence);

        /**
         * Get current sequence
         */
        String^ GetSequence();

        // ===== SEARCH METHODS =====

        /**
         * Exact pattern matching using KMP
         */
        List<ManagedMatchResult^>^ ExactMatch(String^ pattern);

        /**
         * Approximate matching with edit distance
         */
        List<ManagedMatchResult^>^ ApproximateMatch(String^ pattern, int maxDistance);

        /**
         * Search for specific motif (1-6)
         */
        List<ManagedMatchResult^>^ SearchMotif(int motifType);

        /**
         * Search for all known motifs
         */
        List<ManagedMatchResult^>^ SearchAllMotifs();

        /**
         * PDA search with detailed tracing
         */
        List<ManagedMatchResult^>^ PDASearch(String^ pattern);

        // ===== STATISTICS =====

        /**
         * Get sequence statistics
         */
        ManagedSequenceStatistics^ GetStatistics();

        /**
         * Calculate GC content
         */
        double CalculateGCContent();

        // ===== TRACING =====

        /**
         * Get DFA summary trace (for KMP, Aho-Corasick)
         */
        String^ GetDFATrace();

        /**
         * Get detailed PDA trace
         */
        String^ GetPDATrace();

        /**
         * Clear PDA logger
         */
        void ClearPDALogger();

        /**
         * Attach PDA observer for next search
         */
        void AttachPDAObserver();

        // ===== VALIDATION =====

        /**
         * Validate a sequence
         */
        ManagedValidationResult^ ValidateSequence(String^ sequence);

        /**
         * Validate a pattern
         */
        ManagedValidationResult^ ValidatePattern(String^ pattern);

    private:
        DNACore::SequenceAnalyzer* nativeAnalyzer_;
        DNACore::InputValidator* nativeValidator_;
        DNACore::PDALogger* pdaLogger_;

        // Helper methods
        List<ManagedMatchResult^>^ ConvertResults(const std::vector<DNACore::MatchResult>& nativeResults);
        ManagedSequenceStatistics^ ConvertStatistics(const DNACore::SequenceStatistics& nativeStats);
        ManagedValidationResult^ ConvertValidation(const DNACore::ValidationResult& nativeResult);
    };

} // namespace DNACoreBridge