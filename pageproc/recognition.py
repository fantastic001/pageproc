from tkinter import Y
import poppler
from pageproc.segmentation import PageSegment
import subprocess

def recognize_text(filename, page_index, segment: PageSegment):
    proc = subprocess.Popen([
        "pdftotext",
        filename, 
        "/dev/stdout", 
        "-x", 
        str(segment.x),
        "-y",
        str(segment.y),
        "-H", 
        str(segment.height), 
        "-W", 
        str(segment.width), 
        "-r", "300", "-f", str(page_index+1), "-l", str(page_index+1)
    ], stdout=subprocess.PIPE)
    out, _ = proc.communicate()
    return out.decode("utf-8")