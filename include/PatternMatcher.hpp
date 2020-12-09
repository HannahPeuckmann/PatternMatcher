// PatternMatcher.hpp
// pattern matching machine 
// Klasse zur Stringsuche in Textdateien auf Basis eines dyn. Buchstabenbaumes (Trie)
// nach Aho/Corasick (1975): Efficient string matching
// Hannah Peuckmann 04.03.20
// Auf Basis von dyn. Buchstabenb�ume (Fredkin, 1956)
// g++ 7.4.0


#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/** 
 * \brief PatternMatcher: Pattern matching machine to match strings
 * \details Klasse zur Stringsuche in Textdateien auf Basis eines Tries  
*/
class PatternMatcher
{
public:
  /// �ffentlicher Typ
  typedef unsigned State;
  
public:
  /** 
    * \brief Default-Konstruktor, erzeugt einen leeren Pattern-Matching-Trie 
  */
  PatternMatcher(){}
  /** 
    * \brief Delegierender Konstruktor, erzeugt einen Pattern-Matching-Trie aus input_filename 
    * \param input_filename Textdatei, ISO-8859-1 kodiert, die ein Token pro Zeile enth�lt
  */
  PatternMatcher(std::string input_filename) 
  {
    // Startzustand anlegen
    states.push_back(StateDesc());
    // Datei mit Suchw�rtern �ffnen, wortweise lesen und W�rter einf�gen
    if (input_filename != "") {
      std::ifstream words_in(input_filename.c_str());
      if (words_in) {
        std::string word;
        while (std::getline(words_in,word)) {
          add(word);
        } 
      }
      else {
        std::cerr << "FATAL: Unable to open " << input_filename << "\n";
        exit(1);
      }
    words_in.close();
    }
    else{
      std::cerr << "FATAL: No token filename\n";
      exit(1);}
    if(states.size()<2){
      // Es wurde nur der Startzustand erstellt
      std::cerr<< "FATAL: Empty token file\n";
      exit(1);
    }
    // F�r jeden Zustand des fertigen Tries wird ein Fehlerzustand bestimmt
    make_failure();
  }


  /** 
    * \brief Pattern matching machine: Sucht in einer Textdatei nach den W�rtern im Trie und gibt die Funde im Terminal aus
    * \param input_filename Textdatei, in der gesucht werden soll 
    * \details Wenn der Trie leer ist, wird eine Fehlermeldung ausgegeben
  */
  void search(std::string input_filename){
    if(states.size()<2){
      std::cerr<< "FATAL: Pattern matching machine has no tokens to search for\n";
      exit(1);
    }
    // Datei �ffnen und einlesen
    if (input_filename != "") {
      std::ifstream line_in(input_filename.c_str());
      if (line_in) {
        std::string line;
        unsigned line_idx=0;
        bool found_toks=false;
        std::cout<<"filename\t"<<"Zeile\t"<<"Tokens"<<'\n';
        // Zeilenweise verarbeiten
        while (std::getline(line_in,line)) {
          // Pro Zeile nach den Tokens im Trie suchen
          std::vector<std::string> tokens = search_line(line);
          // Wenn Tokens gefunden wurden, wird eine Ausgabe gemacht
          if(!tokens.empty()){
            found_toks=true;
            // Ausgabe
            std::cout<<input_filename<<'\t'<<line_idx+1<<'\t';
            for(int i =0; i<tokens.size();++i){
              // Tokens werden der Reihe des Auftretens in der Zeile nach ausgegeben
              std::cout<<tokens[i]<<'\t';
            }
            std::cout<<'\n';
          }
          ++line_idx;
        }
        if(found_toks==false){
          std::cout<<"...No matches found\n";
        } 
      }
      else {
        std::cerr << "FATAL: Unable to open " << input_filename << "\n";
      }
    line_in.close();
    }
    else{
      std::cerr << "FATAL: No file to search in\n";
      exit(1);
    }
  }


  /**
    * \brief Speichert den erstellten Trie tabellarisch in txt Format ab
    * \param txt_triefile_out Datei, in der der Trie gespeichert wird, wird vom User �ber das Terminal �bergeben
  */


  void safe_as_txt(std::string txt_triefile_out){
    // Datei wird erstellt
    if (txt_triefile_out != ""){
      std::ofstream txt_out(txt_triefile_out);
      if(!txt_out){
        std::cerr<<"ERROR: Unable to open txt file for writing"<<std::endl;
        return;
      }
      // Jeder Zustand ist eine Zeile
      for(int i=0;i<states.size();++i){
        StateDesc current = states[i];
        txt_out<<"s"<<'\t'<<current.is_final<<'\t'<<current.tr_index<<'\t'<<current.failure<<'\t'<<current.output<<'\n';
      }
      // Jeder �bergang ist eine Zeile
      for(int i=0;i<transitions.size();++i){
        Transition current=transitions[i];
        txt_out<<"t"<<'\t'<<current.letter<<'\t'<<current.state<<'\t'<<current.next<<'\n';
      }
      txt_out.close();
      std::cerr<<"Pattern-Matching machine was build and saved in "<<txt_triefile_out<<"\n";
    }
    else{
      std::cerr<<"Fatal: No filename to safe in\n";
      exit(1);
    }
  }

  /**
   * \brief Liest einen in txt-Form gespeicherten Pattern-Matching-Trie ein 
   * \param txt_triefile_in Datei, aus der der Trie eingelesen wird, wird vom User im Terminal �bergeben 
  */

  void read_from_txt(std::string txt_triefile_in){
    // Datei einlesen und zeilenweise verarbeiten
    // Startzustand einlesen
    if (txt_triefile_in != "") {
      std::ifstream line_in(txt_triefile_in.c_str());
      if (line_in) {
        std::string line;
        while (std::getline(line_in,line)) {
          if (line[0]=='s'){
            read_state(tokenize(line));
          }
          else if (line[0]=='t'){
            read_transition(tokenize(line));
          }  
        }
        line_in.close();
      }
      else {
        std::cerr << "FATAL: Unable to open " << txt_triefile_in << "\n";
      }
    }
    else{
      std::cerr<<"Fatal: No filename was given\n";
      exit(1);
    }
  }

  /**
   * \brief Sucht nach einem Wort im Trie
   * \param word Wort, nach dem im Trie gesucht werden soll 
   * \return Gibt true zur�ck gdw. das Wort im Trie enthalten ist 
  */
  /// Komplexit�t: O(|word|)
  bool in(std::string word)
  {
    // Beginnen beim Startzustand
    State q = 0;
    // Iteration �ber das Wort
    for (auto i = 0; i < word.size(); ++i) {
      // Vom aktuellen Zustand Nachfolgezustand bestimmen
      State next = delta(q,word[i]);
      if (next != State(-1)) {
        // Zustand existiert => von diesem aus weitersuchen
        q = next;
      }
      else {
        // Zustand existiert nicht => Pr�fix existiert nicht
        // => Wort nicht im Trie enthalten
        return false;
      }
    }
    return is_final(q);
  }

  /**
   * \brief F�gt ein Wort in den Trie ein, falls es noch nicht enthalten war 
   * \param word Wort, das in den Trie eingef�gt werden soll 
   * \return Gibt true zur�ck gdw Wort noch nicht im Trie enthalten war 
  */ 
  bool add(std::string word)
  {
	bool unknown_word = false;
    // Abgehen des Pr�fix beginnt beim Startzustand
    State current_state = 0;
    for (int i = 0; i < word.length(); ++i) {
      State next = delta(current_state,word[i]);
      // Neuen Zweig im Trie aufmachen, wenn kein Pr�fix von 'word' mehr im Trie ist 
      if (next == -1) {
        next = add_transition(current_state,word[i]);
        unknown_word = true;
      }
      current_state=next;
    }
    
    // Letzter neu erstellter Zustand wird ein Endzustand
    states[current_state].is_final=true;
    // Endzustand wird 'word' als Output zugewiesen
    states[current_state].output=word;
    return unknown_word;
  }

  /**
   * \brief Erzeugt eine Graphviz-dot-Repr�sentation des Pattern-Matching-Tries
   * \param dot_filename Zu erstellende dot-Datei 
   * \details Fehler�berg�nge werden ebenfalls  dargestellt
  */
  void draw_as_dot(std::string dot_filename)
  {
    if(dot_filename!=""){
      // Datei erstellen
      std::ofstream dot_out(dot_filename); // 
      if(!dot_out){
        std::cerr<<"ERROR:Unable to open dot file for writing"<<std::endl;
        return;
      }
      dot_out<<"digraph A { \nrankdir=\"LR\"\n";
      // Zust�nde schreiben
      for(int i=0;i<states.size();++i){
        // Endzust�nde
        if(is_final(State(i))){
          dot_out<<i <<" [shape=doublecircle]\n";
        }
        else{
          dot_out<<i <<" [shape=circle]\n";
        }
        // �berg�nge schreiben
        for (auto idx = states[i].tr_index; idx != -1; idx = transitions[idx].next) {
          dot_out<<i<<" -> " << transitions[idx].state<<" [label = \""<<transitions[idx].letter<<"\"]\n";
        }
        // Fehlerzust�nde bzw. Fehler�berg�nge k�nnten dargestellt werden, das wird aber schnell un�bersichtlich
        //dot_out<<i<<" -> " << states[i].failure<<" [label = "failure"]\n";
      }
      dot_out<<"}\n";
      std::cerr<<dot_filename<<" wurde erstellt\n";
      dot_out.close();
    }
    else{
      std::cerr<<"FATAL: No dot-filename was given\n";
      exit(1);
    }
  }

  
private: // Typen
  /**
   * \brief Typ eines Zustands
  */
  struct StateDesc
  {
    StateDesc(bool b, int tr, State f, std::string out){is_final=b, tr_index=tr, failure=f, output=out;}
    // Default-Konstruktor
    StateDesc() { is_final = false; tr_index = -1; failure=0; output='\0'; }
    bool  is_final; ///< Finalit�t des Zustands
    int   tr_index; ///< Verweis in den �bergangsvektor
    State   failure; ///< Fehlerzustand
    std::string  output;/// <Token, falls Endzustand, sonst Epsilon 
  };
  
  /**
   * \brief Typ eines �bergangs
   * \param l Symbol am �bergang
   * \param s Folgezustand
   * \param n Index des n�chsten Schwester�bergangs in der Adjazenzliste
  */
  struct Transition
  {
    Transition(char l, State s, int n){letter=l, state=s, next=n;}
    // Default-Konstruktor 
    Transition() { letter = '\0'; state = -1; next = -1; }

    char      letter; ///< Label am �bergang
    State     state;  ///< Zielzustand
    int       next;   ///< N�chster �bergang der Adjazenzliste
  };

private: // Funktionen
  /**
   * \brief Erstellt einen Zustand im Zustands-Vektor
   * \param line Vektor aus Strings, enth�lt eine tokenisierte Zeile einer txt-Datei
   * \details Die Stringelemente des Vektors werden von der Funktion in die entsprechenden Typen
   * der Attribute eines States umgewandelt
  **/
  void read_state(std::vector<std::string> line){
    states.push_back(StateDesc(bool(std::stoi(line[1])),std::stoi(line[2]),std::stoi(line[3]),line[4]));
  }

  /**
   * \brief Tokenisiert einen string
   * \param line String aus einer txt-Datei
   * \details Tabs markieren die Tokens
  **/
  std::vector<std::string> tokenize(std::string line){
    std::vector<std::string> tokens;
    std::string tok;
    for(int i=0;i<line.length();++i){
      if(line[i]=='\t'){
        tokens.push_back(tok);
        tok="";
      }
      else if(i==line.length()-1){
        tok=tok+line[i];
        tokens.push_back(tok);
      }
      else{tok=tok+line[i];
      }
    }
    return tokens;

  }


  /**
   * \brief Erstellt einen �bergang im Transitions-Vektor
   * \param line Vektor, der eine tokenisierte Zeile aus einer txt-Datei enth�lt
   * \details Die einzelnen Stringelemente des Vektors line, werden von der Funktion in die entsprechenden Typen der Atribute 
   * einer Transition umgewandelt
  **/
  void read_transition(std::vector<std::string> line){
    transitions.push_back(Transition(line[1][0],stoi(line[2]),stoi(line[3])));
  }


  /**
   * \brief Liefert den Folgezustand f�r Zustand q mit Symbol a
   * \param q Ausgangszustand
   * \param a Symbol am �bergang
   * \return Folgezustand des �bergangs
   * \details Gibt -1 zur�ck, wenn kein �bergang existiert
  */
  State delta(State q, char a)
  {
    // Iteration �ber alle Folgezust�nde
    for (int daughteridx = states[q].tr_index; daughteridx != -1; daughteridx = transitions[daughteridx].next){
      // Pr�fen, ob das gesuchte Symbol am �bergang ist 
      if(transitions[daughteridx].letter==a){
        return transitions[daughteridx].state;
      }
    }
    return State(-1);
  }

  /**
   * \brief Modifiziert die R�ckgabe der delta-Funktion 
   * \param q Ausgangszustand
   * \param a Symbol am �bergang
   * \details Realisiert eine Schleife am Startzustand f�r alle Symbole, f�r die es keinen �bergang am Startzustand gibt 
  */
  State go_to(State q, char a){
    // Schleife am Startzustand
    State next=delta(q,a);
    if(q==0 && next==-1){
      // Symbole, die keinen �bergang am Startzustand haben, bekommen eine Schleife zum Startzustand 
      return 0;
      }
    else{return next;}
  }

  /**
   * \brief Erstellt einen neuen �bergang von Zustand q mit Symbol a
   * \param q Ausgangszustand
   * \param a Symbol am �bergang
   * \return Gibt neu erstellten Folgezustand des neuen �bergangs zur�ck  
  */
  State add_transition(State q, char a)
  { 
    // Neuen Folgezustand erstellen 
    states.push_back(StateDesc());
    // Name des neuen Zustands ist sein Index im States-Vektor
    State new_state=states.size()-1;
    // Neuer �bergang
    transitions.push_back(Transition(a, new_state, states[q].tr_index));
    // Ausgangszustand 'q' verweist auf neu erstellten �bergang
    states[q].tr_index=transitions.size()-1;
    // Neu erstellter Zustand wird zur�ckgegeben
    return new_state;
  }

  /**
   * \brief Bestimmt f�r jeden Zustand im Trie einen Fehlerzustand
   * \details Fehlerzustand ist by default der Startzustand
  */
  void make_failure(){
    // Queue erstellen, first in, first out
    // Vorderstes Element wird nicht gel�scht, Index, der auf das vorderste Element zeigt, wird inkrementiert
    std::vector <State> queue;
    State start=0;
    // Index des vordersten Zustands in der Queue   
    State front=0;
    // Alle T�chter des Startzustands werden auf die Queue gereiht
    for (auto idx = states[start].tr_index; idx != -1; idx = transitions[idx].next){
      queue.push_back(transitions[idx].state);
    }
    // Alle Zust�nde haben Startzustand 0 als failure by default
    while(front<queue.size()){
      // Alle T�chter des aktuellen Zustands auf die Queue reihen
      // Der aktuelle Zustand befindet sich am Index 'front' in der Queue
      for (auto idx = states[queue[front]].tr_index; idx != -1; idx = transitions[idx].next){
        queue.push_back(transitions[idx].state);
        // Der Fehlerzustand der T�chter, ist Fehlerzustand des Vorg�ngers. Der Vorg�nger ist der aktueller Zustand am Index Front
        State f_state = states[queue[front]].failure;
        // Sp�testens beim Startzustand wird 'go_to' einen �bergang finden, auf Grund der Schleife am Startzustand
        while (go_to(f_state,transitions[idx].letter)==-1)
        {
          // Bis ein �bergang gefunden wird, wird von Fehler zu Fehler gegangen 
          f_state=states[f_state].failure;
          
        }
        // Ein �bergang wurde gefunden, der Folgezustand dieses wird der aktuellen Tochter als Fehler zugewiesen 
        states[transitions[idx].state].failure=go_to(f_state,transitions[idx].letter);
       
      }
      ++front;
    }

  }

  /**
   * \brief Suchfunktion, Sucht jede Zeile nach den im Trie des Pattern-Matchers gespeicherten W�rtern ab
   * \return Gibt einen Vektor mit den Funden zur�ck
   * \param line Einzelne Textzeile, die abgesucht wird 
  */
  std::vector<std::string> search_line(std::string line){
    // Aktueller Zustand wird zu Beginn der Startzustand
    State state=0;
    std::vector<std::string> tokens;
    // �ber Symbole der Zeile iterieren 
    for (auto i = 0; i < line.size(); ++i){
      // �bergang wird gesucht, wird keiner gefunden, wird zum Fehlerzustand gegangen und von dort aus weiter gesucht 
      while(go_to(state,line[i])==-1){
        state=states[state].failure;
      }
      // �bergang wurde gefunden, Symbol wird verarbeitet, aktueller Zustand wechselt zum Folgezustand 
      state=go_to(state,line[i]);
      // Finalit�t pr�fen, bei Endzustand wurde ein Wort gefunden 
      if(is_final(state)){
        // gefundenes Wort und W�rter im Trie, die Pr�fix dieses Wortes sind, werden in Reihenfolge im Vektor gespeichert 
        tokens.push_back(outputfunction(state));
        if(is_final(states[state].failure)){
          tokens.push_back(outputfunction(states[state].failure));
        }
      }
    }
    return tokens;
  }

  /** 
   * \brief Pr�ft, ob ein Zustand ein Endzustand ist
   * \param q Zustand, dessen Finalit�t gepr�ft wird 
   * \return Gibt true zur�ck gdw. w ein Endzustand ist 
  */
  bool is_final(State q) 
  {
    return states[q].is_final;
  }

  /** 
   * \brief Gibt f�r einen Endzustand das Wort aus, welches auf dem Pfad verarbeitet wurde
   * \param q Zustand, dessen Output gesucht ist 
   * \details Wenn q kein Endzustand ist, wird Epsilon ausgegeben 
  */
  std::string outputfunction(State q){
    if (is_final(q)){return states[q].output;}
    // Epsilon, wenn 'q' kein Endzustand  
    else {return "";}
  }

  
private: // Instanzvariablen
  std::vector<StateDesc>  states;       ///< Zustandsvektor
  std::vector<Transition> transitions;  ///< �bergangsvektor
}; // PatternMatcher

