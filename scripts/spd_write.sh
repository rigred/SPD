#!/bin/bash

# Project: Modifying RAM SPD Data
# Author: Zak Kemble, contact@zakkemble.co.uk
# Copyright: (C) 2016 by Zak Kemble
# Web: http://blog.zakkemble.co.uk/modifying-ram-spd-data/

# Licence:
# Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
# http://creativecommons.org/licenses/by-nc-sa/4.0/

ADDRESS=0x50 # RAM module address, usually 0x50 - 0x54
BUS=0 # I2C bus number, usually 0
INFILE="$1"

if [ ! -f "${INFILE}" ]; then
	echo "Input file not found!"
	exit 1
fi

echo "Writing..."
echo "Bus: ${BUS}"
echo "Address: ${ADDRESS}"

#SPDDATA=($(od -An -vtx1 ${INFILE} | tr -d '\n'))
SPDDATA=($(cat ${INFILE} | tr -d '\n'))

SPDDATALEN=${#SPDDATA[@]}

if [ ${SPDDATALEN} != 256 ]; then
	echo "Invalid SPD data length (${SPDDATALEN}), should be 256"
	exit 1
fi

for DATAADDR in {0..255}
do
	echo -en "\rWriting: ${DATAADDR}/255 (${SPDDATA[${DATAADDR}]})"

	echo -en " WARNING open this script, comment the sleep line and uncomment the i2cset line at your own risk! (No changes have been made)"
	#sleep 0.02
	$(i2cset -y ${BUS} ${ADDRESS} ${DATAADDR} ${SPDDATA[${DATAADDR}]})

	if [ $? -ne 0 ]; then
		echo ""
		echo "Error"
		echo ""
		exit 1
	fi
done

echo ""
echo "Done"
