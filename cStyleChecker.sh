#!/bin/bash

make fclean
coding-style . .
grep -v "./documentation" coding-style-reports.log > coding-style-reports.log
cat coding-style-reports.log
