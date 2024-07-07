#!/bin/bash

# Create a single virtual port for the master
MASTER_PORT="/dev/virtual-ttyMaster"

# Create a main socat process to listen on the master port and forward data
sudo socat -d -d PTY,link=$MASTER_PORT,raw,echo=0 &

# Sleep to allow the master port to be created
sleep 1

# Array of slave ports
SLAVE_PORTS=("/dev/virtual-ttyS1" "/dev/virtual-ttyS2" "/dev/virtual-ttyS3")

# Create socat processes to route communication between master and slaves
for SLAVE_PORT in "${SLAVE_PORTS[@]}"
do
    sudo socat -d -d PTY,link=${SLAVE_PORT} PTY,link=${MASTER_PORT},raw,echo=0 &
done

# Wait for all socat processes to finish
wait
