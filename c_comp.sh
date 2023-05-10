#!/bin/bash

if test $# -lt 1
then echo "please input file"
fi

output=$(gcc -Wall $1 2>&1)

error_count=`echo $output | grep -E -c "error"`
warning_count=`echo $output | grep -E -c "warning"` 

echo "errors: $error_count"
echo "warnings: $warning_count"