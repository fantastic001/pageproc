
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

#include "segmentation.hpp"

// debug 
#include "img_debug.hpp" 
#include "debug.hpp" 
#include <iostream> 

using namespace std; 
using namespace cv; 

#define MAX_WHITESPACE_ROWS 10
#define MAX_WHITESPACE_COLS 15

// binary remover 
vector<int> binRemover(vector<int> bin) 
{
	vector<int> newBin; 
	if (bin.size() <= 1) 
	{
		return bin; 
	}
	for (int i = 0; i<bin.size()-1; i++) 
	{
		int num = bin.at(i);
		int nextNum = bin.at(i+1); 
		if (num == 1 && nextNum == 0) 
		{
			newBin.push_back(1);
		}
		else 
		{
			newBin.push_back(0); 
		}
	}
	newBin.push_back(bin.at(bin.size()-1));

	return newBin; 
}

// Removes unneeded lines 
vector<HVLine> lineRemover(vector<HVLine> lines, int type) 
{
	vector<HVLine> nlines; 
	if (lines.empty()) return nlines; 
	nlines.push_back(lines.at(0));
	for (int i = 1; i<lines.size(); i++) 
	{
		int d; 
		if (type == REMOVER_TYPE_VERTICAL) 
		{
			d = lines.at(i).x - lines.at(i-1).x; 
		}
		else 
		{
			d = lines.at(i).y - lines.at(i-1).y;
		}

		if (d > 1)  
		{
			nlines.push_back(lines.at(i));
		}
	}
	return nlines; 
}

vector<int> xy_cut(vector<int> sum, int max_whitespace) 
{
	vector<int> result; 
	int state = 1; 
	int c = 0; 
	int i;
	
	if (sum.empty()) return result; 

	for (i = 0; i<sum.size(); i++) 
	{
		int p = sum.at(i);
		if (p == 255 && state == 1) c++; 
		else if (p < 255 && state == 0) c++;
		else if (p == 255 && state == 0) 
		{
			c = 1; 
			state = 1; 
		}
		else if (p < 255 && state == 1) 
		{
			if (c >= max_whitespace) 
			{
				result.push_back(i-c);
				result.push_back(i);
			}
			c = 1; 
			state = 0; 
		}
	}
	bool found = false; 
	for (i = 0; i<7 && i<sum.size(); i++) 
	{
		if (sum.at(i) < 255) 
		{
			found = true; 
			break; 
		}
	}
	if (found) result.push_back(0);
	
	if (sum.at(sum.size() - 1) == 255) result.push_back(sum.size() - c);
	else result.push_back(sum.size() - 1);
	sort(result.begin(), result.end());
	if (!found) result.erase(result.begin());
	return result;
}


void divideIntoBlocks(MatPart &part, int type, bool recursive_call) 
{
	int max_whitespace;
	if (type == DIVIDE_BY_ROWS) max_whitespace = MAX_WHITESPACE_ROWS; 
	else max_whitespace = MAX_WHITESPACE_COLS;
	vector<int> sum = genSum(part.mat, type); 
	vector<int> result = xy_cut(sum, max_whitespace);
	if (type == DIVIDE_BY_ROWS) 
	{
		part.childs = divByRow(part, result);
	}
	else 
	{
		part.childs = divByCol(part, result);
	}
	if (part.childs.size() <= 1 && recursive_call) return; 
	for (int i = 0; i<part.childs.size(); i++) 
	{
		divideIntoBlocks(part.childs.at(i), (type + 1) % 2, true);
	}
}

vector<MatPart> divByRow(MatPart src, vector<int> points) 
{
	vector<MatPart> parts; 
	for (int i = 0; i<points.size(); i += 2) 
	{
		MatPart part; 
		part.x = src.x;
		part.y = src.y + points.at(i); 
		part.width = src.mat.cols;
		part.height = points.at(i+1) - points.at(i);
		part.mat = Mat(src.mat, Rect(0, points.at(i), part.width, part.height));
		parts.push_back(part); 
	}
	return parts; 
}

vector<MatPart> divByCol(MatPart src, vector<int> points) 
{
	vector<MatPart> parts;
	for (int i = 0; i<points.size(); i += 2) 
	{
		MatPart part;
		part.x = src.x + points.at(i);
		part.y = src.y;
		part.width = points.at(i+1) - points.at(i);
		part.height = src.mat.rows;
		part.mat = Mat(src.mat, Rect(points.at(i), 0, part.width, part.height));
		parts.push_back(part);
	}
	return parts;
}


vector<int> genSum(Mat img, int type) 
{
	if (type == ROWS_SUM) 
	{
		
		vector<int> sum; 
		for (int i = 0; i<img.rows; i++) 
		{
			int z = 0; 
			for (int j = 0; j<img.cols; j++) 
			{
				z = z + (int) img.data[img.step[0]*i + img.step[1]*j];
			}
			sum.push_back(z / img.cols);
		}
		return sum;
	}
	else 
	{
		vector<int> sum; 
		for (int i = 0; i<img.cols; i++) 
		{
			int z = 0; 
			for (int j = 0; j<img.rows; j++) 
			{
				z = z + (int) img.data[img.step[0]*j + img.step[1]*i];
			}
			sum.push_back(z / img.rows);
		}
		return sum;
	}
}

double avgRowSize(MatPart part) 
{
	vector<MatPart> parts = divByRow(part, xy_cut(genSum(part.mat, ROWS_SUM), 1));
	double sum = 0; 
	double count = (double) parts.size();
	for (int i = 0; i<count; i++) 
	{
		sum = sum + parts.at(i).mat.rows;
	}
	if (count == 0) return 0; 
	return (double) (sum / count);
}


std::vector<HVLine> findHLines(Mat  img, int threshold) 
{
	bool state;
	int l;
	int i,j;
	int maxVal = 240;
	vector<HVLine> lines;
	for (i = 0; i<img.rows; i++) 
	{
		state = false; 
		l = 0;
		if (img.data[img.step[0]*i] < maxVal) 
		{
			state = true; 
			l++; 
		}
		for (j = 0; j<img.cols; j++) 
		{
			if (img.data[img.step[0]*i + img.step[1]*j] < maxVal && state) l++; 
			else if (img.data[img.step[0]*i + img.step[1]*j] < maxVal && !state) 
			{
				state = true; 
				l++;
			}
			else if (img.data[img.step[0]*i + img.step[1]*j] >= maxVal && state) 
			{
				if (l > threshold) {
					HVLine line; 
					line.x = j; line.y = i; line.length = l;
					lines.push_back(line);
				}
				state = false; 
				l = 0;
			}
			else 
			{
				// maybe this will change in the future
				state = false; 
				l = 0;
			}
		}
		if (img.data[img.step[0]*i + img.step[1]*(j-1)] < maxVal) 
		{
			if (l > threshold) {
				HVLine line; 
				line.x = j; line.y = i; line.length = l;
				lines.push_back(line);
			}
		}
	}
	return lineRemover(lines, REMOVER_TYPE_HORIZONTAL); 
}

std::vector<HVLine> findVLines(Mat img, int threshold) 
{
	bool state;
	int l;
	int i,j;
	int maxVal = 240;
	vector<HVLine> lines;
	for (j = 0; j<img.cols; j++) 
	{
		state = false; 
		l = 0;
		if (img.data[img.step[1]*j] < maxVal) 
		{
			state = true; 
			l++; 
		}
		for (i = 0; i<img.rows; i++) 
		{
			if (img.data[img.step[0]*i + img.step[1]*j] < maxVal && state) l++; 
			else if (img.data[img.step[0]*i + img.step[1]*j] < maxVal && !state) 
			{
				state = true; 
				l++;
			}
			else if (img.data[img.step[0]*i + img.step[1]*j] >= maxVal && state) 
			{
				if (l > threshold) {
					HVLine line; 
					line.x = j; line.y = i; line.length = l;
					lines.push_back(line);
				}
				state = false; 
				l = 0;
			}
			else 
			{
				// maybe this will change in the future
				state = false; 
				l = 0;
			}
		}
		if (img.data[img.step[0]*(i-1) + img.step[1]*j] < maxVal) 
		{
			if (l > threshold) {
				HVLine line; 
				line.x = j; line.y = i; line.length = l;
				lines.push_back(line);
			}
		}
	}
	return lineRemover(lines, REMOVER_TYPE_VERTICAL); 
} 

vector<Point> intersections(vector<HVLine> hlines, vector<HVLine> vlines) 
{
	vector<Point> points; 

	for (int i = 0; i<hlines.size(); i++) 
	{
		for (int j = 0; j<vlines.size(); j++) 
		{
			cout << "Calculating dy and dx\n"; 
			int dy = vlines.at(j).y - hlines.at(i).y; 
			int dx = hlines.at(i).x - vlines.at(j).x; 
			if (dy <= vlines.at(j).length && dx <= hlines.at(i).length) 
			{
				cout << "pushing\n"; 
				points.push_back(Point(vlines.at(j).x, hlines.at(i).y));
			}
		}
	}
	return points;
} 

HVLine* findLine(vector<HVLine> lines, int x, int type) 
{
	cout << "starting..\n";
	for (int i = 0; i<lines.size(); i++) 
	{
		if (type == BY_X)
		{
			if (lines.at(i).x == x) 
			{
				return &lines.at(i); 
			}
		}
		else 
		{
			if (lines.at(i).y == x) 
			{
				return &lines.at(i); 
			}
		}
	}
	return NULL; 
}

vector<int> binarizeHorizontalLines(Mat img, vector<HVLine> lines) 
{
	//int li = 0; 
	vector<int> bin; 
	for (int i = 0; i<img.rows; i++) 
	{
		/*
		if (li < lines.size())
		{
			if (lines.at(li).y == i) 
			{
				bin.push_back(1);
				li++; 
			}
			else 
			{
				bin.push_back(0);
			}
		}*/
		if (findLine(lines, i, BY_Y) != NULL) 
		{
			bin.push_back(1); 
		}
		else 
		{
			bin.push_back(0);
		}
	}
	return bin; 
}

vector<HVLine> formHLines(vector<int> bin) 
{
	vector<HVLine> lines; 
	for (int i = 0; i<bin.size(); i++) 
	{
		if (bin.at(i) == 1) {
			HVLine line; 
			line.x = bin.size()-1;
			line.y = i;
			line.length = bin.size() - 1; 
			lines.push_back(line); 
		}
	}
}

std::vector<HVLine> formVLines(std::vector<int> bin) 
{	
	vector<HVLine> lines; 
	for (int i = 0; i<bin.size(); i++) 
	{
		if (bin.at(i) == 1) {
			HVLine line; 
			line.y = bin.size()-1;
			line.x = i;
			line.length = bin.size() - 1; 
			lines.push_back(line); 
		}
	}
}

vector<int> binarizeVerticalLines(Mat img, vector<HVLine> lines) 
{
	int li = 0; 
	vector<int> bin; 
	for (int i = 0; i<img.cols; i++) 
	{
		/*if (li < lines.size())
		{
			if (lines.at(li).x == i) 
			{
				bin.push_back(1);
				li++; 
			}
			else 
			{
				bin.push_back(0);
			}
		}*/
		if (findLine(lines, i, BY_X) != NULL) 
		{
			bin.push_back(1); 
		}
		else 
		{
			bin.push_back(0);
		}
	}
	return bin; 
}

void getEndParts(MatPart part, vector<MatPart>& parts) 
{
	if (part.childs.empty()) 
	{
		parts.push_back(part); 
	}
	else 
	{
		for (int i = 0; i<part.childs.size(); i++) 
		{
			getEndParts(part.childs.at(i), parts);
		}
	}
}

/*vector<int> lineHistogram(vector<HVLine> hlines, vector<HVLine> vlines, int type)
{
	int i;
	int maxx = 0; 
	int maxy = 0; 
	int hs = hlines.size();
	int vs = vlines.size();
	for (i = 0; i<hlines.size(); i++) 
	{
		if (maxx < hlines.at(i).x) maxx = hlines.at(i).x; 
		if (maxy < hlines.at(i).y) maxy = hlines.at(i).y;
	}
	for (i = 0; i<vlines.size(); i++) 
	{
		if (maxx < vlines.at(i).x) maxx = vlines.at(i).x; 
		if (maxy < vlines.at(i).y) maxy = vlines.at(i).y;
	}

	vector<int> sum; 
	for (i = 0; i<maxx; i++) 
	{
		int z = hs; 
		for (int j = 0; j<vlines.size(); j++) 
		{
			if (i == vlines.at(i).x)
		}
	}

}*/
