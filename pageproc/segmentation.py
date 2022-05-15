import itertools
from tracemalloc import stop
from typing import List, Tuple
import numpy as np 


def get_numpy_array_from_img(image) -> np.array: 
    result = np.array(image.memoryview(), dtype=np.uint8)
    return result

def _binarize(arr: np.array) -> np.array:
    return (arr[:, :, 0] + arr[:,:,1] + arr[:,:,2]) / 3.0

def get_cuts(hist, threshold=0.10):
    return np.diff((hist < threshold).astype(int))

def get_stops(cuts):
    i = np.arange(cuts.shape[0])
    return (
        i[cuts == -1],
        i[cuts == 1]
    )

class PageSegment:
    def __init__(self, page, image, y=None, x=None, height=None, width=None, parent=None) -> None:
        self.image = image
        self.img: np.array = get_numpy_array_from_img(image)
        if y is None:
            y = 0 
        if x is None:
            x = 0 
        if height is None:
            height = self.img.shape[0]
        if width is None:
            width = self.img.shape[1]
        self.page = page 
        self.x = x
        self.y = y 
        self.width = width
        self.height = height
        self.img = self.img[y:y+height, x:x+width]
        self.parent = parent
    
    def get_histogram(self, vertical=True) -> np.array:
        hist: np.array
        kernel_size = 10
        kernel = np.ones(kernel_size) / kernel_size
        if vertical:
            hist = np.mean(_binarize(self.img), axis=1) / 255
        else:
            hist = np.mean(_binarize(self.img), axis=0) / 255
        return np.convolve(hist, kernel, mode="same")

    def segment(self, vertical, threshold=0.30, deepdive=False):
        starts, stops = get_stops(get_cuts(self.get_histogram(vertical=vertical), threshold=threshold))
        if starts.shape[0] > stops.shape[0]:
            starts = starts[:stops.shape[0]]
        else:
            stops = stops[:starts.shape[0]]
        print(starts)
        print(stops)
        children =  [
            PageSegment(
                self.page, 
                self.image, 
                self.y if not vertical else start, 
                self.x if vertical else start, 
                self.height if not vertical else stop-start,
                self.width if vertical else stop-start,
                self
            ) for start, stop in zip(starts, stops)
        ]
        if not deepdive:
            return children
        else:
            return list(itertools.chain(children, *[child.segment(not vertical, threshold+0.1, deepdive=True) for child in children]))