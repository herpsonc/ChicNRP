#!/bin/sh

echo "Entrer le nom du fichier au format ods (sans l'extension)"
read FILENAME
cp $FILENAME.ods copie.ods
mv copie.ods copie.zip
unzip copie.zip content.xml
g++ -o entree entree.cpp -I/usr/include/libxml2/ -lxml2
./entree "content.xml"
rm copie.zip
rm content.xml