
import pandas as pd 
import numpy as np 
from sklearn.neural_network import MLPClassifier
from base64 import b64encode, b64decode
import pickle

DROP_COLS = ["page", "id", "type", "type_guess", "img_path", "text", "parent_id"]

def serialize_model(model):
    return b64encode(pickle.dumps(model)).decode("ascii")

def deserialize_model(data: str):
    return pickle.loads(b64decode(data.encode("ascii")))

class MyNN(MLPClassifier):
    def predict(self, X):
        X["type"] = X["type_guess"]
        return super().predict(X.drop(DROP_COLS, axis=1))

def train(df: pd.DataFrame):
    df["type"] = df["type_guess"]
    clf = MyNN()
    y = df["type_guess"]
    X = df.drop(DROP_COLS, axis=1)
    clf.fit(X, y)
    return {
        "data": serialize_model(clf)
    }

def load(model: dict):
    return deserialize_model(model["data"])
