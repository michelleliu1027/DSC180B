from sklearn.linear_model import LinearRegression
from sklearn import svm
from sklearn.tree import DecisionTreeRegressor
from sklearn.metrics import mean_absolute_error
from sklearn.model_selection import train_test_split

# helper methods 

# calculates the mean absolute error of the given model
def mae(model):
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3)
    reg = model.fit(X_train, y_train)
    train_error = mean_absolute_error(y_train, reg.predict(X_train))
    test_error = mean_absolute_error(y_test, reg.predict(X_test))
    
    return train_error, test_error

# run a simulation to see how different the observed mean absolute error is from the simulation 
def simulation(model):
    errors = []
    for _ in range(1000):
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3)
        reg = model.fit(X_train, y_train)
        errors.append(mean_absolute_error(y_test, reg.predict(X_test)))
        
    return errors

# the features are number of processes, page faults, capacity, cpu percentage, and cpu temperature 
X = pd.concat([num_proc, page_faults, capacity, cpu_percent, cpu_temp], axis = 1).dropna()
y = battery_event[['guid', 'battery_minutes_remaining']][battery_event.guid.isin(X.index)].groupby('guid')['battery_minutes_remaining'].apply(lambda x: (x!=-1).mean())



##### Baseline Model #####
def linear_reg():
    # for our baseline model, we will use linear regression for calculating mean absolute error 
    linear_train, linear_test = mae(LinearRegression())
    print(linear_train, dt_test)
    
    linear_errors = simulation(LinearRegression())
    
    p_val = (linear_errors <= linear_train).mean()
    return p_val 



##### Improving Model #####
def svm():
    # to improve our baseline model, we will consider SVM for calculating mean absolute error 
    svm_train, svm_test = mae(svm.SVR())
    print(svm_train, svm_test)
    
    # next, to run a hypothesis testing, we will bootstrap 1000 times to simulate a distribution 
    # and see how different the observed mean absolute error is from the simulation
    svm_errors = simulation(svm.SVR())
    
    p_val = (svm_errors <= svm_train).mean()
    return p_val

    
def dtr():
    # This time, we will use decision tree regressor to calculate mean absolute error 
    dt_train, dt_test = mae(DecisionTreeRegressor(random_state=0))
    print(dt_train, dt_test)
    
    # next, to run a hypothesis testing, we will bootstrap 1000 times to simulate a distribution 
    # and see how different the observed mean absolute error is from the simulation
    dt_errors = simulation(DecisionTreeRegressor(random_state=0))
    
    p_val = (dt_errors <= dt_train).mean()
    return p_val



##### Overfitting ##### 
def overfitting():
    # though we got a p-value of 0 for decision tree regressor, from the large difference in mean absolute errors 
    # between train data and test data, 1.5274384551368124e-06 and 0.3092834478762173 respectively, we can clearly 
    # see that this model suffers from overfitting 

    # let's see if removing features would reduce the difference between the two mean absolute errors 
    # first, let's remove number of processes 
    X = pd.concat([page_faults, capacity, cpu_percent, cpu_temp], axis = 1).dropna()

    dt_train, dt_test = mae(DecisionTreeRegressor(random_state=0))
    print(dt_train, dt_test)

    # next, let's remove page faults
    X = pd.concat([num_proc, capacity, cpu_percent, cpu_temp], axis = 1).dropna()

    dt_train, dt_test = mae(DecisionTreeRegressor(random_state=0))
    print(dt_train, dt_test)

    # let's remove capacity
    X = pd.concat([num_proc, page_faults, cpu_percent, cpu_temp], axis = 1).dropna()

    dt_train, dt_test = mae(DecisionTreeRegressor(random_state=0))
    print(dt_train, dt_test)

    # let's remove cpu percentage
    X = pd.concat([num_proc, page_faults, capacity, cpu_temp], axis = 1).dropna()

    dt_train, dt_test = mae(DecisionTreeRegressor(random_state=0))
    print(dt_train, dt_test)

    # unfortunately, we can see that removing features does not really reduce the issue of overfitting since 
    # the difference between the 2 mean absolute errors is still pretty large 
    # in future works, we will need to think about how to tackle this issue of overfitting 

    
