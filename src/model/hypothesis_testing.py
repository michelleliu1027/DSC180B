import pandas as pd
import gzip
import numpy as np
import matplotlib.pyplot as plt
import warnings
import sys
from datetime import datetime


from sklearn.linear_model import LinearRegression
from sklearn import svm
from sklearn.tree import DecisionTreeRegressor
from sklearn.metrics import mean_absolute_error
from sklearn.model_selection import train_test_split

sys.path.insert(0, 'src/data')
from Loading_Data import * 
sys.path.insert(0, 'src/eda')
from feature_selection import * 

# Helper Method1
# calculates the mean absolute error of the given model
def mae(model, X_train, y_train, X_test, y_test):
    reg = model.fit(X_train, y_train)
    train_error = mean_absolute_error(y_train, reg.predict(X_train))
    test_error = mean_absolute_error(y_test, reg.predict(X_test))
    
    return train_error, test_error

# run a simulation to see how different the observed mean absolute error is from the simulation 
def simulation2(model1, model2, X, y):
    errors1 = []
    errors2 = []
    
    for _ in range(1000):
        X_train2, X_test2, y_train2, y_test2 = train_test_split(X, y, test_size=0.3)

        reg1 = model1.fit(X_train2, y_train2)
        reg2 = model2.fit(X_train2, y_train2)
        
        errors1.append(mean_absolute_error(y_test2, reg1.predict(X_test2)))
        errors2.append(mean_absolute_error(y_test2, reg2.predict(X_test2)))
        
        
    return (np.array(errors1) - np.array(errors2))

# # the features are number of processes, page faults, capacity, cpu percentage, and cpu temperature 
# def train_test_XY(num_proc, page_faults, capacity, cpu_percent, cpu_temp, num_devices,avg_memory,cpu_sec):

#     X = pd.concat([num_proc, page_faults, capacity, cpu_percent, cpu_temp, num_devices,avg_memory,cpu_sec], axis = 1).dropna()
#     y = battery_event[['guid', 'battery_minutes_remaining']][battery_event.guid.isin(X.index)].groupby('guid')['battery_minutes_remaining'].apply(lambda x: (x!=-1).mean())

#     X_train1, X_test1, y_train1, y_test1 = train_test_split(X, y, test_size=0.3)
#     return X,y, X_train1, X_test1, y_train1, y_test1



##### Baseline Model #####
def linear_reg(X_train1, y_train1, X_test1, y_test1):
    # for our baseline model, we will use linear regression for calculating mean absolute error 
    linear_train, linear_test = mae(LinearRegression(), X_train1, y_train1, X_test1, y_test1)
    print("linear training error: " + str(linear_train), "linear test error: " + str(linear_test))
    return linear_train, linear_test
   


##### Improving Model #####
def svm(X_train1, y_train1, X_test1, y_test1):
    # to improve our baseline model, we will consider SVM for calculating mean absolute error 
    svm_train, svm_test = mae(svm.SVR(), X_train1, y_train1, X_test1, y_test1)
    print("svm training error: " + str(svm_train), "svm test error: " + str(svm_test))
    return svm_train, svm_test

    
def dtr(X_train1, y_train1, X_test1, y_test1):
    # This time, we will use decision tree regressor to calculate mean absolute error 
    dt_train, dt_test = mae(DecisionTreeRegressor(), X_train1, y_train1, X_test1, y_test1)
    print("decision tree training error: " + str(dt_train), "decision tree test error: " + str(dt_test))
    return dt_train, dt_test
    
    
##### Hypothesis Testing #####

# Hypothesis Testing1
# Null Hypo: There's no difference in performance between SVM and Decision Tree Regressor
# Alternative Hypo: SVM performs better than Decision Tree Regressor
def hypo1(svm_test,dt_test):
    print("Null Hypo: There's no difference in performance between SVM and Decision Tree Regressor")
    print("Alternative Hypo: SVM performs better than Decision Tree Regressor")
    observed_svm_dt = svm_test - dt_test
    print("Observed difference between svm error and decision tree error: " + str(observed_svm_dt))
    
    diffa = simulation2(svm.SVR(), DecisionTreeRegressor(), X, y)
    p_dt_svm = 1-(diffa>observed_svm_dt).mean()
    print("p-value: " + str(p_dt_svm))
    
    
# Hypothesis Testing2
# Null Hypo: There's no difference in performance between SVM and Linear Regression
# Alternative Hypo: SVM performs better than Linear Regression
def hypo2(svm_test,linear_test):   
    print("Null Hypo: There's no difference in performance between SVM and Linear Regression")
    print("Alternative Hypo: SVM performs better than Linear Regression")
    observed_svm_lr = svm_test - linear_test
    print("Observed difference between svm error and linear error: " + str(observed_svm_lr))
    
    diff = simulation2(svm.SVR(), LinearRegression(), X, y)
    p_dt_lr = 1-(diff>observed_svm_lr).mean()
    print("p-value: " + str(p_dt_lr))
    
    

