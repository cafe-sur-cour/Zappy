#!/bin/bash

# Script pour générer un résumé visuel du pipeline
# Ce script doit être exécuté depuis le dossier du workspace

echo "==============================================="
echo "=           PIPELINE STATUS SUMMARY           ="
echo "==============================================="

# Ne pas utiliser git pour obtenir le nom de la branche dans Jenkins
# Utilisons plutôt une valeur par défaut ou la variable d'environnement JOB_NAME
if [ -n "$JOB_NAME" ]; then
    # Extract branch name from JOB_NAME (assumes format Branches/branchname/jobname)
    BRANCH_NAME=$(echo "$JOB_NAME" | awk -F'/' '{print $2}')
else
    BRANCH_NAME="main"
fi
echo "BRANCH: $BRANCH_NAME"
echo ""

# Fonction pour afficher le statut avec couleur
function print_status() {
    local status=$1
    local name=$2

    if [ "$status" == "SUCCESS" ]; then
        echo -e "\033[32m✓ $name: $status\033[0m"
    elif [ "$status" == "FAILURE" ]; then
        echo -e "\033[31m✗ $name: $status\033[0m"
    elif [ "$status" == "RUNNING" ]; then
        echo -e "\033[33m⟳ $name: $status\033[0m"
    else
        echo -e "\033[36m? $name: $status\033[0m"
    fi
}

# Vérification du statut de codage
echo "CODING STYLE CHECK:"
if [ -f coding-style-c-reports.log ] && [ ! -s coding-style-c-reports.log ]; then
    print_status "SUCCESS" "C Style"
else
    print_status "FAILURE" "C Style"
fi

if [ -f coding-style-cpp-reports.log ] && [ ! -s coding-style-cpp-reports.log ]; then
    print_status "SUCCESS" "C++ Style"
else
    print_status "FAILURE" "C++ Style"
fi

if [ -f coding-style-python-reports.log ] && [ ! -s coding-style-python-reports.log ]; then
    print_status "SUCCESS" "Python Style"
else
    print_status "FAILURE" "Python Style"
fi

echo ""
echo "BUILD STATUS:"
# Vérification du build
if [ -f zappy_server ] && [ -f zappy_gui ] && [ -f zappy_ai ]; then
    print_status "SUCCESS" "All Components"
else
    if [ -f zappy_server ]; then
        print_status "SUCCESS" "Server"
    else
        print_status "FAILURE" "Server"
    fi

    if [ -f zappy_gui ]; then
        print_status "SUCCESS" "GUI"
    else
        print_status "FAILURE" "GUI"
    fi

    if [ -f zappy_ai ]; then
        print_status "SUCCESS" "AI"
    else
        print_status "FAILURE" "AI"
    fi
fi

echo ""
echo "TESTS STATUS:"
# Vérification des tests
if [ -d coverage ]; then
    TOTAL_TESTS=$(grep -o "Tested: [0-9]*" tests/unit/server/test_results.txt 2>/dev/null | awk '{print $2}' || echo "0")
    PASSING_TESTS=$(grep -o "Passing: [0-9]*" tests/unit/server/test_results.txt 2>/dev/null | awk '{print $2}' || echo "0")
    FAILING_TESTS=$(grep -o "Failing: [0-9]*" tests/unit/server/test_results.txt 2>/dev/null | awk '{print $2}' || echo "0")
    CRASHING_TESTS=$(grep -o "Crashing: [0-9]*" tests/unit/server/test_results.txt 2>/dev/null | awk '{print $2}' || echo "0")

    echo "Total Tests: $TOTAL_TESTS"
    echo "Passing: $PASSING_TESTS"

    if [ "$FAILING_TESTS" -gt "0" ] || [ "$CRASHING_TESTS" -gt "0" ]; then
        print_status "FAILURE" "Some tests failed"
        echo "Failing: $FAILING_TESTS"
        echo "Crashing: $CRASHING_TESTS"
    else
        print_status "SUCCESS" "All tests passed"
    fi
else
    print_status "UNKNOWN" "Tests not run or results not available"
fi

echo ""
echo "==============================================="
echo "=               END OF SUMMARY                ="
echo "==============================================="
