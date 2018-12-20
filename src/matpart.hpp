
#ifndef PAGEPROC_MATPART_HPP 
#define PAGEPROC_MATPART_HPP

#include <core/core.hpp>
#include <vector> 
#include <string> 

class MatPart 
{

	cv::Mat mat; 
	int heoght, width, x,y; 
	std::vector<MatPart> childs; 

public: 
	
	MatPart();
	MatPart(cv::Mat); 
	MatPart(const MatPart&); 

	int getHeight(); 
	int getWidth(); 
	int getX(); 
	int getY();
	cv::Mat getMat();

	std::vector<MatPart> getChilds(); 
	void addChild(MatPart); 
	std::vector<MatPart> getEndParts(); 

	std::vector<int> getRowHist();
	std::vector<int> getColHist();

	void saveToFile(std::string); 
};

#endif // PAGEPROC_MATPART_HPP
