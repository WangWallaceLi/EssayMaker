#!/bin/bash
for i in {0..199}
do
 wget http://storage.googleapis.com/books/ngrams/books/googlebooks-eng-1M-3gram-20090715-$i.csv.zip
 unzip googlebooks-eng-1M-2gram-20090715-$i.csv.zip 
done 