#!/bin/bash

python -m pdfproc --segment "/home/stefan/data/Documents/Library/Papers/Computer Science/Computer graphics/J. Olsen - Realtime procedural terrain generation (2004).pdf"  --output data --vertical-whitespace-tolerance 20 --horizontal-whitespace-tolerance 20 --depth 2 --leaf-only

# Assign proper labels 
python -m segment_classify assign --csv data.csv --data data/

# python -m segment_classify train --data-dir data --output model.json --classifier neural_network
# python -m segment_classify classify --data-dir data --model model.json
# python -m segment_classify score --data-dir data/