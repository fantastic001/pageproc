#pragma once 

#include "document.hpp"

#include <memory>

class DocumentUnion : public Document
{
    std::shared_ptr<Document> first;
    std::shared_ptr<Document> second; 
public:
    explicit DocumentUnion(std::shared_ptr<Document> first_, std::shared_ptr<Document> second_)
        : first(first_), second(second_) {}
    std::vector<Page> get_pages() 
    {
        auto pages1 = first->get_pages();
        auto pages2 = second->get_pages();
        pages1.insert(pages1.end(), pages2.begin(), pages2.end());
        return pages1;
    }
    std::shared_ptr<Document> get_first(){return first;}
    std::shared_ptr<Document> get_second() {return second;}
};