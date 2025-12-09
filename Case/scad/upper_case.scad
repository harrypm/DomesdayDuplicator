/************************************************************************

	upper_case.scad
    
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

// Draw the DdD logo (from an imported SVG file)
module ddd_logo()
{
    move([0,0,0]) {
        rotate([0,0,0]) {
            linear_extrude(1, center = true) scale(0.4) import(file = "svg/DomesdayDuplicatorlogo.svg", center = false, dpi = 96);
        }
    }
}

module case_clip()
{
    cuboid([10,2,2], chamfer=0.5, trimcorners=false);
}

// Draw the upper part of the case
module render_upper_case()
{
    // Lid
    move([0,0,0]) {
        material(2) difference() {
            union(){
                move([32,-2,-4]) cuboid([75,94,2], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
                move([-4, 10, -4]) cuboid([42 + 5,94 - 24,2], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
            }

            // Cut out for logo
            if (global_add_logo == "Yes") {
                move([10,21,-2.5 + 0.001]) ddd_logo();
            }
        }
    }

    // Tab for BNC connector
    move([0,0,-62]) {
        material(3) difference() {
            // BNC cutout
            move([7,12,42]) cuboid([11,3,16], center = false);
            move([12.5,12,36]) rotate([90,0,0]) cyl(h=6, r1=8, r2=8);
        }

        material(3) difference() {
            move([7,13,40]) cuboid([11,2,18], center = false);
            move([12.5,13.5,36]) rotate([90,0,0]) cyl(h=3.01, r1=7, r2=10);
        }

        // RF BNC DdD board
        material(3) difference() {
            move([12.5,13.5,36]) rotate([90,0,0]) cyl(h=3, r1=7, r2=10);
            move([12.5,13.5,36]) rotate([90,0,0]) cyl(h=3.1, r1=6, r2=9);
            move([12.5,12,36]) rotate([90,0,0]) cyl(h=1, r1=8, r2=8);

            move([1,11,21]) cuboid([22,5,18], center = false);

            move([1,11,35]) cuboid([6.1,5,23.1], center = false);
            move([18 - 0.1,11,35]) cuboid([6.1,5,23.1], center = false);
        }
    }

    // Lip around lid
    move([0,0,0]) {
        material(3) difference() {
            union() {
                move([34 + 2,0 + 2,-8]) cuboid([71 - 4,90 - 4,4], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
                move([-2 + 2, 12 + 2, -8]) cuboid([40 + 5 - 4,90 - 24 - 4,4], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
            }

            move([34 + 4,0 + 4,-9]) cuboid([71 - 8,90 - 8,6], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
            move([-2 + 4, 12 + 4, -9]) cuboid([40 + 5 - 8,90 - 24 - 8,6], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
        }
    }

    // Add in the logo material
    if (global_add_logo == "Yes") {
        material(3) {
            move([10,21,-2.5 + 0.001]) ddd_logo();
        }
    }
    
    // Draw the case clips
    material(3) {
        move([49,2.5,-6]) case_clip();
        move([90,2.5,-6]) case_clip();

        move([49,87.5,-6]) case_clip();
        move([90,87.5,-6]) case_clip();

        rotate([0,0,90]) {
            move([27,-0.5,-6]) case_clip();
            move([64,-0.5,-6]) case_clip();

            move([27,-102.5,-6]) case_clip();
            move([64,-102.5,-6]) case_clip();
        }
    }
}