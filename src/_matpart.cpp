
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

#include "matpart.hpp"

#include <highgui/highgui.hpp>

using namespace cv; 

MatPart genMatPartFromMat(Mat m) 
{
	MatPart whole;
	whole.mat = m;
	whole.x = 0; 
	whole.y = 0; 
	whole.height = whole.mat.cols; 
	whole.width = whole.mat.rows;
	return whole; 
}

void saveToFile(MatPart part, string filename) 
{
	imwrite(filename, part.mat); 
}
