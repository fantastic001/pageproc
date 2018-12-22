#pragma once 
#include <memory>
#include "page.hpp"
#include "segmented_page.hpp"
class Segmentation 
{
public:
    virtual std::shared_ptr<SegmentedPage> segment(std::shared_ptr<Page>) =0;
};