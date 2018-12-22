#pragma once 

#include "document.hpp"
#include <memory>

class DocumentWriter 
{
public:
    virtual void write(std::shared_ptr<Document>) = 0;
};