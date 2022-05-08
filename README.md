# pageproc

Recognize page elements by page bitmap analysis.

# Usage 

To do segmentation of PDF and put result into directory run:

    pdfproc --segment file.pdf --output dir/to/data

Output directory will contain:

* directories numbered as `page_1` to `page_N` for every page and inside it files `1.png` to `K.png` for every segment 
* `page_i/data.json` which is metadata info about segments 
* `bib.json` containing bibliography as follows:
    * `authors` - array of names of authors 
    * `year` - year of publication
    * `field` 
    * `conference`
    * `place`
    * `email`
    * `issue`

# Metadata file 

It contains the following info:

* `segments` - list of dictionaries with following data:
    * `text` - text inside segment if any 
    * `type_guess` - can be either `text`, `dummy`, `table` or `image`
    * `x` - segment x-coordinate on page 
    * `y` - segment y coordinate on page 
    * `width` - segment width 
    * `height` - segment height
    * `id` segment ID
    * `parent_id` - ID of segment containing given segment if any, null otherwise
    * `img_path` - path to image relative to this file

# Using ML model to classify segments 

To train model:

    segment_classify train --data-dir <path/to/output/of/pdfproc> --output <model-output-file.json>

To classify segments with ML model run:

    segment_classify classify --data-dir <path/to/output/of/pdfproc> --model <path/to/model/file.json>

NOTE: Classification command will modify data and insert `type` field for every segment 

# Extracting bibliography from ML-classified data for research papers

To train model:

    bib_extract train --data-dir </path/to/directories/containing/datadirs/for/documents> --output <model.json>

To extract and add bibliography for particular documents:

    bib_extract extract --data-dir </path/to/directories/containing/datadirs/for/documents> --model <model.json>


# Changing behaviour of tools 

Tools support the following options to change:

* segmentation behaviour and thresholds for `pdfproc`
* ML model to be trained and used for `segment_classify` along with special model parameters
* Model for `bib_extract`