/************************************************************************

	de0-nano.scad
    
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

// Draw a male pin header (7mm pins)
module male_nano_header()
{
    color("black") cuboid([5,50,3], center = false);
    color("yellow") move([1,0.5,3]) cuboid([3,49,7], center = false);
}

// Draw the DE0-Nano FPG board
module render_de0_nano()
{
    move([0,-75.18,0]) {
        difference() {
            cuboid([49.02, 75.18, 11], center = false);

            // Seperate PCB and case
            move([-0.5,-0.5,1.6]) cuboid([50, 76, 5], center = false);

            // Screw holes
            move([3.3, 3.05, 5.5]) cyl(h=13, r = 1.6);
            move([45.72, 3.05, 5.5]) cyl(h=13, r = 1.6);

            move([3.3, 71.88, 5.5]) cyl(h=13, r = 1.6);
            move([45.72, 71.88, 5.5]) cyl(h=13, r = 1.6);

            // Cutouts for header pins
            move ([-0.01,12.5,1.6]) cuboid([7, 57, 11], center = false);
            move ([45.72 - 3.7 + 0.01,12.5,1.6]) cuboid([7, 57, 11], center = false);

            // Cutout for mini USB connector
        }
    }

    // Header pin rows
    move([0,-75.18 + 16.26,1.6]) male_nano_header();
    move([49.02 - 5,-75.18 + 16.26,1.6]) male_nano_header();

    color("yellow") move([0,-75.18,0]) {
        // Standoffs
        $fn = 6;
        move([3.3, 3.05, -5]) cyl(h=10, r = 2.5);
        move([45.72, 3.05, -5]) cyl(h=10, r = 2.5);

        move([3.3, 71.88, -5]) cyl(h=10, r = 2.5);
        move([45.72, 71.88, -5]) cyl(h=10, r = 2.5);
    }

    color("yellow") move([0,-75.18,9]) {
        // Standoffs
        $fn = 6;
        move([3.3, 3.05, -5]) cyl(h=5.4, r = 2.5);
        move([45.72, 3.05, -5]) cyl(h=5.4, r = 2.5);

        move([3.3, 71.88, -5]) cyl(h=5.4, r = 2.5);
        move([45.72, 71.88, -5]) cyl(h=5.4, r = 2.5);
    }

    // USB mini connector
    move([9,-75,1.6]) cuboid([7.5,9,3.5], center = false, chamfer = 1.2, edges = EDGE_BOT_LF+EDGE_BOT_RT);
}