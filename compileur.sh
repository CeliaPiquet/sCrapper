#!/bin/bash
echo "Compilation..."
gcc main.c -lcurl -o executable
echo "Compilation terminée..."
echo "Lancement de l'executable..."
echo "--------------------------------------------------"
./executable
echo "--------------------------------------------------"
echo "Executable terminée..."

