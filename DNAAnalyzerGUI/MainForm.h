#pragma once

// Prevent Windows SDK conflicts
#define _WINSOCKAPI_
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

// Forward declare namespace - no #include needed for managed types
namespace DNACoreBridge {
    ref class ManagedSequenceAnalyzer;
    ref class ManagedMatchResult;
    ref class ManagedSequenceStatistics;
    ref class ManagedValidationResult;
}

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace DNAAnalyzerGUI {

    public ref class MainForm : public System::Windows::Forms::Form {
    public:
        MainForm(void) {
            InitializeComponent();
            InitializeAnalyzer();
        }

    protected:
        ~MainForm() {
            if (components) {
                delete components;
            }
            if (analyzer_) {
                delete analyzer_;
            }
        }

    private:
        // ===== COMPONENTS =====
        System::ComponentModel::Container^ components;

        // Top panel
        System::Windows::Forms::Panel^ pnlTop;
        System::Windows::Forms::Label^ lblTitle;
        System::Windows::Forms::Label^ lblSequenceInput;
        System::Windows::Forms::TextBox^ txtSequence;
        System::Windows::Forms::Button^ btnLoadFile;
        System::Windows::Forms::Button^ btnClear;
        System::Windows::Forms::Button^ btnValidate;

        // Pattern panel
        System::Windows::Forms::GroupBox^ grpPattern;
        System::Windows::Forms::Label^ lblPattern;
        System::Windows::Forms::TextBox^ txtPattern;
        System::Windows::Forms::Label^ lblMaxDistance;
        System::Windows::Forms::NumericUpDown^ numMaxDistance;
        System::Windows::Forms::ComboBox^ cmbMotifType;
        System::Windows::Forms::Label^ lblMotifType;

        // Analysis buttons
        System::Windows::Forms::GroupBox^ grpAnalysis;
        System::Windows::Forms::Button^ btnExactMatch;
        System::Windows::Forms::Button^ btnApproxMatch;
        System::Windows::Forms::Button^ btnMotifSearch;
        System::Windows::Forms::Button^ btnAllMotifs;
        System::Windows::Forms::Button^ btnStatistics;
        System::Windows::Forms::Button^ btnPDADetail;

        // Split container for results and trace
        System::Windows::Forms::SplitContainer^ splitMain;

        // Results panel (left)
        System::Windows::Forms::Panel^ pnlResults;
        System::Windows::Forms::Label^ lblResults;
        System::Windows::Forms::DataGridView^ dgvResults;
        System::Windows::Forms::DataGridViewTextBoxColumn^ colPosition;
        System::Windows::Forms::DataGridViewTextBoxColumn^ colSequence;
        System::Windows::Forms::DataGridViewTextBoxColumn^ colDistance;
        System::Windows::Forms::DataGridViewTextBoxColumn^ colType;
        System::Windows::Forms::DataGridViewTextBoxColumn^ colAlgorithm;

        // Trace panel (right)
        System::Windows::Forms::Panel^ pnlTrace;
        System::Windows::Forms::Label^ lblTrace;
        System::Windows::Forms::RichTextBox^ txtTrace;

        // Status bar
        System::Windows::Forms::StatusStrip^ statusStrip;
        System::Windows::Forms::ToolStripStatusLabel^ statusLabel;

        // ===== BACKEND =====
        DNACoreBridge::ManagedSequenceAnalyzer^ analyzer_;

        // ===== METHODS =====
        void InitializeComponent(void);
        void InitializeAnalyzer(void);

        // Event handlers
        void btnLoadFile_Click(System::Object^ sender, System::EventArgs^ e);
        void btnClear_Click(System::Object^ sender, System::EventArgs^ e);
        void btnValidate_Click(System::Object^ sender, System::EventArgs^ e);
        void btnExactMatch_Click(System::Object^ sender, System::EventArgs^ e);
        void btnApproxMatch_Click(System::Object^ sender, System::EventArgs^ e);
        void btnMotifSearch_Click(System::Object^ sender, System::EventArgs^ e);
        void btnAllMotifs_Click(System::Object^ sender, System::EventArgs^ e);
        void btnStatistics_Click(System::Object^ sender, System::EventArgs^ e);
        void btnPDADetail_Click(System::Object^ sender, System::EventArgs^ e);

        // Helper methods
        void DisplayResults(List<DNACoreBridge::ManagedMatchResult^>^ results, String^ algorithm);
        void DisplayStatistics(DNACoreBridge::ManagedSequenceStatistics^ stats);
        void DisplayTrace(String^ trace);
        void ClearResults();
        void UpdateStatus(String^ message);
        bool ValidateInputs();
    };
}