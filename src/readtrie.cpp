// readtrie.cpp
// Das Programm baut einen Pattern-Matcher und schreibt alle im Trie des Pattern-matchers enthaltenen
// Wörter in ein neues Textdokument
// Hannah Peuckmann 04.03.20
//g++ 7.4.0

#include <iostream>
#include <fstream>
#include <string>
#include "../include/PatternMatcher.hpp"


/**
 * \brief lies ein dokument ein, erstellt daraus einen Pattern-Matcher und schreibt
 * die nun im Trie des Pattern-Matchers gespeicherten Wörter in eine neues Dokument
 * \param filename_in Name des Textdokuments, aus dem der Pattern-Matcher erstellt wird
 * \param filename_out Name des Textdokuments, in das die im Pattern-Matcher gespeicherten Wörter geschrieben werden
 **/
bool read(std::string filename_in, std::string filename_out){
    if(filename_out!="" && filename_in!=""){
        std::ifstream line_in(filename_in.c_str());
        std::ofstream line_out(filename_out);
        if(line_in){
            // Pattern-Matcher wird erstellt
            PatternMatcher patternmatcher(filename_in.c_str());
            std::string line;
            while (std::getline(line_in,line)) {
                // Prüft für jedes Wort, ob es im Trie ist
                if(patternmatcher.in(line)){
                    // Wenn ja, wird es in eine Datei geschrieben
                    line_out<<line<<'\n';
                }
            }
            line_in.close();
            line_out.close();
            return true;
        }
        else{
            std::cerr<<"FATAL: File is empty";
            return false;
            }
    }
    else{
        std::cerr<<"FATAL: Filenames are not valid\n";
        return false;
        }
}


/**
 * \brief main-Funktion, verarbeitet die Eingabe und delegiert diese an read()
 * \param argc Anzahl der vom User überreichten Argumente
 * \param argv Vektor, der die User-Eingabe enthält
 * \return 0 wenn das Programm durchläuft, 1 falls Fehlermeldungen auftreten
**/ 
int main(int argc, char** argv){
        if(!read(argv[1],argv[2])){
            std::cerr<<"USAGE: testtrie TOKEN_TEXTFILE COMPARE_TEXTFILE\n";
            std::cerr<<"TOKEN_TEXTFILE is an ISO-encoded textfile with a single token per line\n";
            std::cerr<<"CCOMPARE_TEXTFILE is a textfile to write the contructed pattern matcher's tokens to";
        }
}
