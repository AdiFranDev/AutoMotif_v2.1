// Prevent Windows SDK conflicts
#define _WINSOCKAPI_
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "MainForm.h"

using namespace DNACoreBridge;
using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
    // Enable visual styles
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Create and run main form
    DNAAnalyzerGUI::MainForm^ form = gcnew DNAAnalyzerGUI::MainForm();
    Application::Run(form);

    return 0;
}