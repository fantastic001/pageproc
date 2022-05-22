
import poppler
from pageproc.segmentation import PageSegment

def recognize_text(document, page_index, segment: PageSegment):
    # return segment.page.text(poppler.Rectangle(segment.x, segment.y, segment.width, segment.height), poppler.TextLayout.)
    # return document.create_page(page_index).text(poppler.Rectangle(segment.x, segment.y, segment.width, segment.height), poppler.TextLayout.physical_layout)
    return segment.page.text()
