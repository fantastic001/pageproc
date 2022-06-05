
import pandas as pd 
import numpy as np 
import PIL.Image as I
from skimage.transform import resize
from functools import partial

BLOCK_SIZE = 50

resize_img = partial(resize, output_shape = (BLOCK_SIZE, BLOCK_SIZE))


def load_img(path: str):
    img = path.transform(I.open)
    return img.transform(np.asarray).apply(
        lambda x: x[:, :, [0,1,2]].mean(axis=2)
    ).apply(resize_img).apply(partial(np.reshape, newshape=(1, -1)))

def get_pixel_columns(a, i):
    return a[0, i]

def preprocess(df: pd.DataFrame, datadir):
    img = (datadir + "/" + df.page + "/" + df.img_path).transform(load_img)
    print("Extracting image pixels")
    columns = []
    for c in range(BLOCK_SIZE**2):
        column = "%d" % c
        df[column] = img.map(partial(get_pixel_columns, i=c))
        columns.append(column)
    return df[["id", "page", *columns]]