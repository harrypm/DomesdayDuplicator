/************************************************************************

	lower_case.scad
    
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
use <BOSL/paths.scad>

include <material.scad>

// Standoff for an M3 screw
module m3_standoff(x,y,z)
{
    move([x,y,z]) {
        difference() {
            union() {
                material(3) move([0,0,-1]) cyl(h=3, r1=5, r2=3, center = false);
                material(2) move([0,0,-3]) cyl(h=2, r1=5, r2=5, center = false);
            }
            material(3) move([0,0,-4]) cyl(h=7, r=1.6, center = false);
            material(2) move([0,0,-5]) cyl(h=5.1, r=3.1, center = false);
        }
        
    }
}

// Cutout for the M3 standoff
module m3_standoffc(x,y,z)
{
    move([x,y,z]) {
        union() {
            move([0,0,0]) cyl(h=2.2, r1=5, r2=3, center = false);
            move([0,0,-3]) cyl(h=3, r1=5, r2=5, center = false);
        }
    }
}

module render_text_cutouts()
{
    move([0,0,60]) {
        // Text for the RF input
        move([13,12.5,-38]) {
            rotate([90,0,0]) linear_extrude(0.7) {
                text("RF IN", font = "Liberation Sans:style=Bold", size = 4, valign = "center", halign="center");
            }
        }

        // Text for the USB3 connector
        move([74.5,89.5,-26]) {
            rotate([90,0,180]) linear_extrude(0.7) {
                text("USB3", font = "Liberation Sans:style=Bold", size = 4, valign = "center", halign="center");
            }
        }

        // Website address
        move([69.5,.5,-58]) {
            rotate([90,0,0]) linear_extrude(0.7) {
                text("www.domesday86.com", font = "Liberation Sans:style=Bold", size = 4, valign = "center", halign="center");
            }
        }
    }
}

module render_text()
{
    move([0,0,60]) {
        // Website address
        move([69.5,0.6,-58]) {
            rotate([90,0,0]) linear_extrude(0.6) {
                text("www.domesday86.com", font = "Liberation Sans:style=Bold", size = 4, valign = "center", halign="center");
            }
        }
    }
}

// Reccess for case clip
module case_clip_rec()
{
    cuboid([10,2,2], chamfer=0.5, trimcorners=false);
}

// Draw the lower part of the case
module render_lower_case()
{
    move([0,0,0]) {

        // Draw the base
        material(2) {
            difference() {
                union() {
                    move([32,-2,-4]) cuboid([75,94,2], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
                    move([-4, 10, -4]) cuboid([42 + 5,94 - 24,2], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
                }

                // Stand-offs for DE0-Nano (M3 screws)
                m3_standoffc(42.25,6.5,-1.01);
                m3_standoffc(84.75,6.5,-1.01);
                m3_standoffc(42.25,75.25,-1.01);
                m3_standoffc(84.75,75.25,-1.01);
            }
        }

        // Draw the sides
        difference() {
            union() {
                material(3) {
                    move([34,0,-2]) cuboid([71,90,60], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
                    move([-2, 12, -2]) cuboid([40 + 5,90 - 24,60], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
                }
            }

            material(3) {
                move([34+2,2,-3]) cuboid([71 - 4,90 - 4,65], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
                move([-2 + 2, 12 + 2, -3]) cuboid([40 + 5 -4,90-24 -4,64], center = false, fillet = 4, edges = EDGE_FR_LF+EDGE_FR_RT+EDGE_BK_LF+EDGE_BK_RT);
            }
            
            material(3) {
                // BNC hole
                move([12.5,12,36]) rotate([90,0,0]) cyl(h=5, r1=8, r2=8);

                // BNC cutout
                move([7,11.5,36]) cuboid([11,4,24 + 0.001], center = false);

                // USB cutout
                move([70 - 2,87,41 - 2]) cuboid([9 + 4,4,14], center = false);

                // Text cutouts
                render_text_cutouts();
            }

            // Draw the case clips
            material(3) {
                move([0,0,62]) {
                    move([49,2.5,-6]) case_clip_rec();
                    move([90,2.5,-6]) case_clip_rec();

                    move([49,87.5,-6]) case_clip_rec();
                    move([90,87.5,-6]) case_clip_rec();

                    rotate([0,0,90]) {
                        move([27,-0.5,-6]) case_clip_rec();
                        move([64,-0.5,-6]) case_clip_rec();

                        move([27,-102.5,-6]) case_clip_rec();
                        move([64,-102.5,-6]) case_clip_rec();
                    }
                }
            }
        }

        // RF BNC DdD board
        material(3) difference() {
            move([12.5,13.5,36]) rotate([90,0,0]) cyl(h=3, r1=7, r2=10);
            move([12.5,13.5,36]) rotate([90,0,0]) cyl(h=3.1, r1=6, r2=9);

            move([7,11.5,36]) cuboid([11,4,22], center = false);
        }

        // Stand-offs for DE0-Nano (M3 screws)
        m3_standoff(42.25,6.5,-1);
        m3_standoff(84.75,6.5,-1);
        m3_standoff(42.25,75.25,-1);
        m3_standoff(84.75,75.25,-1);
    }

    // Fill text
    if (global_fill_out_text == "Yes") {
        material(2) render_text();
    }
}