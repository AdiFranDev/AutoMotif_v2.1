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
     * . NET wrapper for DNACore:: SequenceAnalyzer
     * Provides managed interface to native C++ functionality
     */
    public ref class ManagedSequenceAnalyzer {
    public:
        ManagedSequenceAnalyzer();
        ~ManagedSequenceAnalyzer();
        !ManagedSequenceAnalyzer();  // Finalizer

        // ===== SEQUENCE MANAGEMENT =====
        void SetSequence(String^ sequence);
        String^ GetSequence();

        // ===== SEARCH METHODS =====
        List<ManagedMatchResult^>^ ExactMatch(String^ pattern);
        List<ManagedMatchResult^>^ ApproximateMatch(String^ pattern, int maxDistance);
        List<ManagedMatchResult^>^ SearchMotif(int motifType);
        List<ManagedMatchResult^>^ SearchAllMotifs();
        List<ManagedMatchResult^>^ PDASearch(String^ pattern);

        // ===== REGEX SEARCH (NEW) =====
        List<ManagedMatchResult^>^ RegexSearch(String^ pattern);
        String^ GetNFADescription();
        String^ GetRegexTrace();
        String^ GetRegexError();

        // ===== STATISTICS =====
        ManagedSequenceStatistics^ GetStatistics();
        double CalculateGCContent();

        // ===== TRACING =====
        String^ GetDFATrace();
        String^ GetPDATrace();
        void ClearPDALogger();
        void AttachPDAObserver();

        // ===== VALIDATION =====
        ManagedValidationResult^ ValidateSequence(String^ sequence);
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