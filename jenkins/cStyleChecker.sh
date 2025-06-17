#!/bin/bash

check.sh $(pwd) $(pwd)
grep -v "./documentation" coding-style-reports.log > new-coding.log
cat new-coding.log
