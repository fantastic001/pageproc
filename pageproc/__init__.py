from typing import List
import poppler

def load_pages(doc):
    return [(doc.create_page(i), poppler.PageRenderer().render_page(doc.create_page(i), 300, 300)) for i in range(doc.pages)]
def load_pdf(filename):
    return poppler.load_from_file(filename)