# Pattern-Matcher

The Project implements a Pattern-Matcher to search for keywords in text data. The implementation relies on a finite state machine that saves the keywords prefx compressed. The finite state machine is modified in a way that dass text data is processed line by line and the keywords that were found are displayed.



# Literature
The implementation relies on Aho & Corasick (1975): (https://cr.yp.to/bib/1975/aho.pdf)

# Requirements (Ubuntu)

* C++-Compiler, e.g. [g++](https://gcc.gnu.org/) 
* [Doxypress](https://www.copperspice.com/docs/doxypress/index.html) für die Dokumentation


# Remarks

The project was made 
The project was developed as part of a course at the University of Potsdam as an examination.
The project should only work with elements of C++ covered in the course,
hence, no other libraries or pointers were allowed to be used..

# Execution

```cmd
$ make
$ make testscript
$ make docu
$ make demo
$ make clean
```

make: an executable is created in the bin folder. 

make demo: creates a demo-programm, saved in bin/ and runs it.

make docu: doxypress html file is created

make clean: All files created are wiped.

make testscript:a shell script that proofs the correctness of the Trie.

# Test data

In the data folder there are three word lists of different size available for use, 10, 100 and 10000 entries, taken from the DeReWo corpus, a basic form list with 250,000 entries from 2011 (https://www1.ids-mannheim.de/kl/projekte/methoden/derewo.html).
Furthermore, three articles of the Berliner Zeitung are available in the data folder in txt format, which can be searched.

# Usage


$ -train TOKEN_TEXTFILE TRIE_TEXTFILE: In this mode a pattern matcher is created and saved in txt format. In this mode 
					 the program asks for two arguments;
					 TOKEN_TEXTFILE: text file, ISO-88591 encoded, one word per line.
					 TRIE_TEXTFILE: nam eof the file the pattern matcher should be saved in.

$ -train -dot TOKEN_TEXTFILE TRIE_TEXTFILE DOT_File: In this mode -train an optional Graphviz-dot-representation of the Trie can be created. 
							DOT_File: document with .dot type.

$ -search SEARCH_TEXTFILE TRIE_TEXTFILE: A text document is searched for the words of the pattern matcher abgesucht.
					 SEARCH_TEXTFILE: text file, ISO-88591 encoded 
					 TRIE_TEXTFILE:text file that holds th ein the -train step created pattern matcher.
					 The output is displayed in the terminal

# Create a .pdf out of .dot

In the mode -train -dot the programm creates a pattern matcher, saves it and in addition creates a dot representation of the Trie.
The following command creates a pdf out of the dot:

```$ dot -Gcharset=latin1 -Tpdf DOT_FILENAME -o PDF_FILENAME``
The option -Gcharset=latin1 is nessecary because of the ISO-8859-1 encoding.
Be carfull the dot file can be very large.

# Example

```cmd
$ make-patternmatcher -train data/derewo100.txt output_data/patternmatcher100.txt
```
A pattern matcher was build and a status message is displayed : "Pattern-Matching machine was build and saved as /output_data/patternmatcher100.txt

```cmd
$ make-patternmatcher -search data/Falafel.txt output_data/patternmatcher100.txt
```

Output in the terminal:
```
filename        Zeile   Tokens
data/Falafel.txt       14      dazukommen
```

# Create testdata

the programm readtrie.cpp can be used to create a pattern matcher and save the words of the Trie in a text file.


```cmd
$ cd src
$ g++ readtrie.cpp -o ../bin/readtrie
$ cd ../bin
$ ./readtrie ../data/derewo10.txt ../output_data/comp_list10.txt

```
The file comp_list10.txt in /output_data holds all the token that are saved in the Trie

# Dokumentation

Besides this README there is a Doxypress-Dokumentation in the doc folder.


# Author
Hannah Peuckmann
peuckmann@uni-potsdam.de
Universität Potsdam, Matrikelnummer 791996
