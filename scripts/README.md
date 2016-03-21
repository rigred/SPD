# Scripts for reading and writing SPD data on Linux

Make sure the i2c-tools package is installed

    sudo apt-get install i2c-tools

Load the I2C kernel module

    sudo modprobe i2c-dev

Run `i2cdetect -l` to list available I2C/SMBus buses.
Run `i2cdetect -y 0` to scan bus 0. RAM modules should show up at addresses 0x50 to 0x54.

Open `spd_read.sh` and `spd_write.sh` and modify the `ADDRESS` variable for the RAM module SPD you want to read/write. The address starts at 0x50 for the first module up to 0x54 for the forth module.
In `spd_write.sh` comment out the `sleep 0.02` line and uncomment the `i2cset` line.
WARNING: Writing incorrect data to the SPD will prevent your PC from booting!

`spd_read.sh` outputs the SPD data as hex values to stderr, use `./spd_read.sh 2>dump.spd` to save the data to a file.
`spd_write.sh` expects the first argument to be a file with 256 hex values for writing to the RAM SPD - `spd_write.sh dump.spd`.
