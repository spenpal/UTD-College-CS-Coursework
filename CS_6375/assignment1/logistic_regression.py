# logistic_regression.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# personal and educational purposes provided that (1) you do not distribute
# or publish solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UT Dallas, including a link to http://cs.utdallas.edu.
#
# This file is part of Programming Assignment 1 for CS6375: Machine Learning.
# Nikhilesh Prabhakar (nikhilesh.prabhakar@utdallas.edu),
# Athresh Karanam (athresh.karanam@utdallas.edu),
# Sriraam Natarajan (sriraam.natarajan@utdallas.edu),
#
#
# INSTRUCTIONS:
# ------------
# 1. This file contains a skeleton for implementing a simple version of the
# Logistic Regression algorithm. Insert your code into the various functions
# that have the comment "INSERT YOUR CODE HERE".
#
# 2. Do NOT modify the classes or functions that have the comment "DO NOT
# MODIFY THIS FUNCTION".
#
# 3. Do not modify the function headers for ANY of the functions.
#
# 4. You may add any other helper functions you feel you may need to print,
# visualize, test, or save the data and results.

#  Team
#  Gautam Sapre (gss170001)
#  Sanjeev Penupala (sxp170022)


import pickle

import matplotlib.pyplot as plt
import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score


class SimpleLogisiticRegression:
    """
    A simple Logisitc Regression Model which uses a fixed learning rate
    and Gradient Ascent to update the model weights
    """

    def __init__(self):
        self.w = []

    def initialize_weights(self, num_features):
        # DO NOT MODIFY THIS FUNCTION
        w = np.zeros((num_features))
        return w

    def compute_loss(self, X, y):
        """
        Compute binary cross-entropy loss for given model weights, features, and label.
        :param w: model weights
        :param X: features
        :param y: label
        :return: loss
        """
        X = np.hstack((X, np.ones((X.shape[0], 1))))  # add bias term
        y_proba = self.sigmoid(np.dot(X, self.w))
        loss = -np.mean(y * np.log(y_proba) + (1 - y) * np.log(1 - y_proba))
        return loss

    def sigmoid(self, val):

        """
        Implement sigmoid function
        :param val: Input value (float or np.array)
        :return: sigmoid(Input value)
        """
        return 1 / (1 + np.exp(-val))

    def gradient_ascent(self, w, X, y, lr):

        """
        Perform one step of gradient ascent to update current model weights.
        :param w: model weights
        :param X: features
        :param y: label
        :param lr: learning rate
        Update the model weights
        """
        w -= lr * (np.dot(X.T, self.sigmoid(np.dot(X, w)) - y)) / len(y)
        return w

    def fit(self, X, y, lr=0.1, iters=100, recompute=True):
        """
        Main training loop that takes initial model weights and updates them using gradient descent
        :param w: model weights
        :param X: features
        :param y: label
        :param lr: learning rate
        :param recompute: Used to reinitialize weights to 0s. If false, it uses the existing weights Default True

        NOTE: Since we are using a single weight vector for gradient ascent and not using
        a bias term we would need to append a column of 1's to the train set (X)
        """
        X = np.hstack((X, np.ones((X.shape[0], 1))))
        if recompute:
            # Reinitialize the model weights
            self.w = self.initialize_weights(X.shape[1])

        for _ in range(iters):
            self.w = self.gradient_ascent(self.w, X, y, lr)

    def predict_example(self, x):
        """
        Predicts the classification label for a single example x using the sigmoid function and model weights for a binary class example
        :param w: model weights
        :param x: example to predict
        :return: predicted label for x
        """
        x = np.hstack((x, np.ones((x.shape[0], 1))))
        yp = self.sigmoid(np.dot(x, self.w))
        yp[yp > 0.5] = 1
        yp[yp <= 0.5] = 0
        return yp

    def compute_error(self, y_true, y_pred):
        """
        Computes the average error between the true labels (y_true) and the predicted labels (y_pred)
        :param y_true: true label
        :param y_pred: predicted label
        :return: error rate = (1/n) * sum(y_true!=y_pred)
        """
        error_rate = (1 / len(y_true)) * sum(y_true != y_pred)
        return error_rate


if __name__ == "__main__":

    # Load the training data
    M = np.genfromtxt(
        "./data/monks-3.train",
        missing_values=0,
        skip_header=0,
        delimiter=",",
        dtype=int,
    )
    ytrn = M[:, 0]
    Xtrn = M[:, 1:]

    # Load the test data
    M = np.genfromtxt(
        "./data/monks-3.test", missing_values=0, skip_header=0, delimiter=",", dtype=int
    )
    ytst = M[:, 0]
    Xtst = M[:, 1:]

    lr = SimpleLogisiticRegression()

    # Part 1) Compute Train and Test Errors for different number of iterations and learning rates
    best_train_err = best_test_err = float("inf")
    best_iter, best_lr = 10, 0.01
    for iter in [10, 100, 1000, 10000]:
        for a in [0.01, 0.1, 0.33]:
            lr.fit(Xtrn, ytrn, a, iter)
            train_err = lr.compute_error(ytrn, lr.predict_example(Xtrn))
            test_err = lr.compute_error(ytst, lr.predict_example(Xtst))
            print(
                f"Iterations: {iter}, Learning Rate: {a}, Train Error: {train_err}, Test Error: {test_err}"
            )

            if test_err < best_test_err:
                best_train_err = train_err
                best_test_err = test_err
                best_lr = a
                best_iter = iter

    print()
    print(f"Best Parameters: Iter={best_iter}, LR={best_lr}")
    print(f"Our Best Model Train Error: {best_train_err}")
    print(f"Our Best Model Test Error: {best_test_err}")

    # Part 2) Retrain Logistic Regression on the best parameters and store the model as a pickle file
    # Code to store as pickle file
    lr.fit(Xtrn, ytrn, best_lr, best_iter)
    netid = "gss170001_sxp170022"
    file_pi = open("{}_model_1.obj".format(netid), "wb")  # Use your NETID
    pickle.dump(lr, file_pi)

    # Part 3) Compare your model's performance to scikit-learn's LR model's default parameters
    model = LogisticRegression()
    model.fit(Xtrn, ytrn)

    train_preds = model.predict(Xtrn)
    train_err = 1 - accuracy_score(ytrn, train_preds)

    test_preds = model.predict(Xtst)
    test_err = 1 - accuracy_score(ytst, test_preds)

    print()
    print(f"Sklearn Train Error: {train_err}")
    print(f"Sklearn Test Error: {test_err}")

    # Part 4) Plot curves on train and test loss for different learning rates. Using recompute=False might help
    for a in [0.01, 0.1, 0.33]:
        lr.fit(Xtrn, ytrn, lr=a, iters=1)
        train_losses = []
        test_losses = []
        for i in range(10):
            lr.fit(Xtrn, ytrn, lr=a, iters=100, recompute=False)
            train_loss = lr.compute_loss(Xtrn, ytrn)
            test_loss = lr.compute_loss(Xtst, ytst)
            train_losses.append(train_loss)
            test_losses.append(test_loss)
        plt.plot(range(1, 11), train_losses, label="train_loss, lr=" + str(a))
        plt.plot(range(1, 11), test_losses, label="test_loss, lr=" + str(a))
        plt.xlabel("epoch")
        plt.ylabel("loss")
        plt.legend()
        plt.show()
