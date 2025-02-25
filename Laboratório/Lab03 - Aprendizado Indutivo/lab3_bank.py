#pip install 
#pip install matplotlib
#pip install pandas

import pandas as pd
import numpy as np
from sklearn import tree, metrics
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import LabelEncoder
import matplotlib.pyplot as plt
from scipy.io import arff


data,meta = arff.loadarff('./bank.arff')

le = LabelEncoder()

attributes = meta.names()
data_value = np.asarray(data)

age = np.asarray(data['age']).reshape(-1,1)
job = le.fit_transform(data['job']).reshape(-1,1)
marital = le.fit_transform(data['marital']).reshape(-1,1)
education = le.fit_transform(data['education']).reshape(-1,1)
default = le.fit_transform(data['default']).reshape(-1,1)
average = np.asarray(data['average']).reshape(-1,1)
housing = le.fit_transform(data['housing']).reshape(-1,1)
loan = le.fit_transform(data['loan']).reshape(-1,1)
contact = le.fit_transform(data['contact']).reshape(-1,1)
day = np.asarray(data['day']).reshape(-1,1)
month = le.fit_transform(data['month']).reshape(-1,1)
duration = np.asarray(data['duration']).reshape(-1,1)
campaign = np.asarray(data['campaign']).reshape(-1,1)
pdays = np.asarray(data['pdays']).reshape(-1,1)
previous = np.asarray(data['previous']).reshape(-1,1)
poutcome = le.fit_transform(data['poutcome']).reshape(-1,1)

features = np.concatenate((age, job, marital, education, default, average, housing, loan, contact, day, month, duration, campaign, pdays, previous, poutcome),axis=1)
target = data['subscribed']


Arvore = DecisionTreeClassifier(criterion='entropy').fit(features, target)

plt.figure(figsize=(10, 6.5))
tree.plot_tree(Arvore,feature_names=['age', 'job', 'marital', 'education', 'default', 'average', 'housing', 'loan', 'contact', 'day', 'month', 'duration', 'campaign', 'pdays', 'previous', 'poutocme'],class_names=['yes', 'no'],
                   filled=True, rounded=True)
plt.show()

fig, ax = plt.subplots(figsize=(25, 10))
metrics.ConfusionMatrixDisplay.from_estimator(Arvore,features,target,display_labels=['yes', 'no'], values_format='d', ax=ax)

plt.show()