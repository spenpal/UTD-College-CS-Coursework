# naive_bayes.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# personal and educational purposes provided that (1) you do not distribute
# or publish solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UT Dallas, including a link to http://cs.utdallas.edu.
#
# This file is part of Programming Assignment 3 for CS6375: Machine Learning.
# Nikhilesh Prabhakar (nikhilesh.prabhakar@utdallas.edu),
# Athresh Karanam (athresh.karanam@utdallas.edu),
# Sriraam Natarajan (sriraam.natarajan@utdallas.edu),
#
#
# INSTRUCTIONS:
# ------------
# 1. This file contains a skeleton for implementing a simple version of the
# Naive Bayes algorithm. Insert your code into the various functions
# that have the comment "INSERT YOUR CODE HERE".
#
# 2. Do NOT modify the classes or functions that have the comment "DO NOT
# MODIFY THIS FUNCTION".
#
# 3. You may add any other helper functions you feel you may need to print,
# visualize, test, or save the data and results.
#
# 4. Make sure to save your model in a pickle file after you fit your Naive
# Bayes algorithm.
#

#  Team
#  Gautam Sapre (gss170001)
#  Sanjeev Penupala (sxp170022)

import pickle
from collections import Counter, defaultdict
from pprint import pprint

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from nltk import word_tokenize
from sklearn.naive_bayes import BernoulliNB, GaussianNB, MultinomialNB


class Simple_NB:
    """
    A class for fitting the classical Multinomial Naive Bayes model that is especially suitable
    for text classifcation problems. Calculates the priors of the classes and the likelihood of each word
    occurring given a class of documents.
    """

    def __init__(self):
        # Instance variables for the class.
        self.priors = {}
        self.likelihood = defaultdict(dict)
        self.columns = None

    def partition(self, x):
        """
        Partition the column vector into subsets indexed by its unique values (v1, ... vk)

        Returns a dictionary of the form
        { v1: indices of y == v1,
        v2: indices of y == v2,
        ...
        vk: indices of y == vk }, where [v1, ... vk] are all the unique values in the vector z.
        """
        return {v: np.where(x == v)[0] for v in np.unique(x)}

    def fit(self, X, y, column_names, alpha=1):
        """
        Compute the priors P(y=k), and the class-conditional probability (likelihood param) of each feature
        given the class=k. P(y=k) is the the prior probability of each class k. It can be calculated by computing
        the percentage of samples belonging to each class. P(x_i|y=k) is the number of counts feature x_i occured
        divided by the total frequency of terms in class y=k.

        The parameters after performing smooothing will be represented as follows.
            P(x_i|y=k) = (count(x_i|y=k) + alpha)/(count(x|y=k) + |V|*alpha)
            where |V| is the vocabulary of text classification problem or
            the size of the feature set

        :param x: features
        :param y: labels
        :param alpha: smoothing parameter

        Compute the two class instance variable
        :param self.priors: = Dictionary | self.priors[label]
        :param self.likelihood: = Dictionary | self.likelihood[label][feature]

        """
        self.columns = column_names
        y_labels = np.unique(y)
        X_label_indices = self.partition(y)

        for label in y_labels:
            self.priors[label] = np.sum(y == label) / len(y)
            X_label = X[X_label_indices[label]]

            V = len(self.columns)  # Calculate vocabulary size
            bag_size = X_label.sum()  # Calculate count(x|y=k)
            smoothed_total_count = bag_size + V * alpha

            for index, word in enumerate(self.columns):
                count = X_label[:, index].sum()  # count(x_i|y=k)
                smoothed_likelihood = (count + alpha) / smoothed_total_count
                self.likelihood[label][word] = smoothed_likelihood

            # Tip: Add an extra key in your likelihood dictionary for unseen data. This will be used when testing sample texts
            # that contain words not present in feature set
            self.likelihood[label]["__unseen__"] = alpha / smoothed_total_count

    def predict_example(self, x, sample_text=False, return_likelihood=False):
        """
        Predicts the classification label for a single example x by computing the posterior probability
        for each class value, P(y=k|x) = P(x_i|y=k)*P(y=k).
        The predicted class will be the argmax of P(y=k|x) over all the different k's,
        i.e. the class that gives the highest posterior probability
        NOTE: Converting the probabilities into log-space would help with any underflow errors.

        :param x: example to predict
        :return: predicted label for x
        :return: posterior log_likelihood of all classes if return_likelihood=True
        """

        if sample_text:
            x = Counter(x)
            words, x = x.keys(), x.values()
        else:
            words = self.columns

        post_log_likelihoods = []
        for label, prior_prob in sorted(self.priors.items()):

            log_likelihoods = []
            for word, count in zip(words, x):
                if word in self.likelihood[label]:
                    log_likelihood = np.log(self.likelihood[label][word])
                else:
                    log_likelihood = np.log(self.likelihood[label]["__unseen__"])
                log_likelihoods.append(log_likelihood * count)

            prior_log_prob = np.log(prior_prob)
            log_likelihoods_sum = np.sum(log_likelihoods)
            post_log_likelihoods.append(prior_log_prob + log_likelihoods_sum)

        return (
            (np.argmax(post_log_likelihoods), post_log_likelihoods)
            if return_likelihood
            else np.argmax(post_log_likelihoods)
        )

    def get_top_words(self, w=3):
        """
        From the features provided in the training data set, report the top three words that have the
        highest  class-conditional  likelihoods  for  both  the  “Spam”  and  “Not  Spam”  classes  along  with  their
        log-likelihood score.

        :param w: number of top words
        :return: top words, along with their likeihoods
        """
        top_words = {}

        for label, likeihoods in self.likelihood.items():
            likeihoods = Counter(likeihoods)
            top_likeihoods = dict(likeihoods.most_common(w))
            top_likeihoods = {
                word: {"Likelihood": likeihood, "Log-Likelihood": np.log(likeihood)}
                for word, likeihood in top_likeihoods.items()
            }
            top_words[label] = top_likeihoods

        return top_words


def compute_accuracy(y_true, y_pred):
    """
    Computes the average error between the true labels (y_true) and the predicted labels (y_pred)
    :param y_true: true label
    :param y_pred: predicted label
    :return: error rate = (1/n) * sum(y_true!=y_pred)
    """
    n = len(y_true)
    return np.sum(y_true == y_pred) / n


def compute_precision(y_true, y_pred):

    """
    Computes the precision for the given set of predictions.
    Precision gives the proportion of positive predictions that are actually correct.
    :param y_true: true label
    :param y_pred: predicted label
    :return: precision
    """
    TP = np.sum((y_true == 1) & (y_pred == 1))
    FP = np.sum((y_true == 0) & (y_pred == 1))
    return TP / (TP + FP) if (TP + FP) != 0 else 0


def compute_recall(y_true, y_pred):
    """
    Computes the recall for the given set of predictions.
    Recall measures the proportion of actual positives that were predicted correctly.
    :param y_true: true label
    :param y_pred: predicted label
    :return: recall
    """
    TP = np.sum((y_true == 1) & (y_pred == 1))
    FN = np.sum((y_true == 1) & (y_pred == 0))
    return TP / (TP + FN) if (TP + FN) != 0 else 0


def compute_f1(y_true, y_pred):
    """
    Computes the f1 score for a given set of predictions.
    F1 score is defined as the harmonic mean of precision and recall.
    :param y_true: true label
    :param y_pred: predicted label
    :return: f1 = 2 * (P*R)/(P+R)
    """
    precision = compute_precision(y_true, y_pred)
    recall = compute_recall(y_true, y_pred)
    return (
        2 * (precision * recall) / (precision + recall)
        if (precision + recall) != 0
        else 0
    )


if __name__ == "__main__":

    df_train = pd.read_csv("data/train_email.csv")
    df_train.drop(df_train.columns[0], inplace=True, axis=1)

    df_test = pd.read_csv("data/test_email.csv")
    df_test.drop(df_test.columns[0], inplace=True, axis=1)

    X_columns = df_train.columns[:-1]
    print(f"{len(X_columns)=}")
    print(f"{df_train.shape=}")

    Xtrn = np.array(df_train.iloc[:, :-1])
    ytrn = np.array(df_train.iloc[:, -1])

    Xtst = np.array(df_test.iloc[:, :-1])
    ytst = np.array(df_test.iloc[:, -1])
    results = {}  # To Store All Results

    # PART A
    print("\nPART A")
    NB = Simple_NB()
    NB.fit(Xtrn, ytrn, column_names=X_columns, alpha=1)

    # Get top 3 words
    print("Top 3 Words")
    pprint(NB.get_top_words(3))

    # Prediction on Test Set
    y_preds = np.array([NB.predict_example(example) for example in Xtst])

    results["Simple Naive Bayes"] = {
        "accuracy": compute_accuracy(ytst, y_preds),
        "precision": compute_precision(ytst, y_preds),
        "recall": compute_recall(ytst, y_preds),
        "f1_score": compute_f1(ytst, y_preds),
    }
    print("NB Model Results")
    pprint(results)

    # PART B - Testing on Sample Text
    print("\nPART B")
    sample_email = [
        "Congratulations! Your raffle ticket has won yourself a house. Click on the link to avail prize",  # SAMPLE EMAIL 1
        "Hello. This email is to remind you that your project needs to be submitted this week",  # SAMPLE EMAIL 2
        "Dear Valued Customer, We are excited to announce our end-of-season sale, offering a flat discount of 30% on all products. We have a wide range of items to choose from, including clothing, accessories, and electronics.",  # SAMPLE EMAIL 3
    ]

    for index, sample in enumerate(sample_email, start=1):
        words = word_tokenize(sample.lower())
        tokens = [word for word in words if word.isalpha()]
        y_sent_pred, post_log_likelihoods = NB.predict_example(
            tokens, sample_text=True, return_likelihood=True
        )
        log_likelihoods = {
            label: np.exp(log_likelihood)
            for label, log_likelihood in enumerate(post_log_likelihoods)
        }

        print(f"Sample Email #{index}: {sample}")
        print(f"Classification: {'Is Spam' if y_sent_pred else 'Is Not Spam'}")
        print("Likelihoods:", log_likelihoods)
        print()

    # PART C - Compare with Sklearn's NB Models
    print("\nPART C")
    # Replace Nones with the respective Sklearn library.
    models = {
        "Gaussian Naive Bayes": GaussianNB(),
        "Bernoulli Naive Bayes": BernoulliNB(),
        "Multinomial Naive Bayes": MultinomialNB(),
    }

    for model_name, sk_lib in models.items():

        model = sk_lib
        model.fit(Xtrn, ytrn)

        # Predict the target values for test set
        y_pred = model.predict(Xtst)

        # Evaluate the Models with the different metrics
        # INSERT CODE HERE
        results[model_name] = {
            "accuracy": compute_accuracy(ytst, y_pred),
            "precision": compute_precision(ytst, y_pred),
            "recall": compute_recall(ytst, y_pred),
            "f1_score": compute_f1(ytst, y_pred),
        }

    pprint(results)

    # PART D - Visualize the model using bar charts

    # INSERT  CODE HERE
    # data = results["Gaussian Naive Bayes"]
    # metrics = list(data.keys())
    # values = list(data.values())
    # fig = plt.figure(figsize=(10, 5))

    # # creating the bar plot
    # plt.bar(metrics, values, color="maroon", width=0.4)

    # plt.xlabel("Metrics")
    # plt.ylabel("Values")
    # plt.title("Gaussian Naive Bayes")
    # plt.show()

    barWidth = 0.15
    fig = plt.subplots(figsize=(12, 8))
    simp = results["Simple Naive Bayes"].values()
    gau = results["Gaussian Naive Bayes"].values()
    ber = results["Bernoulli Naive Bayes"].values()
    mult = results["Multinomial Naive Bayes"].values()

    br1 = np.arange(len(simp))
    br2 = [x + barWidth for x in br1]
    br3 = [x + barWidth for x in br2]
    br4 = [x + barWidth for x in br3]

    plt.bar(
        br1,
        simp,
        color="r",
        width=barWidth,
        edgecolor="grey",
        label="Simple Naive Bayes",
    )
    plt.bar(
        br2,
        gau,
        color="g",
        width=barWidth,
        edgecolor="grey",
        label="Gaussian Naive Bayes",
    )
    plt.bar(
        br3,
        ber,
        color="b",
        width=barWidth,
        edgecolor="grey",
        label="Bernoulli Naive Bayes",
    )
    plt.bar(
        br4,
        mult,
        color="y",
        width=barWidth,
        edgecolor="grey",
        label="Multinomial Naive Bayes",
    )

    plt.xlabel("Metrics", fontweight="bold", fontsize=15)
    plt.ylabel("Score", fontweight="bold", fontsize=15)
    plt.xticks(
        [r + barWidth for r in range(len(simp))],
        ["Accuracy", "Precision", "Recall", "F1 Score"],
    )
    plt.legend()
    plt.show()

    # PART E - Save the Model
    # Code to store as pickle file
    netid = "gss170001_sxp170022"
    file_pi = open(f"{netid}_model_3.obj", "wb")  # Use your NETID
    pickle.dump(NB, file_pi)
