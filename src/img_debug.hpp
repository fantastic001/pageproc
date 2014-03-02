
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

#ifndef PAGEPROC_IMG_DEBUG_HPP
#define PAGEPROC_IMG_DEBUG_HPP

#include <core/core.hpp>

#include <vector>

#include "matpart.hpp"

/*
 * Returns a rectangle around points
 *
 * Arguments: 
 * 	1: std::vector<cv::Point> - vector of points
 *
 * Returns: cv::Rect - rectangle around points
 * */
cv::Rect getRectAround(std::vector<cv::Point>);

/*
 * Draws rectangles on the image showing its parts 
 *
 * Arguments: 
 * 	1: cv::Mat - image 
 * 	2: MatPart - begining part of image, part will be displayed if it's child of begining part
 * 	3: std::string - window name (default: "img")
 * */
void showParts(cv::Mat, MatPart, std::string wn = "img");

/*
 * Shows part on image 
 *
 * Arguments: 
 * 	1: cv::Mat - image 
 * 	2: MatPart - part of image which needs to be shown
 * 	3: std::string - window name for showing image (default: "img")
 * 	4: DocumentPage* - if points to DocumentPage object, prints text of part in that region (default: NULL)
 * */
void showPartAsRect(cv::Mat, MatPart, std::string wn = "img");


#endif // PAGEPROC_IMG_DEBUG_HPP
