#!/bin/bash

# Script pour exécuter les tests de couverture et générer un rapport même si certains tests échouent
# Ce script est conçu pour fonctionner dans l'environnement Jenkins

set -e

echo "Starting test coverage run with workarounds..."

# Créer le répertoire de couverture s'il n'existe pas
mkdir -p coverage

# Exécuter les tests unitaires du serveur et capturer la sortie
echo "Running server unit tests..."
cd tests/unit/server || exit 1
make || true  # Continue même si le build échoue

# Exécuter les tests avec valgrind pour détecter les fuites de mémoire
echo "Running memory leak detection..."
valgrind --leak-check=full --error-exitcode=0 --xml=yes --xml-file=valgrind_report.xml ./unit_tests || true

# Exécuter les tests et générer le rapport de couverture même si les tests échouent
./unit_tests > test_results.txt 2>&1 || true
gcovr --xml-pretty --exclude-unreachable-branches --print-summary -o coverage_report.xml -r . || true
gcovr --html --html-details --exclude-unreachable-branches -o coverage_report.html -r . || true

# Copier les rapports de couverture dans le répertoire principal
mkdir -p ../../../coverage/server
cp coverage_report.* ../../../coverage/server/
cp test_results.txt ../../../coverage/server/
cp valgrind_report.xml ../../../coverage/server/ 2>/dev/null || true

cd ../../..

# Tests unitaires de l'interface graphique
echo "Running GUI unit tests..."
cd tests/unit/gui || exit 1
make || true

# Exécuter les tests et générer le rapport de couverture
./unit_tests > test_results.txt 2>&1 || true
gcovr --xml-pretty --exclude-unreachable-branches --print-summary -o coverage_report.xml -r . || true
gcovr --html --html-details --exclude-unreachable-branches -o coverage_report.html -r . || true

# Copier les rapports
mkdir -p ../../../coverage/gui
cp coverage_report.* ../../../coverage/gui/
cp test_results.txt ../../../coverage/gui/

cd ../../..

# Tests unitaires de l'IA
echo "Running AI unit tests..."
cd tests/unit/ai || exit 1
python3 -m unittest discover -s . -p "test_*.py" > test_results.txt 2>&1 || true

# Générer le rapport de couverture Python
python3 -m coverage run -m unittest discover -s . -p "test_*.py" || true
python3 -m coverage xml -o coverage_report.xml || true
python3 -m coverage html -d coverage_html || true

# Copier les rapports
mkdir -p ../../../coverage/ai
cp coverage_report.xml ../../../coverage/ai/ 2>/dev/null || true
cp -r coverage_html ../../../coverage/ai/ 2>/dev/null || true
cp test_results.txt ../../../coverage/ai/

cd ../../..

echo "Coverage reports generated in the coverage/ directory"
echo "Test run completed with workarounds"

exit 0
