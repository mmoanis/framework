#!/bin/env bash

if [ ! -f ../bin/framework ]; then
    echo "Framework not found..." >&2
    exit 1;
fi

mkdir test_output

# test same file produce same results
echo "testing simulation with same seeds produce same result..."
for test in ./*.conf; do
    ../bin/framework ./$test > test_output/${test}_1.out 2>&1
    ../bin/framework ./$test > test_output/${test}_2.out 2>&1

    if cmp -s test_output/${test}_1.out test_output/${test}_2.out ; then
        echo "passed ${test}"
    else
        echo "failed ${test}"

        rm -rf test_output
        exit 1;
    fi
done


rm -rf test_output
