#!/bin/env bash

if [ ! -f ../bin/framework ]; then
    echo "Framework not found..." >&2
    exit 1;
fi

DIR=$(dirname "$0")

mkdir test_output

# test same file produce same results
echo "testing simulation with same seeds produce same result..."
for test in $DIR/same_seed/*.conf; do
    ../bin/framework $test > test_output/same_seed_$(basename $test)_1.out 2>&1
    ../bin/framework $test > test_output/same_seed_$(basename $test)_2.out 2>&1

    if cmp -s test_output/same_seed_$(basename $test)_1.out test_output/same_seed_$(basename $test)_2.out ; then
        echo "passed ${test}"
    else
        echo "failed ${test}" >&2

        rm -rf test_output
        exit 1;
    fi
done

# test same file produce different results
echo "testing simulation with random seeds produce different result..."
for test in $DIR/*.conf; do
    ../bin/framework $test > test_output/$(basename $test)_1.out 2>&1
    sleep 1
    ../bin/framework $test > test_output/$(basename $test)_2.out 2>&1

    if cmp -s test_output/$(basename $test)_1.out test_output/$(basename $test)_2.out ; then
        echo "failed ${test}" >&2

        rm -rf test_output
        exit 1;
    else
        echo "passed ${test}"
    fi
done

rm -rf test_output
