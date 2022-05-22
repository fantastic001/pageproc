
import pandas as pd 
from sklearn.dummy import DummyClassifier

def train(df: pd.DataFrame):
    return df.to_dict()

def load(model: dict):
    df = pd.DataFrame(model)
    model = DummyClassifier()
    model.fit(df, df["type_guess"])
    return model 
