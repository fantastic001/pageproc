
#include "element.hpp"

Element::Element(MatPart part_, int type_) 
{
	part = part_; 
	type = type_;
}

MatPart Element::getMatPart() 
{
	return part; 
}

int Element::elementType() 
{
	return type; 
}
