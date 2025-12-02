#pragma once

#include "InputValidator.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace DNACore {

    /**
     * FASTA record structure
     */
    struct FASTARecord {
        std::string header;       // Everything after '>'
        std::string sequence;     // Concatenated sequence lines
        std::string description;  // First part of header (ID)

        size_t length() const { return sequence.length(); }
        bool isEmpty() const { return sequence.empty(); }
    };

    /**
     * Parser result
     */
    struct FASTAParseResult {
        std::vector<FASTARecord> records;
        std::vector<std::string> warnings;
        std::vector<std::string> errors;
        bool success;

        FASTAParseResult() : success(true) {}

        size_t totalRecords() const { return records.size(); }
        size_t totalBases() const {
            size_t total = 0;
            for (const auto& rec : records) {
                total += rec.length();
            }
            return total;
        }
    };

    /**
     * FASTA file parser
     */
    class FASTAParser {
    public:
        struct Config {
            bool validateSequences;      // Run InputValidator on each sequence
            bool allowMultipleRecords;   // Allow multi-FASTA files
            bool skipInvalidRecords;     // Continue parsing if one record fails
            size_t maxFileSize;          // Maximum file size in bytes (0 = unlimited)
            InputValidator::Config validatorConfig;

            Config()
                : validateSequences(true),
                allowMultipleRecords(true),
                skipInvalidRecords(false),
                maxFileSize(100 * 1024 * 1024) {
            }  // 100 MB default
        };

        explicit FASTAParser(const Config& config = Config());
        ~FASTAParser();

        /**
         * Parse from file path
         * @param filepath Path to FASTA file
         * @return Parse result with records or errors
         */
        FASTAParseResult parseFile(const std::string& filepath);

        /**
         * Parse from string content
         * @param content FASTA formatted string
         * @return Parse result
         */
        FASTAParseResult parseString(const std::string& content);

        /**
         * Get current configuration
         */
        const Config& getConfig() const { return config_; }

    private:
        Config config_;
        InputValidator validator_;

        /**
         * Parse FASTA content
         */
        FASTAParseResult parseFASTAContent(const std::string& content);

        /**
         * Parse plain text (no FASTA header)
         */
        FASTAParseResult parsePlainText(const std::string& content);

        /**
         * Process a single record
         */
        bool processRecord(FASTARecord& record, FASTAParseResult& result);

        /**
         * Extract description from header
         */
        std::string extractDescription(const std::string& header) const;

        /**
         * Trim whitespace
         */
        std::string trim(const std::string& str) const;
    };

} // namespace DNACore