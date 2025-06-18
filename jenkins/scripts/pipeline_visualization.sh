#!/bin/bash

# Script pour générer une visualisation graphique du pipeline
# Affiche un diagramme ASCII montrant la progression du pipeline

# Couleurs
GREEN="\033[32m"
RED="\033[31m"
YELLOW="\033[33m"
BLUE="\033[36m"
RESET="\033[0m"

# Ne pas utiliser git pour obtenir le nom de la branche dans Jenkins
# Utilisons plutôt une valeur par défaut ou la variable d'environnement JOB_NAME
if [ -n "$JOB_NAME" ]; then
    # Extract branch name from JOB_NAME (assumes format Branches/branchname/jobname)
    BRANCH_NAME=$(echo "$JOB_NAME" | awk -F'/' '{print $2}')
else
    BRANCH_NAME="main"
fi

# Fonction pour afficher une boîte de status
draw_status_box() {
    local name=$1
    local status=$2
    local width=20
    local padding=$(( (width - ${#name}) / 2 ))
    local left_padding=$padding
    local right_padding=$padding

    if [ $(( ${#name} % 2 )) -ne 0 ]; then
        right_padding=$((right_padding + 1))
    fi

    # Couleur en fonction du statut
    local color=$BLUE
    if [ "$status" == "SUCCESS" ]; then
        color=$GREEN
    elif [ "$status" == "FAILURE" ]; then
        color=$RED
    elif [ "$status" == "RUNNING" ]; then
        color=$YELLOW
    fi

    echo -e "${color}┌$("printf '─%.0s' $(seq 1 $width)")┐${RESET}"
    echo -e "${color}│$("printf ' %.0s' $(seq 1 $left_padding)")${name}$("printf ' %.0s' $(seq 1 $right_padding))│${RESET}"
    echo -e "${color}│$("printf ' %.0s' $(seq 1 $width))│${RESET}"
    echo -e "${color}│$("printf ' %.0s' $(seq 1 $(( (width - ${#status}) / 2 )) ))${status}$("printf ' %.0s' $(seq 1 $(( width - ${#status} - $(( (width - ${#status}) / 2 )) )) ))│${RESET}"
    echo -e "${color}└$("printf '─%.0s' $(seq 1 $width)")┘${RESET}"
}

# Fonction pour dessiner une flèche
draw_arrow() {
    echo -e "         ${BLUE}│${RESET}"
    echo -e "         ${BLUE}▼${RESET}"
}

# Déterminer les statuts par défaut
STYLE_STATUS="UNKNOWN"
BUILD_STATUS="UNKNOWN"
TEST_STATUS="UNKNOWN"

# Check coding style status
if [ -f coding-style-c-reports.log ] || [ -f coding-style-cpp-reports.log ] || [ -f coding-style-python-reports.log ]; then
    if [ -s coding-style-c-reports.log ] || [ -s coding-style-cpp-reports.log ] || [ -s coding-style-python-reports.log ]; then
        STYLE_STATUS="FAILURE"
    else
        STYLE_STATUS="SUCCESS"
    fi
fi

# Check build status
if [ -f zappy_server ] && [ -f zappy_gui ] && [ -f zappy_ai ]; then
    BUILD_STATUS="SUCCESS"
elif [ -f zappy_server ] || [ -f zappy_gui ] || [ -f zappy_ai ]; then
    BUILD_STATUS="PARTIAL"
else
    BUILD_STATUS="FAILURE"
fi

# Check test status
if [ -d coverage ]; then
    FAILING_TESTS=$(grep -o "Failing: [0-9]*" tests/unit/server/test_results.txt 2>/dev/null | awk '{print $2}')
    CRASHING_TESTS=$(grep -o "Crashing: [0-9]*" tests/unit/server/test_results.txt 2>/dev/null | awk '{print $2}')

    if [ -z "$FAILING_TESTS" ] && [ -z "$CRASHING_TESTS" ]; then
        TEST_STATUS="SUCCESS"
    elif [ "$FAILING_TESTS" = "0" ] && [ "$CRASHING_TESTS" = "0" ]; then
        TEST_STATUS="SUCCESS"
    else
        TEST_STATUS="FAILURE"
    fi
fi

# Titre
echo ""
echo -e "${BLUE}╔════════════════════════════════════════════════════╗${RESET}"
echo -e "${BLUE}║              ZAPPY PIPELINE PROGRESS               ║${RESET}"
echo -e "${BLUE}╚════════════════════════════════════════════════════╝${RESET}"
echo ""
echo -e "${YELLOW}Branch: ${BRANCH_NAME}${RESET}"
echo ""

# Diagramme
draw_status_box "1. Coding Style" "$STYLE_STATUS"
draw_arrow
draw_status_box "2. Build" "$BUILD_STATUS"
draw_arrow
draw_status_box "3. Tests" "$TEST_STATUS"
echo ""

# Footer
echo -e "${BLUE}╔════════════════════════════════════════════════════╗${RESET}"
echo -e "${BLUE}║                 PIPELINE COMPLETE                  ║${RESET}"
echo -e "${BLUE}╚════════════════════════════════════════════════════╝${RESET}"
echo ""

exit 0

# Fonction pour afficher une boîte de status
function draw_status_box() {
    local name=$1
    local status=$2
    local width=20
    local padding=$(( (width - ${#name}) / 2 ))
    local left_padding=$padding
    local right_padding=$padding

    if [ $(( ${#name} % 2 )) -ne 0 ]; then
        right_padding=$((right_padding + 1))
    fi

    # Couleur en fonction du statut
    if [ "$status" == "SUCCESS" ]; then
        color=$GREEN
    elif [ "$status" == "FAILURE" ]; then
        color=$RED
    elif [ "$status" == "RUNNING" ]; then
        color=$YELLOW
    else
        color=$BLUE
    fi

    echo -e "${color}┌$("printf '─%.0s' $(seq 1 $width)")┐${RESET}"
    echo -e "${color}│$("printf ' %.0s' $(seq 1 $left_padding)")${name}$("printf ' %.0s' $(seq 1 $right_padding))│${RESET}"
    echo -e "${color}│$("printf ' %.0s' $(seq 1 $width))│${RESET}"
    echo -e "${color}│$("printf ' %.0s' $(seq 1 $(( (width - ${#status}) / 2 )) ))${status}$("printf ' %.0s' $(seq 1 $(( width - ${#status} - $(( (width - ${#status}) / 2 )) )) ))│${RESET}"
    echo -e "${color}└$("printf '─%.0s' $(seq 1 $width)")┘${RESET}"
}

# Fonction pour dessiner une flèche
function draw_arrow() {
    echo -e "         ${BLUE}│${RESET}"
    echo -e "         ${BLUE}▼${RESET}"
}

# Déterminer les statuts (simulé pour cet exemple)
# Dans un environnement réel, vous récupéreriez ces informations depuis Jenkins

# Check if coding style files exist
STYLE_STATUS="UNKNOWN"
if [ -f coding-style-c-reports.log ] || [ -f coding-style-cpp-reports.log ] || [ -f coding-style-python-reports.log ]; then
    if [ -s coding-style-c-reports.log ] || [ -s coding-style-cpp-reports.log ] || [ -s coding-style-python-reports.log ]; then
        STYLE_STATUS="FAILURE"
    else
        STYLE_STATUS="SUCCESS"
    fi
fi

# Check if executables exist for build status
BUILD_STATUS="UNKNOWN"
if [ -f zappy_server ] && [ -f zappy_gui ] && [ -f zappy_ai ]; then
    BUILD_STATUS="SUCCESS"
elif [ -f zappy_server ] || [ -f zappy_gui ] || [ -f zappy_ai ]; then
    BUILD_STATUS="PARTIAL"
else
    BUILD_STATUS="FAILURE"
fi

# Check test results
TEST_STATUS="UNKNOWN"
if [ -d coverage ]; then
    FAILING_TESTS=$(grep -o "Failing: [0-9]*" tests/unit/server/test_results.txt 2>/dev/null | awk '{print $2}')
    CRASHING_TESTS=$(grep -o "Crashing: [0-9]*" tests/unit/server/test_results.txt 2>/dev/null | awk '{print $2}')

    if [ -z "$FAILING_TESTS" ] && [ -z "$CRASHING_TESTS" ]; then
        TEST_STATUS="SUCCESS"
    elif [ "$FAILING_TESTS" = "0" ] && [ "$CRASHING_TESTS" = "0" ]; then
        TEST_STATUS="SUCCESS"
    else
        TEST_STATUS="FAILURE"
    fi
fi

# Titre
echo ""
echo -e "${BLUE}╔════════════════════════════════════════════════════╗${RESET}"
echo -e "${BLUE}║              ZAPPY PIPELINE PROGRESS               ║${RESET}"
echo -e "${BLUE}╚════════════════════════════════════════════════════╝${RESET}"
echo ""
echo -e "${YELLOW}Branch: ${BRANCH_NAME}${RESET}"
echo ""

# Diagramme
draw_status_box "1. Coding Style" "$STYLE_STATUS"
draw_arrow
draw_status_box "2. Build" "$BUILD_STATUS"
draw_arrow
draw_status_box "3. Tests" "$TEST_STATUS"
echo ""

# Footer
echo -e "${BLUE}╔════════════════════════════════════════════════════╗${RESET}"
echo -e "${BLUE}║                 PIPELINE COMPLETE                  ║${RESET}"
echo -e "${BLUE}╚════════════════════════════════════════════════════╝${RESET}"
echo ""
