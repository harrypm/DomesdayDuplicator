# Introduction


This repository contains a 3D printable case design for the Domesday Duplicator.

The case is designed using OpenSCAD and full source code is included.  There are also ready-made STL files for the various parts and materials if you don't want to build them yourself in OpenSCAD.

The case design contains multiple materials however you can easily remix the design using OpenSCAD customization GUI if you wish to print in a single material only (simply deselect the 'fill out text' and 'add logo' options and re-render).  Ready-made single material STL files are also included.

<img src="/Graphics/case1.png" width="800">


# Printing instructions


## Overview


The model has been printed and tested on the Prusa MK3S/MMU2S printer. All parts fit on a 20x20 printing bed.

The OpenSCAD source files provide a parameter interface that allows you to render either the whole model or the individual parts (ready for STL (or other 3D format) export).  This requires OpenSCAD 2019.05 or later.


## Recommended Print Settings


- Material: PLA Blue and PLA White
- Layer: 0.20mm (Quality)
- Infill: 15%
- Supports: None
- Notes: The upper and lower case parts simply snap together.  4x M3 6mm screws are required to hold the electronics in place.


## Print Time


Clearly this depends on your printer and it's settings.  For the 2 colour multi-material print the time was 11h 15m for both the upper and lower case.  For single-material printing to time was 8h 30m.


## Notes


The design also includes approximate renderings of the FPGA board, the DdD board and the USB3 board.  These are to assist with the design process and are not intended for printing.

<img src="/Graphics/case3.png" width="800">


# Author


The DomesdayDuplicator case is designed and maintained by Simon Inns.