#define _WINSOCKAPI_
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "ManagedSequenceAnalyzer.h"

namespace DNACoreBridge {

    ManagedSequenceAnalyzer::ManagedSequenceAnalyzer() {
        nativeAnalyzer_ = new DNACore::SequenceAnalyzer();

        DNACore::InputValidator::Config validatorConfig;
        validatorConfig.autoUppercase = false;  // Reject lowercase instead of converting
        validatorConfig.allowAmbiguous = false;
        validatorConfig.allowWhitespace = true;
        validatorConfig.minLength = 1;
        validatorConfig.maxLength = 100000000;
        
        nativeValidator_ = new DNACore::InputValidator(validatorConfig);

        pdaLogger_ = nativeAnalyzer_->getPDALogger();
    }

    ManagedSequenceAnalyzer::~ManagedSequenceAnalyzer() {
        this->!ManagedSequenceAnalyzer();
    }

    ManagedSequenceAnalyzer::!ManagedSequenceAnalyzer() {
        if (nativeAnalyzer_) {
            delete nativeAnalyzer_;
            nativeAnalyzer_ = nullptr;
        }
        if (nativeValidator_) {
            delete nativeValidator_;
            nativeValidator_ = nullptr;
        }
    }

    // ===== SEQUENCE MANAGEMENT =====

    void ManagedSequenceAnalyzer::SetSequence(String^ sequence) {
        std::string nativeSeq = TypeConverters::ToStdString(sequence);
        nativeAnalyzer_->setSequence(nativeSeq);
    }

    String^ ManagedSequenceAnalyzer::GetSequence() {
        return TypeConverters::ToManagedString(nativeAnalyzer_->getSequence());
    }

    // ===== SEARCH METHODS =====

    List<ManagedMatchResult^>^ ManagedSequenceAnalyzer::ExactMatch(String^ pattern) {
        std::string nativePattern = TypeConverters::ToStdString(pattern);
        auto nativeResults = nativeAnalyzer_->exactMatch(nativePattern);
        return ConvertResults(nativeResults);
    }

    List<ManagedMatchResult^>^ ManagedSequenceAnalyzer::ApproximateMatch(String^ pattern, int maxDistance) {
        std::string nativePattern = TypeConverters::ToStdString(pattern);
        auto nativeResults = nativeAnalyzer_->approximateMatch(nativePattern, maxDistance);
        return ConvertResults(nativeResults);
    }

    List<ManagedMatchResult^>^ ManagedSequenceAnalyzer::SearchMotif(int motifType) {
        auto nativeResults = nativeAnalyzer_->searchMotif(motifType);
        return ConvertResults(nativeResults);
    }

    List<ManagedMatchResult^>^ ManagedSequenceAnalyzer::SearchAllMotifs() {
        auto nativeResults = nativeAnalyzer_->searchAllMotifs();
        return ConvertResults(nativeResults);
    }

    List<ManagedMatchResult^>^ ManagedSequenceAnalyzer::PDASearch(String^ pattern) {
        std::string nativePattern = TypeConverters::ToStdString(pattern);
        auto nativeResults = nativeAnalyzer_->pushdownSearch(nativePattern);
        return ConvertResults(nativeResults);
    }

    // ===== STATISTICS =====

    ManagedSequenceStatistics^ ManagedSequenceAnalyzer::GetStatistics() {
        auto nativeStats = nativeAnalyzer_->getStatistics();
        return ConvertStatistics(nativeStats);
    }

    double ManagedSequenceAnalyzer::CalculateGCContent() {
        return nativeAnalyzer_->calculateGCContent();
    }

    // ===== TRACING =====

    String^ ManagedSequenceAnalyzer::GetDFATrace() {
        auto tracer = nativeAnalyzer_->getDFATracer();
        if (tracer) {
            return TypeConverters::ToManagedString(tracer->getTraceSummary());
        }
        return "";
    }

    String^ ManagedSequenceAnalyzer::GetPDATrace() {
        if (pdaLogger_) {
            return TypeConverters::ToManagedString(pdaLogger_->formatLogFormatted());
        }
        return "";
    }

    void ManagedSequenceAnalyzer::ClearPDALogger() {
        if (pdaLogger_) {
            pdaLogger_->clear();
        }
    }

    void ManagedSequenceAnalyzer::AttachPDAObserver() {
        auto pda = nativeAnalyzer_->getPDA();
        if (pda && pdaLogger_) {
            pda->clearObservers();
            pda->attachObserver(pdaLogger_);
        }
    }

    // ===== VALIDATION =====

    ManagedValidationResult^ ManagedSequenceAnalyzer::ValidateSequence(String^ sequence) {
        std::string nativeSeq = TypeConverters::ToStdString(sequence);
        auto nativeResult = nativeValidator_->validate(nativeSeq);
        return ConvertValidation(nativeResult);
    }

    ManagedValidationResult^ ManagedSequenceAnalyzer::ValidatePattern(String^ pattern) {
        std::string nativePat = TypeConverters::ToStdString(pattern);
        auto nativeResult = nativeValidator_->validatePattern(nativePat);
        return ConvertValidation(nativeResult);
    }

    // ===== HELPER METHODS =====

    List<ManagedMatchResult^>^ ManagedSequenceAnalyzer::ConvertResults(
        const std::vector<DNACore::MatchResult>& nativeResults) {

        auto managedResults = gcnew List<ManagedMatchResult^>();

        for (const auto& nativeResult : nativeResults) {
            auto managedResult = gcnew ManagedMatchResult(
                static_cast<int>(nativeResult.position),
                TypeConverters::ToManagedString(nativeResult.matchedSequence),
                nativeResult.editDistance,
                TypeConverters::ToManagedString(nativeResult.motifType),
                TypeConverters::ToManagedString(nativeResult.algorithm)
            );
            managedResults->Add(managedResult);
        }

        return managedResults;
    }

    ManagedSequenceStatistics^ ManagedSequenceAnalyzer::ConvertStatistics(
        const DNACore::SequenceStatistics& nativeStats) {

        auto managedStats = gcnew ManagedSequenceStatistics();

        managedStats->AdenineCount = static_cast<int>(nativeStats.adenineCount);
        managedStats->ThymineCount = static_cast<int>(nativeStats.thymineCount);
        managedStats->GuanineCount = static_cast<int>(nativeStats.guanineCount);
        managedStats->CytosineCount = static_cast<int>(nativeStats.cytosineCount);
        managedStats->UracilCount = static_cast<int>(nativeStats.uracilCount);

        managedStats->AdeninePercent = nativeStats.adeninePercent;
        managedStats->ThyminePercent = nativeStats.thyminePercent;
        managedStats->GuaninePercent = nativeStats.guaninePercent;
        managedStats->CytosinePercent = nativeStats.cytosinePercent;
        managedStats->UracilPercent = nativeStats.uracilPercent;

        managedStats->TotalLength = static_cast<int>(nativeStats.totalLength);
        managedStats->GCContent = nativeStats.gcContent;
        managedStats->SequenceType = TypeConverters::ToManagedString(nativeStats.sequenceType);

        // Convert motifs
        for (const auto& nativeMotif : nativeStats.motifs) {
            auto managedMotif = gcnew ManagedMotifOccurrence();
            managedMotif->MotifName = TypeConverters::ToManagedString(nativeMotif.motifName);
            managedMotif->MotifPattern = TypeConverters::ToManagedString(nativeMotif.motifPattern);
            managedMotif->Count = nativeMotif.count;

            for (auto pos : nativeMotif.positions) {
                managedMotif->Positions->Add(static_cast<int>(pos));
            }

            managedStats->Motifs->Add(managedMotif);
        }

        return managedStats;
    }

    ManagedValidationResult^ ManagedSequenceAnalyzer::ConvertValidation(
        const DNACore::ValidationResult& nativeResult) {

        auto managedResult = gcnew ManagedValidationResult();
        managedResult->IsValid = nativeResult.isValid;
        managedResult->CleanedSequence = TypeConverters::ToManagedString(nativeResult.cleanedSequence);
        managedResult->Warnings = TypeConverters::ToManagedList(nativeResult.warnings);
        managedResult->Errors = TypeConverters::ToManagedList(nativeResult.errors);

        return managedResult;
    }

} // namespace DNACoreBridge