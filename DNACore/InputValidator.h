#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

namespace DNACore {

    /**
     * Validation result structure
     */
    struct ValidationResult {
        bool isValid;
        std::string cleanedSequence;
        std::vector<std::string> warnings;
        std::vector<std::string> errors;

        ValidationResult() : isValid(true) {}
    };

    /**
     * Input validator for DNA/RNA sequences
     */
    class InputValidator {
    public:
        /**
         * Configuration options
         */
        struct Config {
            bool allowAmbiguous;        // Allow IUPAC ambiguity codes (N, R, Y, etc.)
            bool autoUppercase;         // Convert to uppercase
            bool allowWhitespace;       // Allow spaces/newlines (auto-remove)
            size_t minLength;           // Minimum sequence length
            size_t maxLength;           // Maximum sequence length (0 = no limit)

            Config()
                : allowAmbiguous(false),
                autoUppercase(true),
                allowWhitespace(true),
                minLength(1),
                maxLength(100000000) {
            }  // 100 Mbp default max
        };

        explicit InputValidator(const Config& config = Config());
        ~InputValidator();

        /**
         * Validate a DNA/RNA sequence
         * @param sequence The sequence to validate
         * @return ValidationResult with cleaned sequence and any errors/warnings
         */
        ValidationResult validate(const std::string& sequence) const;

        /**
         * Validate a search pattern (less strict than sequence validation)
         * @param pattern The pattern to validate
         * @return ValidationResult
         */
        ValidationResult validatePattern(const std::string& pattern) const;

        /**
         * Get current configuration
         */
        const Config& getConfig() const { return config_; }

        /**
         * Update configuration
         */
        void setConfig(const Config& config) { config_ = config; }

    private:
        Config config_;

        // Standard nucleotides
        static const std::string STANDARD_NUCLEOTIDES;
        // IUPAC ambiguity codes
        static const std::string AMBIGUITY_CODES;

        /**
         * Clean sequence (remove whitespace, convert case)
         */
        std::string cleanSequence(const std::string& seq, ValidationResult& result) const;

        /**
         * Validate characters in sequence
         */
        void validateCharacters(const std::string& seq, ValidationResult& result) const;
    };

} // namespace DNACore