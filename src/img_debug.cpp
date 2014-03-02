
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

#include "img_debug.hpp"

#include <highgui/highgui.hpp>

#include <iostream>

// avgRowSize(...)
#include "segmentation.hpp"
#include "debug.hpp"

// debug 
#include "recognition.hpp"

using namespace cv; 
using namespace std;

Rect getRectAround(vector<Point> points) 
{
	int maxx = -1, maxy = -1, miny = -1, minx = -1;
	int i;
	// Find maxx 
	for (i = 0; i<points.size(); i++) 
	{
		if (maxx == -1 || maxx > points.at(i).x) maxx = points.at(i).x;
	}
	
	// Find maxy 	
	for (i = 0; i<points.size(); i++) 
	{
		if (maxy == -1 || maxy > points.at(i).y) maxy = points.at(i).y;
	}

	// Find minx 
	for (i = 0; i<points.size(); i++) 
	{
		if (minx == -1 || minx < points.at(i).x) minx = points.at(i).x;
	}

	// Find miny
	for (i = 0; i<points.size(); i++) 
	{
		if (miny == -1 || miny < points.at(i).y) miny = points.at(i).y;
	}
	return Rect(maxx, maxy, minx - maxx + 1, miny - maxy + 1);
}


void showPartAsRect(Mat img, MatPart part, string wn) 
{
	rectangle(img, Point(part.x, part.y), Point(part.x + part.width, part.y + part.height), Scalar(0), 2);
	imshow(wn, img);

	// Just save for later testing ... 
	saveToFile(part, "part.png");
	/* debug */ cout << avgRowSize(part) << endl;
	/* debug */ cout << "recognizeTable: "; 
		    recognizeTable(part, 32);
		    cout << endl; 
	waitKey(0);
}

void showParts(Mat img, MatPart whole, string wn) 
{
	namedWindow(wn);
	if (whole.childs.empty()) 
	{
		showPartAsRect(img.clone(), whole, wn);
	}
	else 
	{
		for (int i = 0; i<whole.childs.size(); i++) 
		{
			showParts(img, whole.childs.at(i), wn); 
		}
	}
}

