# pageproc

Recognize page elements by page bitmap analysis.

# Usage 

To do segmentation of PDF and put result into directory run:

    pdfproc --segment file.pdf --output dir/to/data

Output directory will contain:

* directories numbered as `page_1` to `page_N` for every page and inside it files `1.png` to `K.png` for every segment 
* `page_i/data.json` which is metadata info about segments 
<!-- * `bib.json` containing bibliography as follows:
    * `authors` - array of names of authors 
    * `year` - year of publication
    * `field` 
    * `conference`
    * `place`
    * `email`
    * `issue` -->

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
<!-- 
# Extracting bibliography from ML-classified data for research papers

To train model:

    bib_extract train --data-dir </path/to/directories/containing/datadirs/for/documents> --output <model.json>

To extract and add bibliography for particular documents:

    bib_extract extract --data-dir </path/to/directories/containing/datadirs/for/documents> --model <model.json> -->


# Changing behaviour of tools 

Tools support the following options to change:

* segmentation behaviour and thresholds for `pdfproc`
* ML model to be trained and used for `segment_classify` along with special model parameters given as `PREPROCESS_*` and `MODEL_*` envvars
<!-- * Model for `bib_extract` -->


# Segmentation 

For segmentation, xy-cuts algorithm is used which is documented in this paper: https://www.haralick.org/conferences/71280952.pdf

# Preprocessing and ML models 

User can chose different preprocessors and ML models to use. Every segment is preprocessed and transformed by specified 
preprocessor function during training and prediction phases. There are several preprocessors available:

| Preprocessor name | Description                                         |
|-------------------|-----------------------------------------------------|
| `simple`            | This preprocessor does nothing to segment. All data will be used from data file as input to model |
| `img_attrs`         | Every segment is transformed to vector [image area, width to area ratio, height / width, mean value of grayscale values of pixels] |
| `pixels`            | Image resized to some predefined size and transformed to grayscale and flattened and values of such output are used as input of model | 
| `histogram`         | Image is transformed to grayscaled, resized to predefined value and ixels are summed for each column and each row, so we get vector (sum(c1), sum(c2), ..., sum(ck), sum(r1), ..., sum(rk)) which is input to model |

These preprocessed segments are fed into specific ML model which can be: 

| Model | Description | 
|-------|-------------|
| 1-rule | One rule model | 
| Neural network | neural network model | 
| RF | Random forect |

# Development and testing of models 

To setup development env and run model tests first create virtualenv:

    virtualenv env
    . env/bin/activate
    pip install -r requirements.txt

For testing, there is one paper which is used and for which there are pre-labeled data to score accuracy of model. 

Please download J. Olsen - Realtime procedural terrain generation (2004) paper and save it on your local machine. 

After saving it, export env ariable to its path:

    export SAMPLE_PDF=/path/to/paper/pdf.pdf

Run all tests and generate reports:

    ./bin/testAll.sh sample_configurations/ reports

# Results 

In the following table, results are shown depending on which preprocessor was used and which model:

|    | preprocessor   | model    |   accuracy |
|---:|:---------------|:---------|-----------:|
|  1 | `histogram`      | RF       |     71.792 |
|  2 | `img_attrs`      | RF       |     72.034 |
|  3 | `img_attrs`      | one_rule |     63.153 |
|  4 | `pixels`         | NN       |     38.451 |
|  5 | `simple`         | NN       |     42.345 |
|  6 | `simple`        | RF       |     70.422 |
|  7 | `simple`         | one_rule |     63.216 |


From results we can see that random forest, when used with just image attributes as its input can achieve accuracy of 72%. 

