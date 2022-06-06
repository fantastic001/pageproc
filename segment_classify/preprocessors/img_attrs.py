
import pandas as pd 
import numpy as np 
import PIL.Image as I
import os 

def load_img(path: str):
    img = path.transform(I.open)
    return img.transform(np.asarray).apply(lambda x: x[:, :, [0,1,2]].mean(axis=2))


def preprocess(df: pd.DataFrame, datadir, standardize = False):
    img = (datadir + "/" + df.page + "/" + df.img_path).transform(load_img)
    df["area"] = df.height * df.width 
    df["width_to_area"] = df.width / df.area 
    df["height_to_width"] = df.height / df.width
    df["mean"] = img.apply(np.mean)
    if standardize:
        for col in ["area", "width_to_area", "height_to_width", "mean", "x", "y", "width", "height"]:
            df[col] = (df[col] - df[col].min()) / (df[col].max() - df[col].min())
    return df 