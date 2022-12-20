# Domesday Duplicator (With Docs Version)

Additional Images and documentaion by Harry Munday.

![DdD-Trasparent-Harrypm](https://user-images.githubusercontent.com/56382624/183899501-a1914f3f-0710-4095-8f0e-b84e6d266d1c.png)

## Manufacturing and Costs

#### The Domesday Duplicator is 3 Boards

DdD ADC (RF Input, Amplification & Filtering)

DE0-Nano FPGA (Data Buffer)

FX3 USB 3.0 (Computer Interface)

### Parts Avaliblity Status

FX3 SuperSpeed Explorer Kit Status - Avalible at orignal MSRP rates. (30-50GBP)

DE0-Nano FFPGA Status - Shortages 100-200% Inflation. (100-140GBP) (MSRP 70-80GBP)

Due to inflation and shortages (2022) prices vary too much to recommend any spesific vendors in the USA/UK/Europe or Asia currently.

There is normally a 2-4 week manufacturing time but due to shipping costs its cheaper on a single unit basis to use a frabrication service due to mulit parts order venders total shipping cost makes it impractical unless making 5-20+ units this cost is around 120-150USD per board depending on provider with the example company being PCBWay.

## Required Files for Fabraciation!

The Gerber files this .zip file containes the specifications of the bare board and ware its taces/pads go witch is 2 layers.

The BOM or Bill Of Materials is the parts to be aquired and soldered to the pads/holes on the board, this is in the Microsoft .XLSX spredsheet format.

The POS File or Centroid file is the position data for though hole parts i,e positions to drill holes in the baord.

All these files are clearly layed out in this repository allowing drag and drop fabricaiton.

## Firmware & Flashing

[Harrypm's AIO windows Setup Package](https://docs.google.com/document/d/1j9QM5dFXIgRhrnFb7q1N1S557zTegOVq0JKApvoDmdE/edit?usp=sharing) (User Friendly)

[DD86 Docs](https://www.domesday86.com/?page_id=1070) (Tehcnicals & Linux Based Setup)

## Capture & Control Software Installation

[Windows Build](https://github.com/TokugawaHeavyIndustries/DomesdayDuplicator-WinBuild) (self-contained usb driver install needed)

To compile the software on Ubuntu, you will need Qt5 installed as well as the necessary support packages

Dependencys Install: Old

    sudo apt install git g++ qtbase5-dev qt5-default qt5-qmake libqt5serialport5-dev libusb-1.0-0-dev

Note: For later Ubuntu versions such as 22.04 you may need libqt5-dev and libgl-dev.

Dependencys Install: Ubuntu 22.04:

    sudo apt install --no-install-recommends git cmake libgl-dev qt6-base-dev libqt6serialport6-dev libusb-1.0-0-dev qt6-multimedia-dev build-essential

Applicaton Install:

    git clone https://github.com/simoninns/DomesdayDuplicator

Once the software is downloaded, simply issue the following commands to compile the application:

    cd DomesdayDuplicator/Linux-Application/DomesdayDuplicator
    qmake
    make all
    sudo make install

This will allow you to run the application using the following command:

    ~/DomesdayDuplicator/Linux-Application/DomesdayDuplicator/DomesdayDuplicator &

And should be avalible for quick lanuch inside Linux Mint.

In addition, it is recommended to also compile and install the two utility applications dddconv and dddutil:

    cd DomesdayDuplicator/Linux-Application/dddconv
    qmake
    make
    sudo make install

&

    cd DomesdayDuplicator/Linux-Application/dddutil
    qmake
    make all
    sudo make install

## Building the software with CMake

The DomesdayDuplicator application and tools can also be built using CMake, using either Qt 5 or Qt 6. To build on Ubuntu 22.04 using Qt 6, you should first install the dependencies:

    sudo apt install --no-install-recommends cmake libgl-dev qt6-base-dev libqt6serialport6-dev libusb-1.0-0-dev

The application and tools can then be configured and built like this:

     cd DomesdayDuplicator/Linux-Application
     cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo .
     make -j8
     sudo make install

By default CMake will install into /usr/local.

To install into /some/dir instead, add -DCMAKE_INSTALL_PREFIX=/some/dir to the cmake command.

## USB Device Permissions

In order for the GUI application to connect to the Domesday Duplicator USB device it is necessary to provide permissions for user-level access. For this you need to create a file called 40-domesdayduplicator.rules in the /etc/udev/rules.d directory (requires root permissions). The contents of the file should be as follows:

    # 1d50:603b - Domesday Duplicator
    SUBSYSTEM=="usb", ATTRS{idVendor}=="1d50", ATTRS{idProduct}=="603b", MODE="0666"
    Once the file has been created, issue the following command to reload the USB configuration rules:

    sudo udevadm control --reload-rules

Serial port device permissions
If you wish to use the automatic capture feature a serial connection between the Ubuntu PC and the laser video disc player is required (you will need an appropriate cable and USB to RS232 adapter). In order to access a serial device the Ubuntu user account must given the 'dialout' group permissions. This can be added to the current user account using the following command:

    sudo usermod -a -G dialout $USER

Note that this permission does not become active until the user account has logged out and logged back in again.

## Serial port device permissions
If you wish to use the automatic capture feature a serial connection between the Ubuntu PC and the laser video disc player is required (you will need an appropriate cable and USB to RS232 adapter). In order to access a serial device the Ubuntu user account must given the 'dialout' group permissions. This can be added to the current user account using the following command:

    sudo usermod -a -G dialout $USER

Note that this permission does not become active until the user account has logged out and logged back in again.

## Software Layout & Use

(Same in both Windows & Linux)

![DdD-Main-Linux-No-DdD](https://user-images.githubusercontent.com/56382624/183902999-e3852084-c193-4fab-9099-eec9710650ed.png)

![DdD-Main-Linux-DdD-Connected](https://user-images.githubusercontent.com/56382624/183903020-f71bbc63-6a0b-4262-917f-58093017e4c3.png)

As you can see over 11 hours of capture time avalible  on a fully setup Linux Mint intall with a 2TB M.2 NVME drive.

![Advanced-Capture-Naming](https://user-images.githubusercontent.com/56382624/183903089-4cde72ba-fa5b-4d55-9aeb-efc186f5b9e8.png)

![Preferences-Capture](https://user-images.githubusercontent.com/56382624/183903116-f3fcf2b8-57ec-465f-bc08-6b07ac61b11c.png)

## LazerDisk Player Control Fetures (Requires Seirial Connection)

![Preferences-Player-Control](https://user-images.githubusercontent.com/56382624/183903368-51eb54cf-69cc-4884-86bd-43ae5870097b.png)

![Preferences-USB](https://user-images.githubusercontent.com/56382624/183903445-1b324b0d-13e7-4f77-93ec-9b645c50cfae.png)

![Automatic-Capture](https://user-images.githubusercontent.com/56382624/183903273-faa4f86c-57c1-40de-966f-89d2121a2293.png)

![Player-Remote](https://user-images.githubusercontent.com/56382624/183903323-28b51b11-d5f3-4a76-87be-8b19d1ec30a7.png)

## Synopsis

The Domesday Duplicator is a USB3 based DAQ capable of 40 million samples per second acquisition of analogue RF data.

The hardware is a USB3 based 10-bit analogue to digital converter designed to allow the backup of Domesday AIV LaserDiscs (as well as generic laserdiscs and now Tape/CD media) through the direct sampling of the RF data from the optical head (laser) of a LaserDisc player.

The hardware/software solution is designed to act as a sampling front-end to the ld-decode (software decode of laserdiscs) project https://github.com/happycube/ld-decode and replaces the generic TV capture card to provide high-frequency sampling with 4 times the sample resolution.

This project is currently work-in-progress and should be considered beta (until the decoding is proven modifications may be needed to the capture set-up).

Please see http://www.domesday86.com/?page_id=978 for detailed documentation on the Domesday Duplicator and overall information about the Domesday86 project.

There are 3 main components that make up the Domesday Duplicator:

A custom ADC board based around the Texas Instruments ADS825 10-Bit, 40MSPS analogue to digital converter.  This board contains an RF amplifier and conditioner (to amplify the output from the laserdisc player RF tap and condition it for the single-ended ADC chip) as well as headers for both the DE0-Nano FPGA development board and Cypress FX3 SuperSpeed Explorer board.  RF input is physically provided by a BNC connector.

Terasic DE0-Nano FPGA development board - The DE0-Nano is a low-cost FPGA development board containing an Altera Cyclone IV FPGA.  The DE0-Nano is used to process the raw 10-bit ADC data stream and provides FIFO buffering (towards the FX3) and sample conversion from unsigned 10-bit to scaled 16-bit signed data.  The FPGA provides a dual-clock FIFO; receiving ADC data at a maximum of 40 MSPS (million samples per second) and transmitting the data to the FX3 at a maximum of 60 MSPS.  This is to ensure that no data is lost during sustained transfers (as RF sampling of a disc can take more than 40 minutes).

Cypress FX3 SuperSpeed Explorer board - The FX3 is a low-cost USB3 development board from Cypress.  The FX3 is used to buffer data from the FPGA and provide it to a host computer using USB3.  USB3 is required for this application due to the high-data bandwidth necessary for high-speed/high-resolution sampling.

## Motivation

The Domesday86 project is involved in the documentation and preservation of the BBC Domesday Project from 1986.  The Domesday project provided a set of analogue laserdiscs in a proprietary format (like standard laserdiscs but with some specific extensions only supported by the Philips VP415 laserdisc player).

The Domesday Duplicator is intended to allow high-quality back-ups of the analogue information contained on the laserdiscs by bypassing most of the 30-year-old electronics in the VP415 player.  Direct RF sampling also allows all information on the laserdiscs to be stored (unlike conventional RGB sampling of the video output).  Since AIV laserdiscs are a combination of video, pictures, sound and data (as well as numerous VBI streams), direct RF sampling is the preferred method of preservation, witch also applys to colour under analouge video tape formats such as (S)VHS, Video 8, High8, BetaCam and so on.

## Author

Domesday Duplicator is written and maintained by Simon Inns.

## Software License (GPLv3)

    Domesday Duplicator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Domesday Duplicator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Hardware License (Creative Commons BY-SA 4.0)

Please see the following link for details: https://creativecommons.org/licenses/by-sa/4.0/

You are free to:

Share - copy and redistribute the material in any medium or format
Adapt - remix, transform, and build upon the material
for any purpose, even commercially.

This license is acceptable for Free Cultural Works.

The licensor cannot revoke these freedoms as long as you follow the license terms.

Under the following terms:

Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

No additional restrictions - You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
