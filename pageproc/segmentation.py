import itertools
from tracemalloc import stop
from typing import List, Tuple
import numpy as np 


def get_numpy_array_from_img(image) -> np.array: 
    result = np.array(image.memoryview(), dtype=np.uint8, copy=False)
    return result

def _binarize(arr: np.array) -> np.array:
    arr = arr.astype(int)
    grayscale = (arr[:, :, 0] + arr[:,:,1] + arr[:,:,2]) / 3 
    x =  (grayscale < 255).astype(int)
    return x

def xy_cut(sum: List[int], max_whitespace) -> List[int]:
    # state = 1 we are on white area, state = 0 means we are on non-totally white area
    state = 1 
    c = 0 
    result = [] 
    if len(sum) == 0: return []
    for i, p in enumerate(sum):
        if p == 0 and state == 1: c += 1 
        elif p == 1 and state == 0: c += 1 
        elif p == 0 and state == 0: 
            result.append(i-c)
            result.append(i)
            c = 1 
            state = 1 
        elif p == 1 and state == 1:
            if c >= max_whitespace:
                c = 1 
                state = 0
    for i, p in enumerate(sum[:7]):
        if p == 1:
            if 0 not in result: result.append(0)
    if sum[-1] == 0: result.append(len(sum) - c)
    result = sorted(result)
    return result 


class PageSegment:
    def __init__(self, page, image, y=None, x=None, height=None, width=None, parent=None) -> None:
        self.image = image
        if y is None:
            y = 0 
        if x is None:
            x = 0 
        if height is None:
            height = self.image.height
        if width is None:
            width = self.image.width
        self.page = page 
        self.x = x
        self.y = y 
        self.width = width
        self.height = height
        img: np.array = get_numpy_array_from_img(image)[y:y+height, x:x+width]
        self.v_hist = np.sum(_binarize(img), axis=1) > 0
        self.h_hist = np.sum(_binarize(img), axis=0) > 0
        self.parent = parent
    
    def get_histogram(self, vertical=True) -> np.array:
        if vertical:
            return self.v_hist.astype(int)
        else:
            return self.h_hist.astype(int)

    def segment(self, vertical, depth=0):
        MAX_WHITESPACE_ROWS = 10
        MAX_WHITESPACE_COLS = 15
        # print("Sums: ", ", ".join([str(x) for x in sums]))
        result = xy_cut(self.get_histogram(vertical=vertical), MAX_WHITESPACE_ROWS if not vertical else MAX_WHITESPACE_COLS)
        # print("Result for segment: ", result)       
        children =  [
            PageSegment(
                self.page, 
                self.image, 
                self.y if not vertical else self.y + start, 
                self.x if vertical else self.x + start, 
                self.height if not vertical else stop-start,
                self.width if vertical else stop-start,
                self
            ) for start, stop in zip(result[::2], result[1::2])
        ]
        if depth == 0:
            return children
        else:
            result = children 
            for child in children:
                result.extend(child.segment(not vertical, depth-1))
            return result 
            # return list(itertools.chain(children, *[child.segment(not vertical, depth-1) for child in children]))