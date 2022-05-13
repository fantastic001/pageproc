
#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"
#include <string>

using namespace std; 

int segment_pdf(string pdffilename, string outputdir) {
    return 0;
}

int main(int argc, char** argv) {
    CLI::App app("PDF processing tool");
    string pdffilename, outputdir; 

    app.add_option("--segment", pdffilename, "Segment given file to output directory");
    app.add_option("--output", outputdir, "Specify custom output directory");

    if (! pdffilename.empty()) {
        return segment_pdf(pdffilename, outputdir);
    }

    CLI11_PARSE(app, argc, argv);
    return 0;
}