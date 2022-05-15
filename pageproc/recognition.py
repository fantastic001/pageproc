
import poppler
from pageproc.segmentation import PageSegment

def recognize_text(document, page_index, segment: PageSegment):
    # return segment.page.text(poppler.Rectangle(segment.x, segment.y, segment.width, segment.height), poppler.TextLayout.)
    return segment.page.text()
