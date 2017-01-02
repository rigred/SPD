# SPD
Hacking DDR Memory Serial Presence Detect

This project aims to create an 'open source'\libre (linux) editor for DDR memory chips Serial Presence Detect Data (SPD)
SPD is present on virtually all modern DDR memory modules and serves to define the operating paramters of the memory module to the computer firmware.
SPD devices can be accessed via the systems built in i2c bus.
Usually i2c bus 0 on any system.
RAM modules are most often located at SPD bus addresses 0x50, 0x52, 0x54 etc.

An easy way to access SPD devices on any linux system is to use the i2c-tools

```bash
sudo su
apt install -y i2c-tools (for debian/ubuntu based)
modprobe i2c-dev 
## On some systems i2c-i801 is needed
modprobe i2c-i801

## Use i2cdetect to find the i2c bus
i2cdetect -lt
## Output
i2c-3   unknown         NVIDIA i2c adapter 6 at 1:00.0          N/A
i2c-1   unknown         NVIDIA i2c adapter 1 at 1:00.0          N/A
i2c-4   unknown         NVIDIA i2c adapter 8 at 1:00.0          N/A
i2c-2   unknown         NVIDIA i2c adapter 2 at 1:00.0          N/A
i2c-0   unknown         NVIDIA i2c adapter 0 at 1:00.0          N/A
i2c-5   unknown         SMBus I801 adapter at 0400              N/A
## In this case the i2c SPD is not actually bus 0, but on bus 5, note that the i801 module was loaded
this is a common issue with systems with Nvidia graphics cards or chipsets.

## A successful dump should look a little like this. (In this case a rubbish DDR3-1333 (PC3-10600) RAM module.
sudo i2cdump 5 0x50                                                                                                                                                                                                           
No size specified (using byte-data access)                                                                                                                                                                                                   
WARNING! This program can confuse your I2C bus, cause data loss and worse!                                                                                                                                                                   
I will probe file /dev/i2c-5, address 0x50, mode byte                                                                                                                                                                                        
Continue? [Y/n] y                                                                                                                                                                                                                            
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
00: 92 10 0b 02 03 19 00 09 03 52 01 08 0c 00 3c 00    ??????.??R???.<.                                                                                                                                                                      
10: 69 78 69 30 69 11 20 89 00 05 3c 3c 00 f0 83 01    ixi0i? ?.?<<.???                                                                                                                                                                      
20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................                                                                                                                                                                      
30: 00 00 00 00 00 00 00 00 00 00 00 00 0f 11 01 01    ............????                                                                                                                                                                      
40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
70: 00 00 00 00 00 85 02 02 00 00 00 00 00 00 fd ba    .....???......??
80: 50 53 44 33 34 47 31 33 33 33 32 00 00 00 00 00    PSD34G13332.....
90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................



```

## More to come


Part of this involves:

1. Creating an xml structure definition for okteta to better work with binary SPD dumps.
2. Creating a cli tool for reading, writing and editing SPD data.
