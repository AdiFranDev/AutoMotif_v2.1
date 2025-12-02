// Prevent Windows SDK conflicts
#define _WINSOCKAPI_
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "MainForm.h"

using namespace DNAAnalyzerGUI;
using namespace DNACoreBridge;

// Rest of the file stays the same...

using namespace DNAAnalyzerGUI;

void MainForm::InitializeComponent(void) {
    this->components = gcnew System::ComponentModel::Container();
    this->SuspendLayout();

    // ===== FORM SETUP =====
    this->Text = L"AutoMotif DNA and RNA Sequence Analyzer v2.1";
    this->Size = System::Drawing::Size(1400, 900);
    this->StartPosition = FormStartPosition::CenterScreen;
    this->MinimumSize = System::Drawing::Size(1200, 700);
    this->BackColor = Color::White;

    // ===== TOP PANEL =====
    this->pnlTop = gcnew Panel();
    this->pnlTop->Dock = DockStyle::Top;
    this->pnlTop->Height = 280;
    this->pnlTop->BackColor = Color::FromArgb(240, 240, 245);
    this->pnlTop->Padding = System::Windows::Forms::Padding(10);

    // Title
    this->lblTitle = gcnew Label();
    this->lblTitle->Text = L"AutoMotif The DNA and RNA Sequence Analyzer";
    this->lblTitle->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, FontStyle::Bold);
    this->lblTitle->Location = Point(15, 10);
    this->lblTitle->Size = System::Drawing::Size(500, 35);
    this->lblTitle->ForeColor = Color::FromArgb(0, 102, 204);

    // Sequence Label
    this->lblSequenceInput = gcnew Label();
    this->lblSequenceInput->Text = L"DNA and RNA Sequence:";
    this->lblSequenceInput->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
    this->lblSequenceInput->Location = Point(15, 55);
    this->lblSequenceInput->Size = System::Drawing::Size(150, 25);

    // Sequence TextBox
    this->txtSequence = gcnew TextBox();
    this->txtSequence->Location = Point(15, 80);
    this->txtSequence->Size = System::Drawing::Size(900, 80);
    this->txtSequence->Multiline = true;
    this->txtSequence->ScrollBars = ScrollBars::Vertical;
    this->txtSequence->Font = gcnew System::Drawing::Font(L"Consolas", 10);
    this->txtSequence->BackColor = Color::White;

    // Buttons
    this->btnLoadFile = gcnew Button();
    this->btnLoadFile->Text = L"Load FASTA";
    this->btnLoadFile->Location = Point(925, 80);
    this->btnLoadFile->Size = System::Drawing::Size(120, 35);
    this->btnLoadFile->Click += gcnew EventHandler(this, &MainForm::btnLoadFile_Click);

    this->btnClear = gcnew Button();
    this->btnClear->Text = L"Clear";
    this->btnClear->Location = Point(925, 120);
    this->btnClear->Size = System::Drawing::Size(120, 35);
    this->btnClear->Click += gcnew EventHandler(this, &MainForm::btnClear_Click);

    this->btnValidate = gcnew Button();
    this->btnValidate->Text = L"Validate";
    this->btnValidate->Location = Point(1055, 80);
    this->btnValidate->Size = System::Drawing::Size(120, 35);
    this->btnValidate->Click += gcnew EventHandler(this, &MainForm::btnValidate_Click);

    // ===== PATTERN GROUP BOX =====
    this->grpPattern = gcnew GroupBox();
    this->grpPattern->Text = L"Pattern Settings";
    this->grpPattern->Location = Point(15, 170);
    this->grpPattern->Size = System::Drawing::Size(600, 95);
    this->grpPattern->Font = gcnew System::Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);

    // Pattern Label
    this->lblPattern = gcnew Label();
    this->lblPattern->Text = L"Pattern:";
    this->lblPattern->Location = Point(10, 25);
    this->lblPattern->Size = System::Drawing::Size(60, 25);

    // Pattern TextBox
    this->txtPattern = gcnew TextBox();
    this->txtPattern->Location = Point(75, 23);
    this->txtPattern->Size = System::Drawing::Size(200, 25);
    this->txtPattern->Font = gcnew System::Drawing::Font(L"Consolas", 10);

    // Max Distance Label
    this->lblMaxDistance = gcnew Label();
    this->lblMaxDistance->Text = L"Max Distance:";
    this->lblMaxDistance->Location = Point(285, 25);
    this->lblMaxDistance->Size = System::Drawing::Size(90, 25);

    // Max Distance NumericUpDown
    this->numMaxDistance = gcnew NumericUpDown();
    this->numMaxDistance->Location = Point(380, 23);
    this->numMaxDistance->Size = System::Drawing::Size(60, 25);
    this->numMaxDistance->Minimum = 0;
    this->numMaxDistance->Maximum = 10;
    this->numMaxDistance->Value = 2;

    // Motif Type Label
    this->lblMotifType = gcnew Label();
    this->lblMotifType->Text = L"Motif Type:";
    this->lblMotifType->Location = Point(10, 58);
    this->lblMotifType->Size = System::Drawing::Size(70, 25);

    // Motif Type ComboBox
    this->cmbMotifType = gcnew ComboBox();
    this->cmbMotifType->Location = Point(75, 56);
    this->cmbMotifType->Size = System::Drawing::Size(200, 25);
    this->cmbMotifType->DropDownStyle = ComboBoxStyle::DropDownList;
    this->cmbMotifType->Items->Add(L"TATA Box");
    this->cmbMotifType->Items->Add(L"CAAT Box");
    this->cmbMotifType->Items->Add(L"GC Box");
    this->cmbMotifType->Items->Add(L"Kozak Sequence");
    this->cmbMotifType->Items->Add(L"Poly-A Signal");
    this->cmbMotifType->SelectedIndex = 0;

    // Add controls to pattern group
    this->grpPattern->Controls->Add(this->lblPattern);
    this->grpPattern->Controls->Add(this->txtPattern);
    this->grpPattern->Controls->Add(this->lblMaxDistance);
    this->grpPattern->Controls->Add(this->numMaxDistance);
    this->grpPattern->Controls->Add(this->lblMotifType);
    this->grpPattern->Controls->Add(this->cmbMotifType);

    // ===== ANALYSIS GROUP BOX =====
    this->grpAnalysis = gcnew GroupBox();
    this->grpAnalysis->Text = L"Analysis Methods";
    this->grpAnalysis->Location = Point(625, 170);
    this->grpAnalysis->Size = System::Drawing::Size(700, 95);
    this->grpAnalysis->Font = gcnew System::Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);

    // Analysis Buttons
    this->btnExactMatch = gcnew Button();
    this->btnExactMatch->Text = L"Exact Match\n(KMP)";
    this->btnExactMatch->Location = Point(15, 25);
    this->btnExactMatch->Size = System::Drawing::Size(100, 60);
    this->btnExactMatch->BackColor = Color::FromArgb(0, 122, 204);
    this->btnExactMatch->ForeColor = Color::White;
    this->btnExactMatch->Click += gcnew EventHandler(this, &MainForm::btnExactMatch_Click);

    this->btnApproxMatch = gcnew Button();
    this->btnApproxMatch->Text = L"Approx Match\n(Levenshtein)";
    this->btnApproxMatch->Location = Point(115, 25);
    this->btnApproxMatch->Size = System::Drawing::Size(100, 60);
    this->btnApproxMatch->BackColor = Color::FromArgb(0, 150, 136);
    this->btnApproxMatch->ForeColor = Color::White;
    this->btnApproxMatch->Click += gcnew EventHandler(this, &MainForm::btnApproxMatch_Click);

    this->btnMotifSearch = gcnew Button();
    this->btnMotifSearch->Text = L"Search Motif\n(Aho-Corasick)";
    this->btnMotifSearch->Location = Point(215, 25);
    this->btnMotifSearch->Size = System::Drawing::Size(100, 60);
    this->btnMotifSearch->BackColor = Color::FromArgb(156, 39, 176);
    this->btnMotifSearch->ForeColor = Color::White;
    this->btnMotifSearch->Click += gcnew EventHandler(this, &MainForm::btnMotifSearch_Click);

    this->btnAllMotifs = gcnew Button();
    this->btnAllMotifs->Text = L"All Motifs\n(Multi-Pattern)";
    this->btnAllMotifs->Location = Point(315, 25);
    this->btnAllMotifs->Size = System::Drawing::Size(100, 60);
    this->btnAllMotifs->BackColor = Color::FromArgb(233, 30, 99);
    this->btnAllMotifs->ForeColor = Color::White;
    this->btnAllMotifs->Click += gcnew EventHandler(this, &MainForm::btnAllMotifs_Click);

    this->btnPDADetail = gcnew Button();
    this->btnPDADetail->Text = L"PDA Detail\n(Stack Trace)";
    this->btnPDADetail->Location = Point(415, 25);
    this->btnPDADetail->Size = System::Drawing::Size(100, 60);
    this->btnPDADetail->BackColor = Color::FromArgb(255, 87, 34);
    this->btnPDADetail->ForeColor = Color::White;
    this->btnPDADetail->Click += gcnew EventHandler(this, &MainForm::btnPDADetail_Click);

    this->btnStatistics = gcnew Button();
    this->btnStatistics->Text = L"Statistics\n(GC Content)";
    this->btnStatistics->Location = Point(515, 25);
    this->btnStatistics->Size = System::Drawing::Size(100, 60);
    this->btnStatistics->BackColor = Color::FromArgb(76, 175, 80);
    this->btnStatistics->ForeColor = Color::White;
    this->btnStatistics->Click += gcnew EventHandler(this, &MainForm::btnStatistics_Click);

    // Add buttons to analysis group
    this->grpAnalysis->Controls->Add(this->btnExactMatch);
    this->grpAnalysis->Controls->Add(this->btnApproxMatch);
    this->grpAnalysis->Controls->Add(this->btnMotifSearch);
    this->grpAnalysis->Controls->Add(this->btnAllMotifs);
    this->grpAnalysis->Controls->Add(this->btnPDADetail);
    this->grpAnalysis->Controls->Add(this->btnStatistics);

    // Add top panel controls
    this->pnlTop->Controls->Add(this->lblTitle);
    this->pnlTop->Controls->Add(this->lblSequenceInput);
    this->pnlTop->Controls->Add(this->txtSequence);
    this->pnlTop->Controls->Add(this->btnLoadFile);
    this->pnlTop->Controls->Add(this->btnClear);
    this->pnlTop->Controls->Add(this->btnValidate);
    this->pnlTop->Controls->Add(this->grpPattern);
    this->pnlTop->Controls->Add(this->grpAnalysis);

    // ===== SPLIT CONTAINER =====
    this->splitMain = gcnew SplitContainer();
    this->splitMain->Dock = DockStyle::Fill;
    this->splitMain->SplitterDistance = 700;
    this->splitMain->BackColor = Color::Gray;

    // ===== RESULTS PANEL (LEFT) =====
    this->pnlResults = gcnew Panel();
    this->pnlResults->Dock = DockStyle::Fill;
    this->pnlResults->Padding = System::Windows::Forms::Padding(5);

    this->lblResults = gcnew Label();
    this->lblResults->Text = L"Results";
    this->lblResults->Dock = DockStyle::Top;
    this->lblResults->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
    this->lblResults->Height = 35;
    this->lblResults->TextAlign = ContentAlignment::MiddleLeft;

    // DataGridView
    this->dgvResults = gcnew DataGridView();
    this->dgvResults->Dock = DockStyle::Fill;
    this->dgvResults->AllowUserToAddRows = false;
    this->dgvResults->AllowUserToDeleteRows = false;
    this->dgvResults->ReadOnly = true;
    this->dgvResults->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
    this->dgvResults->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
    this->dgvResults->BackgroundColor = Color::White;
    this->dgvResults->BorderStyle = BorderStyle::None;

    // Columns
    this->colPosition = gcnew DataGridViewTextBoxColumn();
    this->colPosition->Name = L"Position";
    this->colPosition->HeaderText = L"Position";
    this->colPosition->FillWeight = 15;

    this->colSequence = gcnew DataGridViewTextBoxColumn();
    this->colSequence->Name = L"Sequence";
    this->colSequence->HeaderText = L"Matched Sequence";
    this->colSequence->FillWeight = 35;

    this->colDistance = gcnew DataGridViewTextBoxColumn();
    this->colDistance->Name = L"Distance";
    this->colDistance->HeaderText = L"Edit Distance";
    this->colDistance->FillWeight = 15;

    this->colType = gcnew DataGridViewTextBoxColumn();
    this->colType->Name = L"Type";
    this->colType->HeaderText = L"Type/Motif";
    this->colType->FillWeight = 20;

    this->colAlgorithm = gcnew DataGridViewTextBoxColumn();
    this->colAlgorithm->Name = L"Algorithm";
    this->colAlgorithm->HeaderText = L"Algorithm";
    this->colAlgorithm->FillWeight = 15;

    this->dgvResults->Columns->Add(this->colPosition);
    this->dgvResults->Columns->Add(this->colSequence);
    this->dgvResults->Columns->Add(this->colDistance);
    this->dgvResults->Columns->Add(this->colType);
    this->dgvResults->Columns->Add(this->colAlgorithm);

    this->pnlResults->Controls->Add(this->dgvResults);
    this->pnlResults->Controls->Add(this->lblResults);

    // ===== TRACE PANEL (RIGHT) =====
    this->pnlTrace = gcnew Panel();
    this->pnlTrace->Dock = DockStyle::Fill;
    this->pnlTrace->Padding = System::Windows::Forms::Padding(5);

    this->lblTrace = gcnew Label();
    this->lblTrace->Text = L"Execution Trace";
    this->lblTrace->Dock = DockStyle::Top;
    this->lblTrace->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
    this->lblTrace->Height = 35;
    this->lblTrace->TextAlign = ContentAlignment::MiddleLeft;

    this->txtTrace = gcnew RichTextBox();
    this->txtTrace->Dock = DockStyle::Fill;
    this->txtTrace->Font = gcnew System::Drawing::Font(L"Consolas", 9);
    this->txtTrace->BackColor = Color::FromArgb(30, 30, 30);
    this->txtTrace->ForeColor = Color::FromArgb(220, 220, 220);
    this->txtTrace->ReadOnly = true;
    this->txtTrace->WordWrap = false;

    this->pnlTrace->Controls->Add(this->txtTrace);
    this->pnlTrace->Controls->Add(this->lblTrace);

    // Add panels to split container
    this->splitMain->Panel1->Controls->Add(this->pnlResults);
    this->splitMain->Panel2->Controls->Add(this->pnlTrace);

    // ===== STATUS BAR =====
    this->statusStrip = gcnew StatusStrip();
    this->statusLabel = gcnew ToolStripStatusLabel();
    this->statusLabel->Text = L"Ready";
    this->statusLabel->Spring = true;
    this->statusLabel->TextAlign = ContentAlignment::MiddleLeft;
    this->statusStrip->Items->Add(this->statusLabel);

    // ===== ADD TO FORM =====
    this->Controls->Add(this->splitMain);
    this->Controls->Add(this->pnlTop);
    this->Controls->Add(this->statusStrip);

    this->ResumeLayout(false);
    this->PerformLayout();
}

void MainForm::InitializeAnalyzer(void) {
    analyzer_ = gcnew ManagedSequenceAnalyzer();
    UpdateStatus("Analyzer initialized.  Load sequence to begin.");
}

// ===== EVENT HANDLERS =====

void MainForm::btnLoadFile_Click(System::Object^ sender, System::EventArgs^ e) {
    OpenFileDialog^ dialog = gcnew OpenFileDialog();
    dialog->Filter = "FASTA Files (*.fasta;*.fa;*.fna)|*.fasta;*.fa;*.fna|Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
    dialog->Title = "Select DNA/RNA Sequence File";

    if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        try {
            String^ content = System::IO::File::ReadAllText(dialog->FileName);
            txtSequence->Text = content;
            UpdateStatus("File loaded: " + dialog->FileName);
        }
        catch (Exception^ ex) {
            MessageBox::Show("Error loading file: " + ex->Message, "Error",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
}

void MainForm::btnClear_Click(System::Object^ sender, System::EventArgs^ e) {
    txtSequence->Clear();
    txtPattern->Clear();
    ClearResults();
    UpdateStatus("Cleared all data.");
}

void MainForm::btnValidate_Click(System::Object^ sender, System::EventArgs^ e) {
    if (String::IsNullOrWhiteSpace(txtSequence->Text)) {
        MessageBox::Show("Please enter a sequence first.", "Validation",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    auto result = analyzer_->ValidateSequence(txtSequence->Text);

    if (result->IsValid) {
        String^ msg = "✓ Sequence is VALID\n\n";
        msg += "Cleaned sequence length: " + result->CleanedSequence->Length + " bp";

        if (result->Warnings->Count > 0) {
            msg += "\n\nWarnings:\n";
            for each (String ^ warn in result->Warnings) {
                msg += "• " + warn + "\n";
            }
        }

        MessageBox::Show(msg, "Validation Result",
            MessageBoxButtons::OK, MessageBoxIcon::Information);

        // Update sequence with cleaned version
        txtSequence->Text = result->CleanedSequence;
        UpdateStatus("Sequence validated successfully.");
    }
    else {
        String^ msg = "✗ Sequence is INVALID\n\nErrors:\n";
        for each (String ^ err in result->Errors) {
            msg += "• " + err + "\n";
        }

        MessageBox::Show(msg, "Validation Failed",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        UpdateStatus("Sequence validation failed.");
    }
}

void MainForm::btnExactMatch_Click(System::Object^ sender, System::EventArgs^ e) {
    if (!ValidateInputs()) return;

    try {
        analyzer_->SetSequence(txtSequence->Text);
        auto results = analyzer_->ExactMatch(txtPattern->Text);

        DisplayResults(results, "KMP");

        // Display DFA trace
        String^ trace = analyzer_->GetDFATrace();
        DisplayTrace(trace);

        UpdateStatus("Exact match completed.  Found " + results->Count + " matches.");
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error during exact match: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

void MainForm::btnApproxMatch_Click(System::Object^ sender, System::EventArgs^ e) {
    if (!ValidateInputs()) return;

    try {
        analyzer_->SetSequence(txtSequence->Text);
        int maxDist = (int)numMaxDistance->Value;
        auto results = analyzer_->ApproximateMatch(txtPattern->Text, maxDist);

        DisplayResults(results, "Levenshtein");

        // Display DFA trace
        String^ trace = analyzer_->GetDFATrace();
        DisplayTrace(trace);

        UpdateStatus("Approximate match completed. Found " + results->Count + " matches (max distance: " + maxDist + ").");
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error during approximate match: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

void MainForm::btnMotifSearch_Click(System::Object^ sender, System::EventArgs^ e) {
    if (String::IsNullOrWhiteSpace(txtSequence->Text)) {
        MessageBox::Show("Please enter a DNA/RNA sequence first.", "Input Required",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    try {
        analyzer_->SetSequence(txtSequence->Text);
        int motifType = cmbMotifType->SelectedIndex + 1; // 1-based index
        auto results = analyzer_->SearchMotif(motifType);

        DisplayResults(results, "Aho-Corasick");

        // Display DFA trace
        String^ trace = analyzer_->GetDFATrace();
        DisplayTrace(trace);

        UpdateStatus("Motif search completed. Found " + results->Count + " occurrences of " + cmbMotifType->Text + ".");
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error during motif search: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

void MainForm::btnAllMotifs_Click(System::Object^ sender, System::EventArgs^ e) {
    if (String::IsNullOrWhiteSpace(txtSequence->Text)) {
        MessageBox::Show("Please enter a DNA/RNA sequence first.", "Input Required",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    try {
        analyzer_->SetSequence(txtSequence->Text);
        auto results = analyzer_->SearchAllMotifs();

        DisplayResults(results, "Aho-Corasick (Multi-Pattern)");

        // Display DFA trace
        String^ trace = analyzer_->GetDFATrace();
        DisplayTrace(trace);

        UpdateStatus("All motifs search completed. Found " + results->Count + " total motif occurrences.");
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error during all motifs search: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

void MainForm::btnPDADetail_Click(System::Object^ sender, System::EventArgs^ e) {
    if (!ValidateInputs()) return;

    try {
        analyzer_->SetSequence(txtSequence->Text);

        // Clear and attach PDA observer
        analyzer_->ClearPDALogger();
        analyzer_->AttachPDAObserver();

        // Perform PDA search
        auto results = analyzer_->PDASearch(txtPattern->Text);

        DisplayResults(results, "PDA");

        // Display detailed PDA trace
        String^ trace = analyzer_->GetPDATrace();
        DisplayTrace(trace);

        UpdateStatus("PDA detail analysis completed.  Found " + results->Count + " matches with detailed stack trace.");
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error during PDA detail: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

void MainForm::btnStatistics_Click(System::Object^ sender, System::EventArgs^ e) {
    if (String::IsNullOrWhiteSpace(txtSequence->Text)) {
        MessageBox::Show("Please enter a DNA/RNA sequence first.", "Input Required",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    try {
        analyzer_->SetSequence(txtSequence->Text);
        auto stats = analyzer_->GetStatistics();

        DisplayStatistics(stats);
        UpdateStatus("Statistics calculated successfully.");
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error calculating statistics: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

// ===== HELPER METHODS =====

bool MainForm::ValidateInputs() {
    if (String::IsNullOrWhiteSpace(txtSequence->Text)) {
        MessageBox::Show("Please enter a DNA/RNA sequence first.", "Input Required",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return false;
    }

    if (String::IsNullOrWhiteSpace(txtPattern->Text)) {
        MessageBox::Show("Please enter a pattern to search for.", "Input Required",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return false;
    }

    return true;
}

void MainForm::DisplayResults(List<ManagedMatchResult^>^ results, String^ algorithm) {
    ClearResults();

    lblResults->Text = "Results (" + results->Count + " matches) - Algorithm: " + algorithm;

    for each (ManagedMatchResult ^ result in results) {
        int rowIndex = dgvResults->Rows->Add();
        DataGridViewRow^ row = dgvResults->Rows[rowIndex];

        row->Cells["Position"]->Value = result->Position;
        row->Cells["Sequence"]->Value = result->MatchedSequence;
        row->Cells["Distance"]->Value = result->EditDistance;
        row->Cells["Type"]->Value = result->MotifType;
        row->Cells["Algorithm"]->Value = result->Algorithm;

        // Color coding based on edit distance
        if (result->EditDistance == 0) {
            row->DefaultCellStyle->BackColor = Color::FromArgb(200, 255, 200); // Light green
        }
        else if (result->EditDistance <= 2) {
            row->DefaultCellStyle->BackColor = Color::FromArgb(255, 255, 200); // Light yellow
        }
        else {
            row->DefaultCellStyle->BackColor = Color::FromArgb(255, 220, 220); // Light red
        }
    }
}

void MainForm::DisplayStatistics(ManagedSequenceStatistics^ stats) {
    ClearResults();

    lblResults->Text = "Sequence Statistics";

    // Basic statistics
    String^ statsText = "=== SEQUENCE STATISTICS ===\n\n";
    statsText += "Total Length: " + stats->TotalLength + " bp\n";
    statsText += "Sequence Type: " + stats->SequenceType + "\n\n";

    statsText += "=== NUCLEOTIDE COMPOSITION ===\n";
    statsText += String::Format("Adenine  (A): {0,6} ({1,5:F2}%)\n", stats->AdenineCount, stats->AdeninePercent);
    statsText += String::Format("Thymine  (T): {0,6} ({1,5:F2}%)\n", stats->ThymineCount, stats->ThyminePercent);
    statsText += String::Format("Guanine  (G): {0,6} ({1,5:F2}%)\n", stats->GuanineCount, stats->GuaninePercent);
    statsText += String::Format("Cytosine (C): {0,6} ({1,5:F2}%)\n", stats->CytosineCount, stats->CytosinePercent);
    statsText += String::Format("Uracil   (U): {0,6} ({1,5:F2}%)\n\n", stats->UracilCount, stats->UracilPercent);

    statsText += String::Format("GC Content: {0:F2}%\n\n", stats->GCContent);

    // Motifs found
    if (stats->Motifs->Count > 0) {
        statsText += "=== MOTIFS FOUND ===\n";
        for each (ManagedMotifOccurrence ^ motif in stats->Motifs) {
            statsText += "\n" + motif->MotifName + " (" + motif->MotifPattern + ")\n";
            statsText += "  Count: " + motif->Count + "\n";
            statsText += "  Positions: ";

            for (int i = 0; i < Math::Min(10, motif->Positions->Count); i++) {
                statsText += motif->Positions[i] + " ";
            }

            if (motif->Positions->Count > 10) {
                statsText += "... (" + (motif->Positions->Count - 10) + " more)";
            }
            statsText += "\n";
        }
    }
    else {
        statsText += "=== NO MOTIFS FOUND ===\n";
    }

    DisplayTrace(statsText);
}

void MainForm::DisplayTrace(String^ trace) {
    txtTrace->Clear();
    txtTrace->Text = trace;

    // Syntax highlighting
    txtTrace->SelectAll();
    txtTrace->SelectionColor = Color::FromArgb(220, 220, 220);

    // Highlight headers
    int index = 0;
    while ((index = txtTrace->Find("===", index, RichTextBoxFinds::None)) != -1) {
        txtTrace->Select(index, 3);
        txtTrace->SelectionColor = Color::FromArgb(100, 200, 255);
        index += 3;
    }

    // Highlight MATCH
    index = 0;
    while ((index = txtTrace->Find("MATCH", index, RichTextBoxFinds::None)) != -1) {
        txtTrace->Select(index, 5);
        txtTrace->SelectionColor = Color::FromArgb(100, 255, 100);
        index += 5;
    }

    // Highlight PUSH
    index = 0;
    while ((index = txtTrace->Find("PUSH", index, RichTextBoxFinds::None)) != -1) {
        txtTrace->Select(index, 4);
        txtTrace->SelectionColor = Color::FromArgb(255, 200, 100);
        index += 4;
    }

    // Highlight POP
    index = 0;
    while ((index = txtTrace->Find("POP", index, RichTextBoxFinds::None)) != -1) {
        txtTrace->Select(index, 3);
        txtTrace->SelectionColor = Color::FromArgb(255, 150, 150);
        index += 3;
    }

    // Reset selection
    txtTrace->Select(0, 0);
}

void MainForm::ClearResults() {
    dgvResults->Rows->Clear();
    txtTrace->Clear();
    lblResults->Text = "Results";
}

void MainForm::UpdateStatus(String^ message) {
    statusLabel->Text = message + " | " + DateTime::Now.ToString("HH:mm:ss");
}

