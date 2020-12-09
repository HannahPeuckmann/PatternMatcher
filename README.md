# Pattern-Matcher

Das Projekt implementiert einen Pattern-Matcher zur Suche von Schlüsselwörtern in einer Textdatei. Die Funktionalität basiert auf einem endlichen Automaten, der die Schlüsselwörter präfixkomprimiert speichert. Der Automat wird so modifiziert, dass er die Textdatei zeilenweise verarbeitet und die gefundenen Schlüsselwörter je Zeile, der Reihe nach ausgibt.



# Literatur

Die Implementierung beruht auf Aho & Corasick (1975): (https://cr.yp.to/bib/1975/aho.pdf)

# Voraussetzungen (Ubuntu)

* C++-Compiler, z.B. [g++](https://gcc.gnu.org/) 
* [Doxypress](https://www.copperspice.com/docs/doxypress/index.html) für die Dokumentation


# Aufruf

```cmd
$ make
$ make testscript
$ make docu
$ make demo
$ make clean
```

make: Eine ausführbare Datei wird im Ornder bin erstellt, die Datei kann wie in #Nutzung beschrieben ausgeführt werden.
	Außerdem wird ein Demo-Programm erstellt, ebenfalls in bin/ gespeichert und ausgeführt. 

make demo: Erzeugt ein Demo-Programm, speichert dieses in bin/ und führt es aus.

make docu: Mithilfe von Doxypress wird eine HTML-Datei erstellt, zur Sichtung der Dokumentation im Code.

make clean: Alle erstellten Dateien werden wieder gelöscht.

make testscript: Ein Shell-Script läuft ab, welches die Korrektheit des Tries testet, der dem Pattern-Matcher zugrunde liegt mithilfe der in data bereitgestellten Testdaten

# Testdaten

Im Ordner data stehen drei Wortlisten verschiedenen Umfangs zur Nutzung zur verfügung, 10, 100 und 10000 Einträge, entnommen aus dem DeReWo Korpus, eine Grundformenliste mit 250.000 Einträgen von 2011 (https://www1.ids-mannheim.de/kl/projekte/methoden/derewo.html).
Des weiteren stehen im data-Ordner drei Artikel der Berliner Zeitung im txt-Format zur Verfügung, in denen gesucht werden kann. 

# Nutzung


$ -train TOKEN_TEXTFILE TRIE_TEXTFILE: In diesem Modus wird ein Pattern-Matcher erstellt und im txt-Format gespeichert. In diesem Modus 
					 verlangt das Programm nach zwei Argumenten;
					 TOKEN_TEXTFILE: ein Textdokument, dieses muss ISO-88591 enkodiert sein und ein Wort pro Zeile enthalten.
					 TRIE_TEXTFILE: der Name des Textdokuments, in dem der erstellte Pattern-Matcher gespeichert wird.

$ -train -dot TOKEN_TEXTFILE TRIE_TEXTFILE DOT_File: Im Modus -train kann man optional eine Graphviz-dot-Repräsentation des Pattern-Matchers erstellen. 
							DOT_File: ein Dokument mit .dot Endung wird gespeichert.

$ -search SEARCH_TEXTFILE TRIE_TEXTFILE: In diesem Modus wird ein Textdokument nach den Wörtern aus dem im Modus -train erstellten Pattern-Matcher abgesucht. Der Modus 					 verlangt nach zwei Argumenten.
					 SEARCH_TEXTFILE: ein Textdokument, ISO-8859-1 enkodiert. 
					 TRIE_TEXTFILE: ein Textdokument, welches einen im Modus -train erstellten Pattern-Matcher beschreibt.
					 Die Ausgabe der Funde wird im Terminal gemacht.

# Erstellen eines PDF aus dot

Im Modus -train -dot erstellt das Programm einen Pattern-Matcher, speichert diesen ab und erstellt zusätzlich eine dot-Repräsentation des zugrundeliegenden Tries.
Mit folgendem Befehl kann aus der dot-Datei ein PDF erzeugt werden:

```$ dot -Gcharset=latin1 -Tpdf DOT_FILENAME -o PDF_FILENAME``

Die Option -Gcharset=latin1 wird benötigt, da das dot-Dokument ISO-8859-1 kodiert ist.
Achtung! Dieser Vorgang kann einige Zeit in Anspruch nehmen bei großen dot-Dateien.

# Beispiel Nutzung

```cmd
$ make-patternmatcher -train data/derewo100.txt output_data/patternmatcher100.txt
```
Ein Pattern-Matcher wird erstellt und eine Statusmeldung wird ausgegeben : "Pattern-Matching machine was build and saved as /output_data/patternmatcher100.txt

```cmd
$ make-patternmatcher -search data/Falafel.txt output_data/patternmatcher100.txt
```

Ausgabe im Terminal:
```
filename        Zeile   Tokens
data/Falafel.txt       14      dazukommen
```

# Testdaten erstellen

Das Programm readtrie.cpp kann dazu verwendet werden, einen Pattern-Matcher zu erstellen und die im Trie gespeicherten Wörter in eine neue Datei zu schreiben.


```cmd
$ cd src
$ g++ readtrie.cpp -o ../bin/readtrie
$ cd ../bin
$ ./readtrie ../data/derewo10.txt ../output_data/comp_list10.txt

```
Die in /output_data erstellte Datei comp_list10.txt enthält alle Tokens, die im Trie gespeichert sind.

# Dokumentation

Neben dieser README-Datei liegt im Ordner doc eine Doxypress-Dokumentation.

# Bemerkung

Das Projekt entstand im Ramen eines Kurses an der Universität Potsdam als Prüfungsleistung.
Im Projekt sollte nur mit im Kurs behandelten Elementen von C++ gearbeitet werden,
dementsprechend durften keine weiteren Bibliotheken oder Pointer benutzt werden.

# Autorin
Hannah Peuckmann
peuckmann@uni-potsdam.de
Universität Potsdam, Matrikelnummer 791996
