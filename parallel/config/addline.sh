#!/bin/bash

for file in *.txt
do
	echo "parallelMode: Master-Slave" >> "$file"
done
