# Makefile.mk
# Hannah Peuckmann, Matr.-Nr.: 791996, 04.03.2020
# Betriebssystem: Ubuntu 18.4 64 Bit

# Variablendefinition
CPPCOMP = g++

#DEFINES = -DNDEBUG


# Haupt-Target
all:bin/make-patternmatcher  


bin/make-patternmatcher: include/PatternMatcher.hpp src/patternmatcher.cpp doc/doxygen.json
			$(CPPCOMP) -O3 -std=c++11 $(DEFINES) -o bin/make-patternmatcher src/patternmatcher.cpp
			doxypress doc/doxygen.json
			@echo Executable was saved to directory bin/
			@echo Open the doxypress documentation with default browser by typing: xdg-open doc/html/index.html

# Installation
install: bin/make-patternmatcher
	sudo cp bin/make-patternmatcher /usr/local/bin/

 

# Demo ausführen
demo: bin/make-patternmatcher
	@echo demo program is now executed
	@echo a pattern matcher is created of 10000 tokens from the file Suchmuster10000.txt from the data directory
	bin/make-patternmatcher -train data/Suchmuster10000.txt output_data/demo.txt 
	@echo the saved pattern matcher is read and used to search for the tokens of Suchmuster10000.txt in SuchtextBsp_1.txt from the data directory
	@echo it is displayed which tokens it found in each line of the file, displayed in the order of thair appearence in the line
	bin/make-patternmatcher -search data/SuchtextBsp_1.txt output_data/demo.txt
	@echo a pattern matcher is created of 10 tokens from the file Suchmuster10.txt from the data directory
	bin/make-patternmatcher -train -dot data/Suchmuster10.txt output_data/demo.txt output_data/demo10.dot
	dot -Gcharset=latin1 -Tpdf -o output_data/demo10.pdf output_data/demo10.dot
	@echo the created .dot-file is converted to pdf and saved in output_data/
	@echo the saved pattern matcher is read and used to search for the tokens of Suchmuster10.txt in SuchtextBsp_2.txt from the data directory
	@echo it is displayed which tokens it found in each line of the file, displayed in the order of their appearence
	bin/make-patternmatcher -search data/SuchtextBsp_2.txt output_data/demo.txt
	@echo demo finished


testscript:
	./test/testscript.sh


# Dokumentieren 
docu: doc/doxygen.json
	doxypress doc/doxygen.json
	xdg-open doc/html/index.html

# Löschen
clean:
	rm -rf bin/*
	rm -rf doc/html
	rm -rf output_data/*
