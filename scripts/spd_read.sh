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
#OUTFILE="dump.spd"

echoErr()
{
	echo "$1" 1>&2
}

#cat /dev/null > ${OUTFILE}

echo "Reading..."
echo "Bus: ${BUS}"
echo "Address: ${ADDRESS}"

SPDDATA=""

for DATAADDR in {0..255}
do
	echo -en "\rReading: ${DATAADDR}/255"
	HEX=$(i2cget -y ${BUS} ${ADDRESS} ${DATAADDR})

	if [ $? -ne 0 ]; then
		echo ""
		echo "Error"
		echo ""
		exit 1
	fi

	SPDDATA="${SPDDATA} ${HEX}"
done

echo ""
echo "Done"
echo ""

echoErr "${SPDDATA}"
#echo -n "${SPDDATA}" | xxd -r -p > ${OUTFILE}
