
import pandas as pd 
import numpy as np 
from sklearn.dummy import DummyClassifier
from sklearn.ensemble import RandomForestClassifier

DROP_COLS = ["page", "id", "type", "type_guess", "img_path", "text", "parent_id"]

class MyRF(RandomForestClassifier):
    def predict(self, X):
        X["type"] = X["type_guess"]
        return super().predict(X.drop(DROP_COLS, axis=1))

def train(df: pd.DataFrame):
    df["type"] = df["type_guess"]
    return {
        "y": df["type_guess"].to_list(),
        "X": df.drop(DROP_COLS, axis=1).to_numpy().tolist()
    }

def load(model: dict):
    clf = MyRF(max_depth=2, random_state=0)
    X = model["X"]
    y = model["y"]
    clf.fit(X, y)
    return clf 
