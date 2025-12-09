/************************************************************************

	cypress_usb3.scad
    
	Domesday Duplicator - 3D Printed Case
    Copyright (C) 2020 Simon Inns
	
    This file is part of Domesday Duplicator.
    
    This is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
    Email: simon.inns@gmail.com
    
************************************************************************/

include <BOSL/constants.scad>
use <BOSL/shapes.scad>
use <BOSL/transforms.scad>

// Draw the female header (7mm pins)
module usb3_female_header()
{
    color("black") cuboid([5,51,8], center = false);
}

// Draw the PCB
module usb3_pcb()
{
    color("darkblue") cuboid([56, 82.3, 1.6], center = false, fillet = 2, edges = EDGE_BK_RT+EDGE_BK_LF+EDGE_FR_RT+EDGE_FR_LF);
}

// Draw the USB3 connector
module usb3_usbheader()
{
    color("darkgrey") {
        move([(56 / 2) - 3.5,82.3 - 18,1.6]) cuboid([12,18,13], center = false);
    }

    color("blue") {
        move([(56 / 2) - 2,82.3 - 18 + 1,2.6]) cuboid([9,17.01,7], center = false);
        move([(56 / 2) - 0,82.3 - 18 + 1,2.6 + 7]) cuboid([5,17.01,4], center = false);
    }
}

// Draw the debug USB connector
module usb3_microheaader()
{
    color("darkgrey") {
        move([(56 / 2) - 3.5,-0.5,1.6]) cuboid([7,5.5,3], center = false, fillet = 1, edges = EDGE_TOP_LF+EDGE_TOP_RT+EDGE_BOT_LF+EDGE_BOT_RT);
    }
}

// Render the Cypress USB3 board
module render_usb3()
{
    move([0,0,0]) usb3_pcb();
    move([4.5,15,-8]) usb3_female_header();
    move([51 - 4.5,15,-8]) usb3_female_header();

    move([0,0,0]) usb3_usbheader();
    move([0,0,0]) usb3_microheaader();
}