
#include <core/core.hpp> 
#include <highgui/highgui.hpp> 
#include <imgproc/imgproc.hpp> 

#include <recognition.hpp>

#include <iostream>

#include <element.hpp>
#include <segmentation.hpp>
#include <img_debug.hpp>
#include <debug.hpp>


#define MAX_ZEROS 4 

using namespace std; 
using namespace cv; 

MatPart openImg(string path) 
{
	Mat img = imread(path, 0);

	//threshold(img, binimg, 0, 1, THRESH_BINARY_INV);
	return genMatPartFromMat(img); 
}

vector<Element> process(MatPart img) 
{
	cout << "_______________________________________\n";
	divideIntoBlocks(img, DIVIDE_BY_ROWS);
	vector<MatPart> parts; 
	getEndParts(img, parts); 
	
	vector<Element> elements; 

	for (int i = 0; i<parts.size(); i++) 
	{	
		// Classify it as text/table/drawing/unknown 
		if (recognizeText(parts.at(i))) 
		{
			cout << "text\n\n"; 
			Element e(parts.at(i), ELEMENT_TYPE_TEXT);
			elements.push_back(e);
		}
		else if (recognizeTable(parts.at(i), 32)) 
		{
			cout << "table\n\n";
			Element e(parts.at(i), ELEMENT_TYPE_TABLE); 
			elements.push_back(e);
		}
		else if (recognizeDrawing(parts.at(i))) 
		{
			cout << "drawing\n\n";
			Element e(parts.at(i), ELEMENT_TYPE_DRAWING);
			elements.push_back(e);
		}
		else 
		{
			cout << "unknown\n\n"; 
			Element e(parts.at(i), ELEMENT_TYPE_UNKNOWN); 
			elements.push_back(e);
		}
	}
	return elements; 
}

char digit_to_char(int d) 
{
	return (char) ('0' + (char) d);
}

int digits_num(int n) 
{
	int c = 0; 
	while (n>0) 
	{
		n /= 10; 
		c++;
	}
	return c; 
}

string string_represent(int n) 
{
	string s; 
	int digits = digits_num(n);
	int zeros = MAX_ZEROS - digits; 
	for (int i = 0; i<zeros; i++) s.push_back('0'); 
	while (n>0) 
	{
		s.push_back(digit_to_char(n%10));
		n /= 10;
	}
	s.append(".png");
	return s; 
}

void test_all() 
{
	/*
	string parts[16] = {"parts/0.png", 
			 "parts/1.png",
			 "parts/2.png",
			 "parts/3.png",
			 "parts/4.png",
			 "parts/5.png",
			 "parts/6.png",
			 "parts/7.png",
			 "parts/8.png",
			 "parts/9.png",
			 "parts/10.png",
			 "parts/11.png",
			 "parts/12.png",
			 "parts/13.png",
			 "parts/14.png",
			 "parts/15.png"
			};
	
	cout << "Enter number of page: "; 
	int i; 
	cin >> i;*/
	for (int i = 0; i<20; i++) 
	{
		MatPart img = openImg("test-parts/" + string_represent(i));
		vector<Element> elements = process(img);
		Mat page = imread("test-parts/" + string_represent(i));
		for (int k = 0; k<elements.size(); k++) 
		{
			MatPart r = elements.at(k).getMatPart(); 
			int type = elements.at(k).elementType(); 
			if (type == ELEMENT_TYPE_TEXT) 
			{
				rectangle(page, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(255, 0, 0), 2);
			}
			else if (type == ELEMENT_TYPE_TABLE) 
			{
				rectangle(page, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(0, 255, 0), 2);
			}
			else if (type == ELEMENT_TYPE_DRAWING) 
			{
				rectangle(page, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(0, 0, 255), 2);
			}
			else 
			{
				rectangle(page, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(0, 0, 0), 2);
			}
		}
		namedWindow("win"); 
		imshow("win", page); 
		waitKey(0); 
	}
}

void test_single() 
{
	MatPart img = openImg("/home/stefan/ss.png");
	vector<Element> elements = process(img);
	for (int i = 0; i<elements.size(); i++) 
	{
		int type = elements.at(i).elementType();
		switch (type) 
		{
			case ELEMENT_TYPE_TEXT: 
				cout << "text\n";
				break; 
			case ELEMENT_TYPE_TABLE: 
				cout << "table\n"; 
				break; 
			case ELEMENT_TYPE_DRAWING: 
				cout << "drawing\n"; 
				break; 
			default: 
				cout << "unknown\n"; 
		}
	}
}

int main(int argc, char* argv[]) 
{

	test_all(); 
	//test_single();

	return 0; 
	
}
