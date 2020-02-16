# qusbipserver
A usb ip group server

This project is opened to any and all helpers, Merge Requests (MRs) of any amount are considered and only a couple accepted MRs likely are to recieve maintainer access.
This project is the sister server project to qusbip

## Wanted
Anyone willing to help by making MRs to improve the project.

### Future desires
* Platform build docker images for redhat or raspbian.

## Build and Install
## Installing
Go to https://github.com/dicetrash/qusbipserver/releases/tag/0.1 and download the correct package for your hardware


```
sudo apt-get install usbip libudev1 libqt5network5
dpkg -i <file downloaded>
```

### Required development libraries to build
depending on how you want to build it
* qmake / make
* libwrap (tcp-wrappers in arch)
* libusbip (usbip in arch)

or if you want a clean directory and environment
* docker

### Developing
I generally just use QT creator (of which I am of course unaffiliated) to debug the C/C++ side

## Copying and Using
This project uses portions of linux kernel code with heavy modification
I am unaffiliated with basically anyone and also sort of wish to remain anonymous.
If any portion is not in complaince with license rules please just make an MR to fix it.
This project is protected under GNU GPL v2 see the [LICESE](https://github.com/dicetrash/qusbip/blob/master/LICENSE) file in this project for more details.

## Acknowledgement
I would like to thank the [QT team](https://www.qt.io/) for making a nice UI product
and the [USB/IP](http://usbip.sourceforge.net/) team for making the usbip project that this uses
