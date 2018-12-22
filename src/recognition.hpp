#pragma once 

#include "segment.hpp"
#include "page.hpp"


class Recognition 
{
public:
    virtual SegmentType recognize(Page, Segment) =0; 
};