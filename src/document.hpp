#pragma once 

#include <vector>

#include "page.hpp"
#include "document_union.hpp"
class Document 
{
public:

    virtual std::vector<Page> get_pages() = 0;

    DocumentUnion operator+(std::shared_ptr<Document> a) 
    {
        return DocumentUnion(this, a);
    }
};