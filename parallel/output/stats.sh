#!/bin/bash

format=*.out
STATE="COMPLETED"
STATE1="RUNNING"
printf '%10s\t' "Dimension" > stats_parallel.txt
printf '%10s\t%15s\t%15s\t%15s\t%10s\t%10s\t%15s\t%35s\t%10s\t%10s\t%10s\t%15s\t%15s\n' "Total time" "Time Intervals" "Learning rate" "Subnets" "Epoch" "Batch size" "Sample size" "Model" "Ans" "Seconds" "Processors" "Mode" "File No">> stats_parallel.txt


for file in $format 
	do
		state=$(echo | awk '/State:/{print $2}' $file)
		echo $state
		if [ "$state" = "$STATE" ] || [ "$state" = "$STATE1" ];
		then
			
			ans=$(echo | grep -B 5 'TCHPC Cluster' $file | grep 'Y0:' | awk -F ':' '{print $3}')
			exeTime=$(echo | awk '/Job Wall-clock/{print $4}' $file)
			dim=$(echo | awk '/Dimension:/{print}' $file | cut -d':' -f2)
			totalTime=$(echo | awk '/Total time:/{print }' $file | cut -d':' -f2)
			NumberofTimeInterval=$(echo | awk '/Number of time intervals:/{print }' $file | cut -d':' -f2)
			learningRate=$(echo | awk '/Learning rate:/{print }' $file | cut -d':' -f2)
			subnets=$(echo | awk '/Number of neurons in each/{print }' $file | cut -d':' -f2)
			epoch=$(echo | awk '/Epochs:/{print }' $file | cut -d':' -f2)
			batch=$(echo | awk '/Batch size:/{print }' $file | cut -d':' -f2)
			sample=$(echo | awk '/Sample size:/{print }' $file | cut -d':' -f2)
			model=$(echo | awk '/Model name:/{print }' $file | cut -d':' -f2)	
			procs=$(echo | awk '/Number of processors:/{print }' $file | cut -d':' -f2)
			mode=$(echo | awk '/Parallel Mode:/{print }' $file | cut -d':' -f2)
			fileNo="${file%.*}"
			seconds=$(echo $exeTime | awk -F: '{ print ($1 * 3600) + ($2 * 60) + $3 }')
			
			printf '%10s\t%10s\t%15s\t%15s\t%15s\t%10s\t%10s\t%15s\t%35s\t%10s\t%10s\t%10s\t%15s\t%15s\n' "$dim" "$totalTime" "$NumberofTimeInterval" "$learningRate" "$subnets" "$epoch" "$batch" "$sample" "$model" "$ans" "$seconds" "$procs" "$mode" "$fileNo">> stats_parallel.txt
	
		else
			continue
		fi
done

