/************************************************************************

	main.scad
    
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

include <ddd_pcb.scad>
include <de0-nano.scad>
include <cypress_usb3.scad>
include <upper_case.scad>
include <lower_case.scad>

include <BOSL/constants.scad>
use <BOSL/transforms.scad>

// Rendering quality
$fn=50;

// Choose what to display:
display_DE0Nano_board = "Yes"; // [Yes, No]
display_DdD_board = "Yes"; // [Yes, No]
display_USB3_board = "Yes"; // [Yes, No]
display_upper_case = "Yes"; // [Yes, No]
display_lower_case = "Yes"; // [Yes, No]

// Choose materials to render (0 = all)
material_choice = 0; // [0, 1, 2, 3, 4, 5]

// Fill out text (Select no for single material printing)
fill_out_text = "Yes"; // [Yes, No]

// Add logo to the upper case (select no for single material)
add_logo = "Yes"; // [Yes, No]

// Main function
global_current_material = material_choice; // Global for material selection
global_fill_out_text = fill_out_text; // Global for material selection
global_add_logo = add_logo; // Global for material selection

// Display the Duplicator PCB?
if (display_DdD_board == "Yes") {
    move([0,0,15]) render_ddd_pcb();
}

// Display the DE0-Nano FPGA?
if (display_DE0Nano_board == "Yes") {
    move([36,36.5,0]) render_de0_nano();
}

// Display the Cypress USB3 PCB?
if (display_USB3_board == "Yes") {
    move([41,-38,26.5]) render_usb3();
}

// Display the upper case?
if (display_upper_case == "Yes") {
    move([-3,-42,51]) render_upper_case();
}

// Display the lower case?
if (display_lower_case == "Yes") {
    move([-3,-42,-11]) render_lower_case();
}
