/************************************************************************

	ddd_pcb.scad
    
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

// Draw the BNC connector
module bnc_connector()
{
    // Rosenberger 51K204-400A5
    color("darkgrey") {
        difference() {
            union() {
                move([-6,0,0]) cuboid([12,12, 18.5 - 4], center = false);
                move([0,0,9]) rotate([90,0,0]) cyl(h=8.1, r=5.5, center = false);
                move([0,-8.1,9]) rotate([90,0,0]) cyl(h=1, r1=5.5, r2=5, center = false);
                move([0,-8.1,9]) rotate([90,0,0]) cyl(h=12.5, r=5, center = false);

                // Bayonet pins
                move([0,-17.1,3.5]) rotate([0,0,0]) cyl(h=11, r=1, center = false);
            }

            move([0,-8.2,9]) rotate([90,0,0]) cyl(h=12.5, r=4.5, center = false);
        }

        // Connection pins
        move([6 - 1,1,-4]) cyl(h=4, r=0.6, center = false);
        move([-6 + 1,1,-4]) cyl(h=4, r=0.6, center = false);

        move([6 - 1,10 + 1,-4]) cyl(h=4, r=0.6, center = false);
        move([-6 + 1,10 + 1,-4]) cyl(h=4, r=0.6, center = false);

        move([0,6,-4]) cyl(h=4, r=0.45, center = false);
    }

    color("white") {
        // Center part of BNC plug (for decoration)
        move([0,-8,9]) rotate([90,0,0]) tube(h=10, ir = 0.75, or=2.5, center = false);
    }
}

// Draw the PCB
module pcb()
{
    color("green") {
        move([0,-27,-1.6 / 2]) cuboid([95,57,1.6], center = false);
    }
}

// Draw a male pin header (7mm pins)
module male_header()
{
    color("black") cuboid([5,50,3], center = false);
    color("yellow") move([1,0.5,3]) cuboid([3,49,7], center = false);
}

// Draw a female pin header (7 mm pins)
module female_header()
{
    color("black") cuboid([5,51,8], center = false);
}

// Draw the headers
module headers()
{
    // Top headers
    move([45.5,-22.5,1.6 / 2]) male_header();
    move([87.5,-22.5,1.6 / 2]) male_header();

    // Bottom headers
    move([36,-22.75,-8.7]) female_header();
    move([79.5,-22.75,-8.7]) female_header();
}

// Render the DdD PCB and components
module render_ddd_pcb()
{
    move([9.6,-17.8,1.6 / 2]) bnc_connector();
    pcb();
    headers();
}