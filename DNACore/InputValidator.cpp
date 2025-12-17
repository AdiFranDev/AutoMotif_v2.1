#include "InputValidator.h"

namespace DNACore {

    // Static member definitions
    const std::string InputValidator::VALID_NUCLEOTIDES = "ACGTU";
    const std::string InputValidator::LOWERCASE_NUCLEOTIDES = "acgtu";
    const std::string InputValidator::AMBIGUITY_CODES = "RYSWKMBDHVN";

    InputValidator::InputValidator(const Config& config)
        : config_(config) {
    }

    InputValidator::~InputValidator() {
    }

    ValidationResult InputValidator::validate(const std::string& sequence) const {
        ValidationResult result;

        // Check empty
        if (sequence.empty()) {
            result.isValid = false;
            result.errors.push_back("Sequence is empty");
            return result;
        }

        // Check for lowercase letters BEFORE cleaning (if autoUppercase is disabled)
        if (!config_.autoUppercase) {
            for (char c : sequence) {
                if (std::isalpha(static_cast<unsigned char>(c)) && std::islower(static_cast<unsigned char>(c))) {
                    result.isValid = false;
                    result.errors.push_back("Lowercase letters are not allowed. Please use uppercase nucleotides only (A, T, G, C, U, N)");
                    return result;
                }
            }
        }

        // Clean and validate
        result.cleanedSequence = cleanSequence(sequence, result);

        // Check length constraints
        if (result.cleanedSequence.length() < config_.minLength) {
            result.isValid = false;
            result.errors.push_back(
                "Sequence too short: " + std::to_string(result.cleanedSequence.length()) +
                " bp (minimum: " + std::to_string(config_.minLength) + ")"
            );
        }

        if (config_.maxLength > 0 && result.cleanedSequence.length() > config_.maxLength) {
            result.isValid = false;
            result.errors.push_back(
                "Sequence too long: " + std::to_string(result.cleanedSequence.length()) +
                " bp (maximum: " + std::to_string(config_.maxLength) + ")"
            );
        }

        // Validate characters
        validateCharacters(result.cleanedSequence, result);

        return result;
    }

    ValidationResult InputValidator::validatePattern(const std::string& pattern) const {
        ValidationResult result;

        if (pattern.empty()) {
            result.isValid = false;
            result.errors.push_back("Pattern is empty");
            return result;
        }

        if (pattern.length() < 2) {
            result.isValid = false;
            result.errors.push_back("Pattern too short (minimum 2 characters)");
            return result;
        }

        // Check for lowercase letters BEFORE cleaning (if autoUppercase is disabled)
        if (!config_.autoUppercase) {
            for (char c : pattern) {
                if (std::isalpha(static_cast<unsigned char>(c)) && std::islower(static_cast<unsigned char>(c))) {
                    result.isValid = false;
                    result.errors.push_back("Lowercase letters are not allowed. Please use uppercase nucleotides only (A, T, G, C, U, N)");
                    return result;
                }
            }
        }

        result.cleanedSequence = cleanSequence(pattern, result);
        validateCharacters(result.cleanedSequence, result);

        return result;
    }

    std::string InputValidator::cleanSequence(const std::string& seq, ValidationResult& result) const {
        std::string cleaned;
        cleaned.reserve(seq.length());

        for (char c : seq) {
            // Skip whitespace if allowed
            if (config_.allowWhitespace && std::isspace(static_cast<unsigned char>(c))) {
                continue;
            }

            // DO NOT convert to uppercase - keep original case so validation can catch it
            // Only uppercase if explicitly enabled AND we want auto-conversion
            char ch = c;  // Keep original character
            if (config_.autoUppercase) {
                ch = std::toupper(static_cast<unsigned char>(c));
            }

            cleaned += ch;
        }

        return cleaned;
    }

    void InputValidator::validateCharacters(const std::string& seq, ValidationResult& result) const {
        size_t invalidCount = 0;
        size_t ambiguousCount = 0;
        std::string invalidChars;

        // HARDCODED: Explicitly check for lowercase and reject it
        for (size_t i = 0; i < seq.length(); ++i) {
            char c = seq[i];

            // Reject any lowercase letter immediately
            if (c >= 'a' && c <= 'z') {
                result.isValid = false;
                result.errors.push_back("Lowercase letters are not allowed. Use uppercase only (A, T, G, C, U, N)");
                return;
            }
        }

        // Now validate the actual content
        for (size_t i = 0; i < seq.length(); ++i) {
            char c = seq[i];

            // Check if standard nucleotide
            if (VALID_NUCLEOTIDES.find(c) != std::string::npos) {
                continue;
            }

            // Check if ambiguous code
            if (AMBIGUITY_CODES.find(c) != std::string::npos) {
                ambiguousCount++;
                if (!config_.allowAmbiguous) {
                    result.isValid = false;
                    if (invalidChars.find(c) == std::string::npos) {
                        invalidChars += c;
                    }
                }
                continue;
            }

            // Invalid character
            invalidCount++;
            result.isValid = false;
            if (invalidChars.find(c) == std::string::npos) {
                invalidChars += c;
            }
        }

        // Report errors
        if (invalidCount > 0) {
            result.errors.push_back(
                "Invalid characters found: '" + invalidChars + "' (" +
                std::to_string(invalidCount) + " occurrences)"
            );
        }

        // Report warnings
        if (ambiguousCount > 0) {
            if (config_.allowAmbiguous) {
                result.warnings.push_back(
                    "Ambiguous bases (N, R, Y, etc.): " + std::to_string(ambiguousCount) +
                    " (" + std::to_string(ambiguousCount * 100.0 / seq.length()) + "%)"
                );
            }
        }

        // Warn if very long
        if (seq.length() > 1000000) {
            result.warnings.push_back(
                "Large sequence (" + std::to_string(seq.length()) +
                " bp) - some algorithms may be slow"
            );
        }
    }

} // namespace DNACore