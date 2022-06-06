from calendar import day_abbr
import itertools
import json
import os
import sys 
import argparse 
import importlib
import inspect

import pandas

modes = ["train", "classify", "label", "--help"] 


arguments = [] 
# we need to trim down argument list up to the group selected
for i, x in enumerate(sys.argv):
    if x in modes:
        arguments = sys.argv[i:]
        break

parser = argparse.ArgumentParser()
subparsers = parser.add_subparsers(dest="mode")

train_mode = subparsers.add_parser("train")
classify_mode = subparsers.add_parser("classify")
label_mode = subparsers.add_parser("label")

train_mode.add_argument("--data-dir", action="store")
train_mode.add_argument("--output", action="store")
train_mode.add_argument("--preprocessor", action="store", default="simple")
train_mode.add_argument("--classifier", action="store", default="one_rule")

classify_mode.add_argument("--data-dir", action="store")
classify_mode.add_argument("--model", action="store")

label_mode.add_argument("--preprocessor", default="simple", action="store")
label_mode.add_argument("--data-dir", action="store")
label_mode.add_argument("--exclude-attribute",nargs="*", default=[])

args = parser.parse_args(arguments)


def load_preprocessor_module(preprocessor):
    return importlib.import_module("segment_classify.preprocessors.%s" % preprocessor)

def load_classifier_module(classifier):
    return importlib.import_module("segment_classify.classifiers.%s" % classifier)

def load(data_dir):
    pages = os.listdir(data_dir)
    df = pandas.DataFrame(itertools.chain(*[
        [dict(x, page=page) for x in json.loads(open(os.path.join(data_dir, page, "data.json")).read())["segments"]]
        for page in pages]))
    return df

def bind_parameters_from_env(f, prefix: str):
    params = {}
    sig = inspect.signature(f)
    for name, p in sig.parameters.items():
        ptype = type(p.default)
        if ptype != type:
            params[name] = ptype(os.environ.get("%s_%s" % (prefix.upper(), name.upper()), p.default))
    return params

def preprocess(df, preprocessor, datadir) -> pandas.DataFrame:
    preprocess_module = load_preprocessor_module(preprocessor)
    f = preprocess_module.preprocess
    params = bind_parameters_from_env(f, "preprocess")
    print(params)
    return f(df, datadir, **params)



def train_model(data_dir, output, preprocessor, classifier):
    df = load(data_dir)
    df = preprocess(df, preprocessor, data_dir)
    print("Training samples: %d" % df.shape[0])
    classifier_module = load_classifier_module(classifier)
    open(output, "w").write(json.dumps({
        "preprocessor": preprocessor,
        "model": classifier_module.train(df, **bind_parameters_from_env(classifier_module.train, "model")),
        "classifier": classifier
    }))

def classify_data(data_dir, model):
    model_data = json.loads(open(model).read())
    model = model_data["model"]
    preprocessor = model_data["preprocessor"]
    df = load(data_dir)
    classifier = model_data["classifier"]
    classifier_module = load_classifier_module(classifier)
    data = preprocess(df, preprocessor, data_dir)
    classified = classifier_module.load(model).predict(data)
    print(classified)
    print("Classified samples: %d" % len(classified))
    df["type"] = classified
    print(df)
    for page, segments in df.groupby("page"):
        page_data_path = os.path.join(args.data_dir, page, "data.json")
        page_data = json.loads(open(page_data_path).read())
        page_data["segments"] = segments.to_dict("records")
        open(page_data_path, "w").write(json.dumps(page_data))


if args.mode == "train":
    print("Training mode: ", args)
    train_model(args.data_dir, args.output, args.preprocessor, args.classifier)
elif args.mode == "classify":
    print("Classification mode: ", args)
    classify_data(args.data_dir, args.model)
elif args.mode == "label":
    df = load(args.data_dir)
    print(preprocess(df, args.preprocessor, args.data_dir).drop(args.exclude_attribute, axis=1).to_csv())