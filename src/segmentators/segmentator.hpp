
#ifndef PAGEPROC_SEGMENTATOR_HPP 
#define PAGEPROC_SEGMENTATOR_HPP


// defines 


#include <vector> 

#include "matpart.hpp"

class Segmentator 
{

public: 
	/*
	Performs segmentaton and recursively put segments into MatPart's childs attribute 
	*/
	virtual std::vector<MatPart> segmentate(MatPart&) = 0; 
	

	/*
	Returns smallest level childs from a given MatPart object (elements without deeper level of recursion - without childs)
	*/
	std::vector<MatPart> getEndParts(MatPart);
};

#endif // PAGEPROC_SEGMENTATOR_HPP
