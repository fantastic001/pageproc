#ifndef PAGEPROC_CLASSIFICATOR_HPP 
#define PAGEPROC_CLASSIFICATOR_HPP

#include "../element.hpp"
#include "../matpart.hpp"

class Classificator 
{

public:
	Classificator();
	
	/*
	Classifies given element 
	*/
	virtual Element classify(MatPart) = 0;
};

#endif // PAGEPROC_CLASSIFICATOR_HPP
