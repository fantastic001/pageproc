
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
    ).apply(resize_img)


def preprocess(df: pd.DataFrame, datadir):
    img = (datadir + "/" + df.page + "/" + df.img_path).transform(load_img)
    print("Extracting image pixels")
    columns = []
    for c in range(BLOCK_SIZE):
        column = "%d" % c
        df["h_%s" % column] = img.apply(lambda x: x[:, c].mean())
        df["v_%s" % column] = img.apply(lambda x: x[c, :].mean())
        columns.append("h_%s" % column)
        columns.append("v_%s" % column)
    return df[["id", "page", "type_guess", 'img_path', 'text', 'parent_id', *columns]]