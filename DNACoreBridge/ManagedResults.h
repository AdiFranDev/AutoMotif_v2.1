#define _WINSOCKAPI_
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace DNACoreBridge {

    /**
     * . NET-compatible match result
     */
    public ref class ManagedMatchResult {
    public:
        property int Position;
        property String^ MatchedSequence;
        property int EditDistance;
        property String^ MotifType;
        property String^ Algorithm;

        ManagedMatchResult() {
            Position = 0;
            EditDistance = 0;
            MatchedSequence = "";
            MotifType = "";
            Algorithm = "";
        }

        ManagedMatchResult(int pos, String^ seq, int dist, String^ type, String^ algo) {
            Position = pos;
            MatchedSequence = seq;
            EditDistance = dist;
            MotifType = type;
            Algorithm = algo;
        }
    };

    /**
     * .NET-compatible motif occurrence
     */
    public ref class ManagedMotifOccurrence {
    public:
        property String^ MotifName;
        property String^ MotifPattern;
        property int Count;
        property List<int>^ Positions;

        ManagedMotifOccurrence() {
            MotifName = "";
            MotifPattern = "";
            Count = 0;
            Positions = gcnew List<int>();
        }
    };

    /**
     * .NET-compatible sequence statistics
     */
    public ref class ManagedSequenceStatistics {
    public:
        // Base counts
        property int AdenineCount;
        property int ThymineCount;
        property int GuanineCount;
        property int CytosineCount;
        property int UracilCount;

        // Percentages
        property double AdeninePercent;
        property double ThyminePercent;
        property double GuaninePercent;
        property double CytosinePercent;
        property double UracilPercent;

        // Summary
        property int TotalLength;
        property double GCContent;
        property String^ SequenceType;

        // Motifs
        property List<ManagedMotifOccurrence^>^ Motifs;

        ManagedSequenceStatistics() {
            AdenineCount = 0;
            ThymineCount = 0;
            GuanineCount = 0;
            CytosineCount = 0;
            UracilCount = 0;
            AdeninePercent = 0.0;
            ThyminePercent = 0.0;
            GuaninePercent = 0.0;
            CytosinePercent = 0.0;
            UracilPercent = 0.0;
            TotalLength = 0;
            GCContent = 0.0;
            SequenceType = "Unknown";
            Motifs = gcnew List<ManagedMotifOccurrence^>();
        }
    };

    /**
     * .NET-compatible validation result
     */
    public ref class ManagedValidationResult {
    public:
        property bool IsValid;
        property String^ CleanedSequence;
        property List<String^>^ Warnings;
        property List<String^>^ Errors;

        ManagedValidationResult() {
            IsValid = true;
            CleanedSequence = "";
            Warnings = gcnew List<String^>();
            Errors = gcnew List<String^>();
        }
    };

} // namespace DNACoreBridge