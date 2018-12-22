#pragma once 
#include "document.hpp"
#include <memory>
class DocumentReader 
{
public:
    virtual std::shared_ptr<Document> read() =0;
};