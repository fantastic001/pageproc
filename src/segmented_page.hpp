#pragma once 
#include "page.hpp"
#include <memory>
#include <vector>
#include "segment.hpp"

class SegmentedPage : public Page 
{
    std::vector<std::shared_ptr<Segment>> segments; 
    std::shared_ptr<Page> page;
public:
    explicit SegmentedPage(std::shared_ptr<Page> page, std::vector<std::shared_ptr<segments>>) {
        this->page = page; 
        this->segments = segments;
    }
    std::vector<std::shared_ptr<Segment>> get_segments() {return segments;}
    std::string get_region_text(double x, double y, double width, double height) 
    {
        return page->get_region_text(x,y,width,height);
    }
    int get_height() 
    {
        return page->get_height();
    }
    int get_width()
    {
        return page->get_width();
    }
    MatPart get_image() 
    {
        return page->get_image();
    }
};