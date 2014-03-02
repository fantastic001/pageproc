
/*
 * Copyright (c) 2013 Stefan Nozinic
 *
 * This file is part of the pageproc. Pdf2epub is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * Pdf2epub is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef PAGEPROC_DEBUG_HPP
#define PAGEPROC_DEBUG_HPP

#include <string> 
#include <vector>

#include "matpart.hpp"
/*
 * Print all elements of a vector with message 
 *
 * Arguments:
 * 	1: std::string - message to be printed before printing vector elements
 * 	1: std::vector - vector which will be printed 
 * */
void print_int_vector(std::string, std::vector<int>);

void print_byte_vector(std::vector<char>);

void printLines(std::vector<HVLine>); 

#endif // PAGEPROC_DEBUG_HPP
