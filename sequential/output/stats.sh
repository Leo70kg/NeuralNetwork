#!/bin/bash

format=*.out
STATE="COMPLETED"
STATE1="RUNNING"
printf '%10s\t' "Dimension" > stats.txt
printf '%10s\t%15s\t%15s\t%15s\t%10s\t%10s\t%15s\t%30s\t%10s\t%10s\t%15s\n' "Total time" "Time Intervals" "Learning rate" "Subnets" "Epoch" "Batch size" "Sample size" "Model" "Ans" "Seconds" "File No">> stats.txt


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
			fileNo="${file%.*}"
			seconds=$(echo $exeTime | awk -F: '{ print ($1 * 3600) + ($2 * 60) + $3 }')
			
			printf '%10s\t%10s\t%15s\t%15s\t%15s\t%10s\t%10s\t%15s\t%30s\t%10s\t%10s\t%15s\n' "$dim" "$totalTime" "$NumberofTimeInterval" "$learningRate" "$subnets" "$epoch" "$batch" "$sample" "$model" "$ans" "$seconds" "$fileNo">> stats.txt
	
		else
			continue
		fi
done

