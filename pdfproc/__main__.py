import os
import json
import shutil

import PIL 
from pageproc import * 
import sys 
import argparse
from pageproc.segmentation import * 
from pageproc.recognition import * 

import matplotlib.pyplot as plt 

parser = argparse.ArgumentParser()
parser.add_argument("--segment", action="store")
parser.add_argument("--output", action="store")

args, rest = parser.parse_known_args(sys.argv)
print(args)


def get_parent_id(segments, child):
    candidates = [i for i, s in enumerate(segments) if child.parent is s] 
    if len(candidates) == 0:
        return None
    else:
        return candidates[0]

def guess_type(segment):
    return "dummy"

def segment(document, page_index, page, image, page_dir):
    segment = PageSegment(page, image)
    segments = [segment] + segment.segment(True, 0.3, deepdive=True)
    for i, s in enumerate(segments):
        imgpath = os.path.join(page_dir, "segment_%d.png" % i)
        PIL.Image.fromarray(s.img).save(imgpath, "PNG")
    return {
        "shape": segment.img.shape,
        "segment_count": len(segments),
        "segments": [
            {
                "text": recognize_text(doc, page_index, s),
                "x": int(s.x),
                "y": int(s.y),
                "width": int(s.width),
                "height": int(s.height),
                "img_path": "segment_%d.png" % i,
                "id": i,
                "parent_id": get_parent_id(segments, s),
                "type_guess": guess_type(s)
            } for i, s in enumerate(segments)
        ]
    }
    
doc = load_pdf(args.segment)
num = 0 
for page, image in load_pages(doc):
    page_dir = os.path.join(args.output, "page_%d" % num)
    os.makedirs(page_dir, exist_ok=True)
    image.save(os.path.join(page_dir, "%d.png" % num), "PNG")
    with open(os.path.join(page_dir, "data.json"), "w") as f:
        f.write(json.dumps(segment(doc, num, page, image, page_dir)))
    num += 1 