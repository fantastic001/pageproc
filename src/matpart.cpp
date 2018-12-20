
#include "matpart.hpp" 

using namespace cv; 
using namespace std; 

MatPart::MatPart() 
{
	heoght = 0; 
	width = 0; 
	x = 0; 
	y = 0; 
}

MatPart::MatPart(Mat m) 
{
	mat = m;
	x = 0; 
	y = 0; 
	height = m.cols; 
	width = m.rows;
}
	
MatPart::MatPart(MatPart part) 
{
	x = part.getX();
	y = part.getY();
	width = part.getWidth();
	height = part.getHeight();
	mat = part.getMat();
	
}

int MatPart::getHeight() 
{
	return height;
}
	
int MatPart::getWidth() 
{
	return width;
}

int MatPart::getX() 
{
	return x; 
}

int MatPart::getY() 
{
	return y; 
}

Mat MatPart::getMat() 
{
	return mat; 
}

std::vector<MatPart> MatPart::getChilds() 
{
	return childs; 
}

void MatPart::addChild(MatPart part) 
{
	childs.push_back(part);
}

std::vector<MatPart> MatPart::getEndParts() 
{
	vector<MatPart> ends; 
	if (childs.empty()) 
	{
		ends.push_back(MatPart(*this));
	}
	else 
	{
		for (int i = 0; i<childs.size(); i++) 
		{
			vector<MatPart> tmp = childs[i].getEndParts();
			for (int j = 0; j<tmp.size(); j++) 
			{
				ends.push_back(tmp[j]);
			}
		}
	}
	return ends; 
}

std::vector<int> MatPart::getRowHist() 
{
	vector<int> sum; 
	for (int i = 0; i<mat.rows; i++) 
	{
		int z = 0; 
		for (int j = 0; j<mat.cols; j++) 
		{
			z = z + (int) mat.data[mat.step[0]*i + mat.step[1]*j];
		}
		sum.push_back(z / img.cols);
	}
	return sum;
}
std::vector<int> getColHist() 
{
	
	vector<int> sum; 
	for (int i = 0; i<mat.cols; i++) 
	{
		int z = 0; 
		for (int j = 0; j<mat.rows; j++) 
		{
			z = z + (int) mat.data[mat.step[0]*j + mat.step[1]*i];
		}
		sum.push_back(z / mat.rows);
	}
	return sum;
}

void MatPart::saveToFile(std::string filename) 
{
	imwrite(filename, mat);
}
