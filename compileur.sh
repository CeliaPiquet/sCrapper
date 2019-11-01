#!/bin/bash
echo "Compilation..."
gcc ./sources/main.c ./sources/ListLinks.c ./sources/Links.c -lcurl -o executable
echo "Compilation terminée..."
echo "Lancement de l'executable..."
echo "--------------------------------------------------"
./executable
echo -e "\n--------------------------------------------------"
echo "Executable terminée..."

