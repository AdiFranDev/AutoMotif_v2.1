# Authors & Contributors
    Francis Adrian Esteban 
    John Carl Ramirez
    Cydney Ruelo
    Julien Marabe 


# AutoMotif DNA and RNA Sequence Analyzer

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![.NET](https://img.shields.io/badge/.NET-Framework%204.8-purple)
![C++](https://img.shields.io/badge/C++-17-blue)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)

A sophisticated DNA and RNA sequence analysis tool featuring multiple pattern matching algorithms and motif detection capabilities.

![Application Screenshot](docs/screenshots/main-window.png)

## 🧬 Features

- **Multiple Search Algorithms**
  - **KMP (Knuth-Morris-Pratt)**: Exact pattern matching
  - **Levenshtein Distance**: Approximate matching with edit distance
  - **Aho-Corasick**: Multi-pattern motif search
  - **PDA (Pushdown Automaton)**: Detailed stack-trace analysis

- **Motif Detection**
  - TATA Box
  - CAAT Box
  - GC Box
  - Kozak Sequence
  - Poly-A Signal

- **Sequence Analysis**
  - Nucleotide composition
  - GC content calculation
  - Sequence validation
  - FASTA file support

- **Rich UI Features**
  - Color-coded results based on match quality
  - Detailed execution traces
  - Real-time validation
  - Export capabilities

## 🚀 Getting Started

### Prerequisites

- Visual Studio 2019 or later
- .NET Framework 4.8
- Windows SDK 10.0 or later
- C++17 compiler support

### Installation

1. **Clone the repository**

git clone https://github.com/yourusername/DNAAnalyzerGUI.git 
cd DNAAnalyzerGUI

2. **Open the solution**

start DNAAnalyzerGUI.sln

3. **Build the project**
- Set configuration to `Release` or `Debug`
- Build → Build Solution (Ctrl+Shift+B)

4. **Run the application**
- Press F5 or click Start

## 📖 Usage

### Basic Workflow

1. **Load Sequence**
- Click "Load FASTA" to import from file, or
- Paste sequence directly into the text box

2. **Validate Sequence**
- Click "Validate" to check sequence integrity
- View warnings and errors if any

3. **Search for Patterns**
- **Exact Match**: Enter pattern → Click "Exact Match (KMP)"
- **Approximate Match**: Set max distance → Click "Approx Match"
- **Motif Search**: Select motif type → Click "Search Motif"
   - **All Motifs**: Click "All Motifs" to find all predefined motifs

4. **View Results**
   - Results appear in the data grid
   - Execution trace shows in the right panel
   - Color coding indicates match quality

### Example

Sequence: ATGCGATATATAAATCGATCG Pattern: TATA Max Distance: 1
Results:
•	Position 9: TATA (exact match)
•	Position 11: ATAA (1 edit distance)

## 🏗️ Project Structure

DNAAnalyzerGUI/
├── .gitignore
├── README.md
├── LICENSE
├── DNAAnalyzerGUI.sln
├── DNAAnalyzerGUI/
│   ├── MainForm.h
│   ├── MainForm.cpp
│   ├── MainForm.resx
│   └── DNAAnalyzerGUI.vcxproj
├── DNACoreBridge/
│   └── (your bridge code)
├── DNACore/
│   └── (your core C++ code)
└── docs/
    ├── screenshots/
    └── user-guide.md


## 🔧 Architecture

### Technology Stack

- **GUI Layer**: Windows Forms (.NET Framework)
- **Bridge Layer**: C++/CLI
- **Core Layer**: Native C++17

### Key Components

graph TD A[Windows Forms UI] --> B[C++/CLI Bridge] B --> C[Native C++ Core] C --> D[KMP Algorithm] C --> E[Levenshtein Distance] C --> F[Aho-Corasick] C --> G[PDA Engine]


## 🧪 Algorithms

### 1. KMP (Knuth-Morris-Pratt)
- **Time Complexity**: O(n + m)
- **Use Case**: Fast exact pattern matching
- **Features**: Preprocessing for efficient search

### 2. Levenshtein Distance
- **Time Complexity**: O(n × m)
- **Use Case**: Finding approximate matches
- **Features**: Configurable edit distance threshold

### 3. Aho-Corasick
- **Time Complexity**: O(n + m + z)
- **Use Case**: Multiple pattern matching
- **Features**: Simultaneous search for multiple motifs

### 4. PDA (Pushdown Automaton)
- **Use Case**: Complex pattern recognition
- **Features**: Stack-based state tracking with detailed traces

## 📊 Performance

- **Typical Sequence Length**: Up to 1,000,000 bp
- **Pattern Length**: 3-50 bp
- **Max Edit Distance**: 0-10
- **Memory Usage**: ~100MB for typical workloads

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Coding Standards

- Follow Microsoft C++ coding conventions
- Use meaningful variable names
- Comment complex algorithms
- Include unit tests for new features

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👥 Authors

- **Your Name** - *Initial work* - [YourGitHub](https://github.com/yourusername)

## 🙏 Acknowledgments

- Inspired by bioinformatics research needs
- Algorithm implementations based on seminal computer science papers
- Thanks to the open-source community

## 📧 Contact

- **Email**: your.email@example.com
- **Project Link**: https://github.com/yourusername/DNAAnalyzerGUI
- **Issues**: https://github.com/yourusername/DNAAnalyzerGUI/issues

## 🗺️ Roadmap

- [ ] Add support for protein sequences
- [ ] Implement BLAST-like local alignment
- [ ] Export results to CSV/JSON
- [ ] Add visualization of matches
- [ ] Cross-platform support (Linux/macOS)
- [ ] Web-based version

## 📚 Documentation

For detailed documentation, see the [docs](docs/) directory:
- [User Guide](docs/user-guide.md)
- [Algorithm Details](docs/algorithms.md)
- [API Reference](docs/api-reference.md)

---

**Note**: This application is for educational and research purposes. For production bioinformatics workflows, consider specialized tools like BLAST, BWA, or Bowtie.

