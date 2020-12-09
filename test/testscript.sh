#!/bin/bash



# Bash script to test wether all tokens of a file where saved in a pattern matcher


echo "It is tested, wether all tokens of a file were saved correctly in the pattern matcher"
echo "A pattern matcher is created of a token file,"
echo "Each token of the file is written to a new file if it is found in the created pattern matcher."
echo "The original file and the new created file are compared by $diff."
echo "This test is run for the example token files in the /data directory."


cp data/Suchmuster10.txt test/
cp data/Suchmuster100.txt test/
cp data/Suchmuster10000.txt test/

g++ src/readtrie.cpp -o  readtrie
./readtrie test/Suchmuster10.txt test/compare1.txt 
./readtrie test/Suchmuster100.txt test/compare2.txt 
./readtrie test/Suchmuster10000.txt test/compare3.txt 

### saved, save
echo "Suchmuster10.txt is tested."
if diff -q  test/Suchmuster10.txt  test/compare1.txt; then
	echo "The pattern matcher saved all tokens correctly."
else echo "The pattern matcher did not save all tokens correctly."
fi

echo "Suchmuster100.txt is tested."
if diff -q test/Suchmuster100.txt test/compare2.txt; then
        echo "The pattern matcher saved all tokens correctly."

else echo "The pattern matcher did not save all tokens correctly."
fi

echo "Suchmuster10000.txt is tested."
if diff test/Suchmuster10000.txt test/compare3.txt; then
        echo "The pattern matcher saved all tokens correctly."

else echo "The pattern matcher did not save all tokens correctly."
fi

rm test/Suchmuster10.txt
rm test/Suchmuster100.txt
rm test/Suchmuster10000.txt
rm test/compare1.txt
rm test/compare2.txt
rm test/compare3.txt
rm readtrie
echo "Test finished, all created files are deleted again."
