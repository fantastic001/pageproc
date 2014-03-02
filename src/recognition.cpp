
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

#include "recognition.hpp" 

#include "img_debug.hpp"
#include "segmentation.hpp"

#include <vector> 

#include <imgproc/imgproc.hpp>

// debug
#include <iostream>
#include "debug.hpp"
#include <highgui/highgui.hpp>

using namespace std; 
using namespace cv; 

bool recognizeText(MatPart part) 
{
	// RULE: number of vertical lines must be less then or equal to 1 
	vector<HVLine> vlines = findVLines(part.mat, 46);
	if (vlines.size() > 1) 
	{
		cout << "Found vertical lines, it is not text. \n"; 
		cout << "Lengths: \n"; 
		for (int i = 0; i<vlines.size(); i++) 
		{
			cout << vlines.at(i).length << endl; 
		}
		return false; 
	}

	// We need to erode image before processing
	//Mat dst;
	//threshold(part.mat, dst, 40, 255,  THRESH_BINARY_INV);
	
	// debug 
	/*
	namedWindow("win");
	imshow("win", dst); 
	waitKey(0); 
	dilate(dst, dst, getStructuringElement(MORPH_RECT, Size(10, 1)));
	
	// debug 
	namedWindow("win");
	imshow("win", dst); 
	waitKey(0); 
	*/
	
	/*
	vector<vector<Point> > contours; 
	findContours(dst, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	
	if (contours.size() < 1) 
	{
		cout << "no contours\n"; 
		return false; 
	}
	*/
	
	/*
	for (int i = 1; i<contours.size(); i++) 
	{
		int A = contours.at(i).size();
		Rect r = getRectAround(contours.at(i));
		int a = r.width * r.height;
		if (r.height > r.width && (r.width > 8 && r.height > 8)) 
		{
			cout << "bad proportion " << r.width << " " << r.height << endl;  
			return false; 
		}
		if (A < a / 5) 
		{
			cout << "bad area " << a << " " << A << endl; 
			return false;
		}
	}
	*/

	/*
	vector<int> sumc = genSum(dst, COLUMNS_SUM); 
	vector<int> sumr = genSum(dst, ROWS_SUM);;
	*/

	/*
	print_int_vector("sumc", sumc); 
	print_int_vector("sumr", sumr); 
	*/

	/*
	int state; 
	int i; 
	int maxVal = 80;

	state = 0; 
	for (i = 0; i<sumc.size(); i++) 
	{
		int z = sumc.at(i); 
		if (state == 0 && z < maxVal) 
		{
			state = 1; 
		}
		else if (state == 1 && z >= maxVal) 
		{
			state = 2; 
		}
		else if (state == 2 && z < maxVal) 
		{
			return false; 
		}
	}
	if (state == 0) return false; 

	state = 0; 
	for (i = 0; i<sumr.size(); i++) 
	{
		int z = sumr.at(i); 
		if (state == 0 && z < maxVal) 
		{
			state = 1; 
		}
		else if (state == 1 && z >= maxVal) 
		{
			state = 2; 
		}
		else if (state == 2 && z < maxVal) 
		{
			return false; 
		}
	}
	if (state == 0) return false; 
	*/

	int i,j;

	// RULE: at least one row or column is not empty and one is empty
	int ner = 0, nec = 0; 
	int er = 0, ec = 0;
	vector<int> sumr = genSum(part.mat, ROWS_SUM);
	vector<int> sumc = genSum(part.mat, COLUMNS_SUM);
	cout << "Row sum: \n"; 
	/*for (i = 0; i<sumr.size(); i++) 
	{
		cout << sumr.at(i) << endl; 
	}*/
	for (int row = 0; row<sumr.size(); row++) 
	{
		if (sumr.at(row) < 255) 
		{
			ner++;
		}
		if (sumr.at(row) == 255) 
		{
			er++;
		}

	}
	for (int col = 0; col<sumc.size(); col++) 
	{
		if (sumc.at(col) < 255) 
		{
			nec++;
		}
		if (sumc.at(col) == 255) 
		{
			ec++;
		}

	}
	if ((ner < 1 || er < 1) && (nec < 1 || ec < 1))
	{
		cout << "has no empty or nonempty rows/cols\n";
		return false;
	}

	// RULE: Every row must have width > height and both must be > 8px 
	//MatPart dst_part = genMatPartFromMat(dst);
	divideIntoBlocks(part, DIVIDE_BY_ROWS);
	vector<MatPart> chunks;
	getEndParts(part, chunks); 
	for (i = 0; i<chunks.size(); i++) 
	{
		if (chunks.at(i).width < chunks.at(i).height && (chunks.at(i).width > 8 || chunks.at(i).height > 8))
		{
			cout << "bad proportion " << chunks.at(i).width << " " << chunks.at(i).height << endl;
			return false; 
		}
	}

	// RULE: area rule
	int A = 0, B = 0; 
	for (i = 0; i<part.mat.rows; i++) 
	{
		for (j = 0; j<part.mat.cols; j++) 
		{
			unsigned char p = part.mat.data[part.mat.step[0]*i + part.mat.step[1]*j];
			if (p == 255) A++; 
			else B++; 
		}
	}
	cout << "Area ratio " << A << " " << B << endl;
	return true; 
}

// ah - average font height
bool recognizeTable(MatPart part, int ah) 
{	
	Mat dst; 
	erode(part.mat, dst, getStructuringElement(MORPH_RECT, Size(2, 2)));

	vector<HVLine> hlines = findHLines(dst, dst.cols/2); 
	vector<HVLine> vlines = findVLines(dst, dst.rows/2);
		
	// Check does it have a border 
	if (!hasBorder(hlines, vlines)) 
	{
		cout << "Has no border\n"; 
		return false; 
	}	

	// Check for number of horizontal and vertical lines 
	if (hlines.size() < 3 && vlines.size() < 3) 
	{
		cout << "too few lines\n"; 
		return false; 
	}
	
	return true; 
}


// before Mat img 
bool hasBorder(vector<HVLine> hlines, vector<HVLine> vlines)  
{
	if (hlines.empty() || vlines.empty()) return false; 

	HVLine hfirst = hlines.at(0);
	HVLine hlast = hlines.at(hlines.size() - 1);

	HVLine vfirst = vlines.at(0); 
	HVLine vlast = vlines.at(vlines.size() - 1);

	int x1 = hfirst.x - hfirst.length; 
	int y1 = hfirst.y;

	int x2 = hfirst.x; 
	int y2 = hfirst.y; 

	int x3 = hlast.x - hlast.length; 
	int y3 = hlast.y; 

	int x4 = hlast.x; 
	int y4 = hlast.y; 

	if (abs(vfirst.x - x3) < 3 && 
		abs(vfirst.y - y3) < 3 && 
		abs(vfirst.x - x1) < 3 && 
		abs(vfirst.y - vfirst.length - y1) < 3 && 
		abs(vlast.x - x4) < 3 && 
		abs(vlast.y - y4) < 3 &&
		abs(vlast.x - x2) < 3 && 
		abs(vlast.y - vlast.length - y2) < 3) 
	{
		return true; 
	}
	else 
	{
		return false; 
	}
}

bool recognizeDrawing(MatPart part) 
{
	Mat dst; 
	threshold(part.mat, dst, 240, 255,  THRESH_BINARY_INV);

	vector<vector<Point> > contours; 
	findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	// Find contour with more then 10 points and if it's found, then it is drawing because it is not text or table
	bool found = false; 
	for (int i = 0; i<contours.size(); i++) 
	{
		int cp = contours.at(i).size(); 
		if (cp > 10) {
			found = true; 
			break; 
		}
	}
	if (! found) 
	{
		cout << "Valid contour not found\n"; 
	}
	return found; 
}
