
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

#ifndef PAGEPROC_MATPART_HPP
#define PAGEPROC_MATPART_HPP

#include <core/core.hpp>
#include <vector> 
#include <string> 

typedef struct MatPart MatPart;

struct MatPart
{
	cv::Mat mat; 
	int x,y,height,width; 
	std::vector<MatPart> childs;
};

struct HVLine 
{
	int x,y,length;
};


/*
 * Generates a MatPart object from Mat 
 *
 * Arguments: 
 * 	1: cv::Mat - Mat object from which will be generated MatPart
 *
 * Returns: MatPart - generated object 
 * */
MatPart genMatPartFromMat(cv::Mat); 


/* 
 * Saves a given partmart to the HDD (or something like that) 
 *
 * */
void saveToFile(MatPart, std::string);

#endif // PAGEPROC_MATPART_HPP
