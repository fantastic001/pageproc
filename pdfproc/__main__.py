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
parser.add_argument("--horizontal-whitespace-tolerance", action="store", default=15, type=int)
parser.add_argument("--vertical-whitespace-tolerance", action="store", default=15, type=int)
parser.add_argument("--leaf-only", action="store_true", default=False)
parser.add_argument("--depth", action="store", default=1, type=int)



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
    segments = [segment] + segment.segment(
        vertical=True, 
        depth=args.depth, 
        vertical_whitespace_tolerance=args.vertical_whitespace_tolerance, 
        horizontal_whitespace_tolerance=args.horizontal_whitespace_tolerance,
        leaf_only=args.leaf_only
    )
    for i, s in enumerate(segments):
        imgpath = os.path.join(page_dir, "segment_%d.png" % i)
        PIL.Image.fromarray(get_numpy_array_from_img(s.image)[s.y:s.y+s.height, s.x:s.x+s.width]).save(imgpath, "PNG")
    return {
        "height": segment.height,
        "width": segment.width,
        "segment_count": len(segments)-1,
        "segments": [
            {
                "text": recognize_text(args.segment, page_index, s),
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
pages = load_pages(doc)
for page, image in pages:
    print("\r  %d%% %s " % (num*100 // len(pages), "=" * (20*num // len(pages))), end="")
    page_dir = os.path.join(args.output, "page_%d" % num)
    os.makedirs(page_dir, exist_ok=True)
    image.save(os.path.join(page_dir, "%d.png" % num), "PNG")
    with open(os.path.join(page_dir, "data.json"), "w") as f:
        f.write(json.dumps(segment(doc, num, page, image, page_dir), indent=5, sort_keys=True))
    num += 1 