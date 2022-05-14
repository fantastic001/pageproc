
#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"
#include <string>
#include <list>
#include <sstream>
#include <iostream>

#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page-renderer.h>

#include <segmentation.hpp>



using namespace std; 

string get_page_filename_by_index(int index) {
    stringstream ss; 
    ss  << index << ".png";
    return ss.str();
}

poppler::document* load_pdf(string filename) {
    cout << "Loading PDF " << filename << endl;
    return poppler::document::load_from_file(filename);
}

list<poppler::page*> get_pdf_pages(poppler::document* doc) {
    list<poppler::page*> result;
    for (int i = 0; i<doc->pages(); i++) {
        result.push_back(doc->create_page(i));
    }
    return result;
}

void render_pages(poppler::document* document, string outputdir) {
    auto pages = get_pdf_pages(document);
    int index = 0;
    for (auto page : pages) {
        cout << "Rendering page " << index << endl;
        auto img = poppler::page_renderer().render_page(page);
        img.save(outputdir + "/" + get_page_filename_by_index(index), "png", 500);
        MatPart part;
        part.height = img.height();
        part.width = img.width();
        divideIntoBlocks(part, DIVIDE_BY_COLUMNS);
        index++;
    }
    cout << "Rendering done\n";
}

int segment_pdf(string pdffilename, string outputdir) {
    render_pages(load_pdf(pdffilename), outputdir);
    cout << "Segmentation done\n";
    return 0; 
}

int main(int argc, char** argv) {
    CLI::App app("PDF processing tool");
    string pdffilename, outputdir; 

    app.add_option("--segment", pdffilename, "Segment given file to output directory");
    app.add_option("--output", outputdir, "Specify custom output directory");
    CLI11_PARSE(app, argc, argv);

    if (! pdffilename.empty()) {
        return segment_pdf(pdffilename, outputdir);
    }
    cout << "Document processed\n";
    return 0;
}