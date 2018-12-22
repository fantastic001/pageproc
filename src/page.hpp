#pragma once 

#include <string>
#include <matpart.hpp>
class Page 
{
public:
    std::string get_text() {return get_region_text(0,0,1,1);}

    virtual std::string get_region_text(double x, double y, double width, double height) =0;
    virtual int get_height() =0; 
    virtual int get_width() =0;
    virtual MatPart get_image() =0;
};