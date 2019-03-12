#!/bin/bash

for event in 100 1000 10000 100000 1000000 10000000; do
	for cpu in 1 2 4 6 8; do
		echo "#events=$event, #cpu=$cpu" | tee -a run.log
		
		# generate test file configuration
		echo "number_of_events = $event" > sample.conf
		if [ "$event" -ne "1" ]; then
			echo "number_of_threads = $cpu" >> sample.conf
		fi
		echo "modules = Module1 Module2 Module3" >> sample.conf

		# run the test
		/usr/bin/time -v ../../build/bin/framework -v sample.conf > tmp 2>> run.log
		tail -n 2 tmp | head -n 1 >> run.log
	done
done

rm -rf sample.conf tmp
