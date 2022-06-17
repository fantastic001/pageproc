#!/bin/bash

python -m pdfproc --segment "/home/stefan/data/Documents/Library/Papers/Computer Science/Computer graphics/J. Olsen - Realtime procedural terrain generation (2004).pdf"  --output data --vertical-whitespace-tolerance 20 --horizontal-whitespace-tolerance 20 --depth 2 --leaf-only

# Assign proper labels 
python -m segment_classify assign --csv data.csv --data data/

