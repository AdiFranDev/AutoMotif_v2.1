#pragma once

#include <string>
#include <vector>
#include <utility>

namespace DNACore {

    /**
     * Database of known DNA/RNA regulatory motifs
     */
    class MotifDatabase {
    public:
        enum MotifType {
            TATA_BOX = 1,
            CAAT_BOX = 2,
            GC_BOX = 3,
            KOZAK_SEQUENCE = 4,
            POLYA_SIGNAL = 5
        };

        struct MotifEntry {
            MotifType type;
            std::string name;
            std::string pattern;
            std::string description;

            MotifEntry(MotifType t, const std::string& n, const std::string& p, const std::string& d)
                : type(t), name(n), pattern(p), description(d) {
            }
        };

        /**
         * Get all motif entries
         */
        static std::vector<MotifEntry> getAllMotifs() {
            return {
                MotifEntry(
                    TATA_BOX,
                    "TATA Box",
                    "TATAAA",
                    "Promoter element ~25-30 bp upstream of transcription start"
                ),
                MotifEntry(
                    CAAT_BOX,
                    "CAAT Box",
                    "GGCCAATCT",
                    "Promoter element ~75-80 bp upstream of transcription start"
                ),
                MotifEntry(
                    GC_BOX,
                    "GC Box",
                    "GGGCGG",
                    "Promoter element binding site for Sp1 transcription factor"
                ),
                MotifEntry(
                    KOZAK_SEQUENCE,
                    "Kozak Sequence",
                    "GCCACCATGG",
                    "Translation initiation site (ribosome binding)"
                ),
                MotifEntry(
                    POLYA_SIGNAL,
                    "Poly-A Signal",
                    "AATAAA",
                    "Polyadenylation signal for mRNA processing"
                )
            };
        }

        /**
         * Get motif by type
         */
        static MotifEntry getMotif(MotifType type) {
            auto motifs = getAllMotifs();
            for (const auto& motif : motifs) {
                if (motif.type == type) {
                    return motif;
                }
            }
            // Default fallback
            return MotifEntry(TATA_BOX, "Unknown", "", "");
        }

        /**
         * Get motif pattern by type ID (1-5)
         */
        static std::string getMotifPattern(int typeId, std::string& outName) {
            if (typeId < 1 || typeId > 5) {
                outName = "Unknown";
                return "";
            }

            auto motif = getMotif(static_cast<MotifType>(typeId));
            outName = motif.name;
            return motif.pattern;
        }
    };

} // namespace DNACore