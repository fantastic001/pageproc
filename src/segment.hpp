#pragma once 

class Segment 
{
    double x,y,w,h;
public:
    Segment(double x_, double y_, double w_, double h_) : x(x_), w(w_), y(y_), h(h_) {}
};

enum SegmentType {TEXT, IMAGE, MATH, TABLE, BLANK, UNKNOWN};