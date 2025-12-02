#include "FASTAParser.h"

namespace DNACore {

    FASTAParser::FASTAParser(const Config& config)
        : config_(config), validator_(config.validatorConfig) {
    }

    FASTAParser::~FASTAParser() {
    }

    FASTAParseResult FASTAParser::parseFile(const std::string& filepath) {
        FASTAParseResult result;

        // Open file
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            result.success = false;
            result.errors.push_back("Failed to open file: " + filepath);
            return result;
        }

        // Check file size
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        if (config_.maxFileSize > 0 && fileSize > config_.maxFileSize) {
            result.success = false;
            result.errors.push_back(
                "File too large: " + std::to_string(fileSize / (1024 * 1024)) +
                " MB (max: " + std::to_string(config_.maxFileSize / (1024 * 1024)) + " MB)"
            );
            return result;
        }

        // Read entire file
        std::string content((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
        file.close();

        // Detect format
        if (content.empty()) {
            result.success = false;
            result.errors.push_back("File is empty");
            return result;
        }

        // Check if FASTA format
        if (content[0] != '>') {
            // Assume plain text sequence
            return parsePlainText(content);
        }

        // Parse FASTA
        return parseFASTAContent(content);
    }

    FASTAParseResult FASTAParser::parseString(const std::string& content) {
        if (content.empty()) {
            FASTAParseResult result;
            result.success = false;
            result.errors.push_back("Input is empty");
            return result;
        }

        if (content[0] == '>') {
            return parseFASTAContent(content);
        }
        else {
            return parsePlainText(content);
        }
    }

    FASTAParseResult FASTAParser::parseFASTAContent(const std::string& content) {
        FASTAParseResult result;

        std::istringstream stream(content);
        std::string line;
        FASTARecord currentRecord;
        bool inRecord = false;

        while (std::getline(stream, line)) {
            // Trim whitespace
            line = trim(line);

            if (line.empty()) continue;

            if (line[0] == '>') {
                // New header
                if (inRecord) {
                    // Save previous record
                    if (!processRecord(currentRecord, result)) {
                        if (!config_.skipInvalidRecords) {
                            result.success = false;
                            return result;
                        }
                    }
                }

                // Start new record
                currentRecord = FASTARecord();
                currentRecord.header = line.substr(1);  // Remove '>'
                currentRecord.description = extractDescription(currentRecord.header);
                inRecord = true;

                // Check if multiple records allowed
                if (!config_.allowMultipleRecords && !result.records.empty()) {
                    result.warnings.push_back(
                        "Multiple records detected, only using first one"
                    );
                    break;
                }
            }
            else {
                // Sequence line
                if (!inRecord) {
                    result.errors.push_back("Sequence data before header");
                    result.success = false;
                    return result;
                }
                currentRecord.sequence += line;
            }
        }

        // Process last record
        if (inRecord) {
            if (!processRecord(currentRecord, result)) {
                if (!config_.skipInvalidRecords) {
                    result.success = false;
                    return result;
                }
            }
        }

        if (result.records.empty()) {
            result.success = false;
            result.errors.push_back("No valid sequences found");
        }

        return result;
    }

    FASTAParseResult FASTAParser::parsePlainText(const std::string& content) {
        FASTAParseResult result;

        FASTARecord record;
        record.header = "Unnamed Sequence";
        record.description = "Plain Text Input";
        record.sequence = content;

        // Remove all whitespace
        record.sequence.erase(
            std::remove_if(record.sequence.begin(), record.sequence.end(),
                [](char c) { return std::isspace(static_cast<unsigned char>(c)); }),
            record.sequence.end()
        );

        if (!processRecord(record, result)) {
            result.success = false;
        }

        return result;
    }

    bool FASTAParser::processRecord(FASTARecord& record, FASTAParseResult& result) {
        // Validate if enabled
        if (config_.validateSequences) {
            ValidationResult validation = validator_.validate(record.sequence);

            if (!validation.isValid) {
                result.errors.push_back(
                    "Invalid sequence in record '" + record.description + "'"
                );
                for (const auto& err : validation.errors) {
                    result.errors.push_back("  " + err);
                }
                return false;
            }

            // Use cleaned sequence
            record.sequence = validation.cleanedSequence;

            // Propagate warnings
            for (const auto& warn : validation.warnings) {
                result.warnings.push_back(
                    record.description + ": " + warn
                );
            }
        }

        result.records.push_back(record);
        return true;
    }

    std::string FASTAParser::extractDescription(const std::string& header) const {
        // Extract first word (usually the ID)
        size_t pos = header.find_first_of(" \t");
        if (pos == std::string::npos) {
            return header;
        }
        return header.substr(0, pos);
    }

    std::string FASTAParser::trim(const std::string& str) const {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";

        size_t end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }

} // namespace DNACore