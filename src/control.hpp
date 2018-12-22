#pragma once 

#include <memory>
#include <utility>
#include "document.hpp"
#include <string>
#include "document_reader.hpp"
#include "document_writer.hpp"
#include "segment.hpp"
#include "segmented_page.hpp"
#include "recognition.hpp"
#include "segmentation.hpp"
class Control
{
    template <class T, class U> using pairs = 
        std::vector<std::pair<std::shared_ptr<T>, U>>;
    pairs<Segmentation, std::string> segmentations; 
    pairs<Recognition, std::string> recognitions; 

protected:
    virtual void on_init() = 0;
    virtual void on_document_load(std::shared_ptr<Document>) =0; 
    virtual void on_page_load(std::shared_ptr<Page>)=0;
    virtual void on_segmented_page(std::shared_ptr<SegmentedPage>)=0;
    virtual void before_document_write(std::shared_ptr<Document>) =0;
    virtual void after_document_write(std::shared_ptr<Document>) =0;
    virtual void on_recognized_segment(std::shared_ptr<Segment>, SegmentType) =0;
    virtual void on_segmentation(std::shared_ptr<Segmentation>, std::string) =0;
    virtual void on_recognition(std::shared_ptr<Recognition>, std::string) =0; 
    virtual void on_writer(std::shared_ptr<DocumentWriter>, std::string) =0;
    virtual void on_reader(std::shared_ptr<DocumentReader>, std::string) =0; 
    virtual void on_error(std::string) =0; 
    virtual void on_info(std::string) =0; 
public:
    explicit Control() 
    {
        this->on_init();
    }
    void add_segmentation(std::shared_ptr<Segmentation>, std::string);
    void add_recognition(std::shared_ptr<Recognition>, std::string);
    void add_reader(std::shared_ptr<DocumentReader>, std::string);
    void add_writer(std::shared_ptr<DocumentWriter>, std::string);
    void  error(std::string);
    void info(std::string);
    void set_default_segmentation(int);
    void set_default_recognition(int);
    void set_default_reader(int);
    void set_default_wriiter(int);
    std::shared_ptr<SegmentedPage> segment();
    SegmentType recognize();
    void undo();
    void next_page();
    void previous_page();
    std::shared_ptr<Page> get_page();
    void save();
    void open();


};