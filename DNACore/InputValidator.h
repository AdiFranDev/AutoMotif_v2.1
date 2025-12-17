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
     * STRICT MODE: Only accepts uppercase A, C, G, T, U
     */
    class InputValidator {
    public:
        /**
         * Configuration options
         */
        struct Config {
            bool strictMode;            // NEW: Only accept uppercase ACGTU (reject lowercase)
            bool allowAmbiguous;        // Allow IUPAC ambiguity codes (N, R, Y, etc.)
            bool autoUppercase;         // Convert to uppercase (DISABLED in strict mode)
            bool allowWhitespace;       // Allow spaces/newlines (auto-remove)
            size_t minLength;           // Minimum sequence length
            size_t maxLength;           // Maximum sequence length (0 = no limit)

            Config()
                : strictMode(true),          // NEW: Default to strict
                allowAmbiguous(false),
                autoUppercase(false),      // CHANGED: Default to false
                allowWhitespace(true),
                minLength(1),
                maxLength(100000000) {
            }
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

        // Standard nucleotides (UPPERCASE ONLY)
        static const std::string VALID_NUCLEOTIDES;
        // Lowercase versions (for error detection)
        static const std::string LOWERCASE_NUCLEOTIDES;
        // IUPAC ambiguity codes
        static const std::string AMBIGUITY_CODES;

        /**
         * Clean sequence (remove whitespace only - NO case conversion in strict mode)
         */
        std::string cleanSequence(const std::string& seq, ValidationResult& result) const;

        /**
         * Validate characters in sequence (STRICT:  reject lowercase)
         */
        void validateCharacters(const std::string& seq, ValidationResult& result) const;
    };

} // namespace DNACore