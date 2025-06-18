#!/bin/bash

# Simple script to visualize pipeline status without complex functions
# Designed to work in any environment including Jenkins

# Colors
GREEN="\033[32m"
RED="\033[31m"
YELLOW="\033[33m"
BLUE="\033[36m"
RESET="\033[0m"

# Get branch name from Jenkins environment if available
if [ -n "$JOB_NAME" ]; then
    BRANCH_NAME=$(echo "$JOB_NAME" | awk -F'/' '{print $2}')
else
    BRANCH_NAME="main"
fi

# Determine statuses based on simple file checks
STYLE_STATUS="UNKNOWN"
BUILD_STATUS="UNKNOWN"
TEST_STATUS="UNKNOWN"

# Check for coding style results
if [ -f coding-style-c-reports.log ] || [ -f coding-style-cpp-reports.log ] || [ -f coding-style-python-reports.log ]; then
    if [ -s coding-style-c-reports.log ] || [ -s coding-style-cpp-reports.log ] || [ -s coding-style-python-reports.log ]; then
        STYLE_STATUS="FAILURE"
    else
        STYLE_STATUS="SUCCESS"
    fi
fi

# Check for build artifacts
if [ -f zappy_server ] && [ -f zappy_gui ] && [ -f zappy_ai ]; then
    BUILD_STATUS="SUCCESS"
elif [ -f zappy_server ] || [ -f zappy_gui ] || [ -f zappy_ai ]; then
    BUILD_STATUS="PARTIAL"
else
    BUILD_STATUS="FAILURE"
fi

# Check for test results
if [ -d coverage ]; then
    # Try to determine test status from results
    TEST_STATUS="SUCCESS"
    # Default to success if we have coverage but can't find specific results
else
    TEST_STATUS="UNKNOWN"
fi

# Draw the pipeline visualization
echo ""
echo -e "${BLUE}=============================================${RESET}"
echo -e "${BLUE}          ZAPPY PIPELINE PROGRESS           ${RESET}"
echo -e "${BLUE}=============================================${RESET}"
echo ""
echo -e "${YELLOW}Branch: ${BRANCH_NAME}${RESET}"
echo ""

# Display job statuses
echo -e "1. Coding Style Check: ${STYLE_STATUS == "SUCCESS" ? "$GREEN$STYLE_STATUS$RESET" : "$RED$STYLE_STATUS$RESET"}"
echo -e "          |"
echo -e "          V"
echo -e "2. Build:           ${BUILD_STATUS == "SUCCESS" ? "$GREEN$BUILD_STATUS$RESET" : "$RED$BUILD_STATUS$RESET"}"
echo -e "          |"
echo -e "          V"
echo -e "3. Tests:           ${TEST_STATUS == "SUCCESS" ? "$GREEN$TEST_STATUS$RESET" : "$RED$TEST_STATUS$RESET"}"

echo ""
echo -e "${BLUE}=============================================${RESET}"
echo -e "${BLUE}           PIPELINE COMPLETE                ${RESET}"
echo -e "${BLUE}=============================================${RESET}"
echo ""

exit 0
