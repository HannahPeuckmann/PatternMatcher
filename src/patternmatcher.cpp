// PatternMatcher.cpp
// Programm zur Stringsuche in Textdateien auf Basis einer Pattern Matching Machine
// nach Aho/Corasick (1975): Efficient string matching
// Hannah Peuckmann 04.03.20
// g++ 7.4.0

#include <iostream>

#include "../include/PatternMatcher.hpp"


/**
 * \brief Überprüft die User Eingabe auf Gültigkeit
 * \param mode Modus, mit dem die Eingabe abgeglichen wird
 * \param user_mode Eingabe des Users ,die abgeglichen wird
 * \return 0 falls user_mode erkannt wurde, 1 andernfalls
**/

// Testet, ob Usermodus mit einem bestimmten Modus übereinstimmt
bool is_mode(std::string mode, char* user_mode) {
	for (int i = 0; i < mode.length(); ++i) {
		if (user_mode[i] == '\0') // Eingabe endet zu früh
			return false;
    // Zeichenweise prüfen  
		else if (mode[i] != user_mode[i])
			return false;
	}
  // Beide Argumente gleich und gleich lang
	if (user_mode[mode.length()] == '\0')
		return true;
	return false;
}


/**
 * \brief main-Funktion, Verarbeitet die User-Eingabe übers Terminal und bestimmt welche Funktionalität aufgerufen wird
 * \param argc Anzahl der vom User überreichten Argumente
 * \param argv Vektor, der die User-Eingabe enthält
 * \return 0 wenn das Programm durchläuft, 1 falls Fehlermeldungen auftreten
**/
int main(int argc, char** argv)
{
  if (argc != 4 && argc !=6) {
    // Synopsis
    std::cerr << "USAGE training: make-patternmatcher -train TOKEN_TEXTFILE TRIE_TEXTFILE\n";
    std::cerr << "Optional in train mode: creating a dot file of saved pattern matcher\n";
    std::cerr << "USAGE dot: make-patternmatcher -train -dot TOKEN_TEXTFILE TRIE_TEXTFILE DOT_FILE\n";
    std::cerr << "TRIE_TEXTFILE: Name of a textfile to save the Trie of the pattern matcher created in training\n";
    std::cerr << "TOKEN_TEXTFILE is an ISO-encoded textfile with a single token per line\n";
    std::cerr << "USAGE searchmode: make-patternmatcher -search SEARCH_TEXTFILE TRIE_TEXTFILE\n";
    std::cerr << "SEARCH_TEXTFILE is an ISO-encoded textfile\n";
    exit(1);
  }

	if (is_mode("-train", argv[1])) {
    // Optionales Erstellen einer dot-file
    if (is_mode("-dot",argv[2])){
      PatternMatcher patternmatcher(argv[3]);
      patternmatcher.safe_as_txt(argv[4]);
      patternmatcher.draw_as_dot(argv[5]);
      std::cerr<<"Use dot -Gcharset=latin1 -Tpdf DOT_FILENAME -o DOTFILENMAE.pdf to draw dot as pdf\n";
    }
    else{
      // Pattern-Matcher erstellen aus token-Datei
      PatternMatcher patternmatcher(argv[2]);
      patternmatcher.safe_as_txt(argv[3]);
    }
  }
	else if (is_mode("-search", argv[1])) {
    // Pattern-Matcher mit leerem Trie erstellen
    PatternMatcher patternmatcher;  
    patternmatcher.read_from_txt(argv[3]);
    patternmatcher.search(argv[2]);
  
  }

  // Fehlermeldungen wenn der Modus nicht erkannt wurde
  else{
    std::cerr<<"Error: Unknown Mode\n";
    std::cerr<<"-train TOKEN_TEXTFILE TRIE_TEXTFILE: To create a pattern matching machine from tokens\n";
    std::cerr<<"in TOKEN_TEXTFILE and save it in TRIE_TEXTFILE\n";
    std::cerr<<"-train -dot TOKEN_TEXTFILE TRIE_TEXTFILE DOT_FILE: To create a dot file in addition\n";
    std::cerr<<"-search SEARCH_TEXTFILE TRIE_TEXTFILE\n: To search in SEARCH_TEXTFILE with a in -train saved pattern matching machine\n";
  }
} 
