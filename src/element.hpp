
#ifndef PAGEPROC_ELEMENT_HPP
#define PAGEPROC_ELEMENT_HPP

#include "matpart.hpp" 

// Definitions for element type 
#define ELEMENT_TYPE_TEXT 0 
#define ELEMENT_TYPE_TABLE 1 
#define ELEMENT_TYPE_DRAWING 2 
#define ELEMENT_TYPE_UNKNOWN -1 


/*
 * Class which represents element (text, table, ...)
 * */
class Element 
{
	MatPart part; 
	int type; 

public: 
	/*
	 * Constructor 
	 *
	 * Arguments: 
	 * 	1: MatPart - part on the image 
	 * 	2: int - type of element which is recognized with recognize... functions, it can be: 
	 * 		* ELEMENT_TYPE_TEXT
	 * 		* ELEMENT_TYPE_TABLE
	 * 		* ELEMENT_TYPE_DRAWING
	 * 		* ELEMENT_TYPE_UNKNOWN
	 * */
	Element(MatPart, int);

	/*
	 * Get MatPart structure 
	 *
	 * Returns: MatPart - part of image assigned with this element 
	 * */
	MatPart getMatPart(); 

	/*
	 * Determines type of element 
	 *
	 * Returns: int - element typw (see constructor description for available types) 
	 * */
	int elementType();
};

#endif // PAGEPROC_ELEMENT_HPP
