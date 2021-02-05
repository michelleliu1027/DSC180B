import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from sklearn import linear_model
from sklearn.linear_model import LogisticRegression
import warnings
warnings.filterwarnings('ignore')


def plot_correlation(battery5):
    fig, axs = plt.subplots(4,figsize=(15,15))
    battery5_cp = battery5.set_index('Time').drop('Battery Tag',axis =1)
    for i in range(len(axs)):
        if i == 3:
            axs[i].plot(battery5_cp.index[1:], battery5_cp[battery5_cp.columns[i]][1:], label=battery5_cp.columns[i])
            axs[i].legend(loc="upper right")
        else:
            axs[i].plot(battery5_cp.index, battery5_cp[battery5_cp.columns[i]], label=battery5_cp.columns[i])
            axs[i].legend(loc="upper right")


def corr_matrix(battery5_cp):
    battery5_cpp = battery5_cp.copy()
    battery5_cpp['Battery estimatedTime (Seconds).'] = (battery5_cpp['Battery estimatedTime (Seconds).'])
    corr = battery5_cpp.corr()
    matrix = corr.style.background_gradient(cmap='coolwarm')
    return matrix 

def ridge(battery5_cpp):
    X, y = battery5_cpp[['Battery Current Capacity (mWh)', 'Battery Rate (mW)', 'Battery Voltage (mv)']], \
       battery5_cpp['Battery estimatedTime (Seconds).']
    ridge = linear_model.Ridge(alpha=.5)
    ridge.fit(X, y)
    ridge_score = ridge.score(X, y)
    print(ridge_score)
    return ridge_score


def logistic(battery5_cpp):
    X, y = battery5_cpp[['Battery Current Capacity (mWh)', 'Battery Rate (mW)', 'Battery Voltage (mv)']], \
        battery5_cpp['Battery estimatedTime (Seconds).']
    logistic = LogisticRegression(random_state=0).fit(X, y)
    logistic_score = logistic.score(X, y)
    print(logistic_score)
    return logistic_score
