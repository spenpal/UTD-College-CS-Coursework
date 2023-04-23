# decision_tree.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# personal and educational purposes provided that (1) you do not distribute
# or publish solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UT Dallas, including a link to http://cs.utdallas.edu.
#
# This file is part of Programming Assignment 1 for CS6375: Machine Learning.
# Gautam Kunapuli (gautam.kunapuli@utdallas.edu)
# Sriraam Natarajan (sriraam.natarajan@utdallas.edu),
#
#
# INSTRUCTIONS:
# ------------
# 1. This file contains a skeleton for implementing the ID3 algorithm for
# Decision Trees. Insert your code into the various functions that have the
# comment "INSERT YOUR CODE HERE".
#
# 2. Do NOT modify the classes or functions that have the comment "DO NOT
# MODIFY THIS FUNCTION".
#
# 3. Do not modify the function headers for ANY of the functions.
#
# 4. You may add any other helper functions you feel you may need to print,
# visualize, test, or save the data and results. However, you MAY NOT utilize
# the package scikit-learn OR ANY OTHER machine learning package in THIS file.

#  Team
#  Gautam Sapre (gss170001)
#  Sanjeev Penupala (sxp170022)

from collections import defaultdict

import numpy as np
from sklearn.ensemble import AdaBoostClassifier, BaggingClassifier
from sklearn.metrics import confusion_matrix
from sklearn.tree import DecisionTreeClassifier


def partition(x):
    """
    Partition the column vector x into subsets indexed by its unique values (v1, ... vk)

    Returns a dictionary of the form
    { v1: indices of x == v1,
      v2: indices of x == v2,
      ...
      vk: indices of x == vk }, where [v1, ... vk] are all the unique values in the vector z.
    """
    return {v: np.where(x == v)[0] for v in np.unique(x)}


def entropy(y, weights=None):
    """
    Compute the entropy of a vector y by considering the counts of the unique values (v1, ... vk), in z.
    Include the weights of the boosted examples if present

    Returns the entropy of z: H(z) = p(z=v1) log2(p(z=v1)) + ... + p(z=vk) log2(p(z=vk))
    """
    if weights is None:
        weights = np.ones(len(y)) / len(y)

    p = np.bincount(y, weights=weights) / np.sum(weights)
    # set entropy to 0 if probability is 0
    entropy = -np.sum(p * np.log2(np.where(p == 0, 1, p)))
    return entropy


def mutual_information(x, y, weights=None):
    """
    Compute the mutual information between a data column (x) and the labels (y). The data column is a single attribute
    over all the examples (n x 1). Mutual information is the difference between the entropy BEFORE the split set, and
    the weighted-average entropy of EACH possible split.

    Returns the mutual information: I(x, y) = H(y) - H(y | x)

    Compute the weighted mutual information for Boosted learners
    """
    if weights is None:
        weights = np.ones(len(x)) / len(x)

    entropy_y = entropy(y, weights)
    entropy_y_given_x = 0
    for value, indices in partition(x).items():
        # normalized_weights = weights[indices] / np.sum(weights[indices])
        entropy_y_given_x_value = entropy(y[indices], weights[indices])
        prob = len(indices) / len(x)
        entropy_y_given_x += prob * entropy_y_given_x_value
    return entropy_y - entropy_y_given_x


def get_attribute_value_pairs(X):
    return [
        (index, value)
        for index in range(X.shape[1])
        for value in np.unique(X[:, index])
    ]


def id3(X, y, attribute_value_pairs=None, depth=0, max_depth=5, weights=None):
    """
    Implements the classical ID3 algorithm given training data (x), training labels (y) and an array of
    attribute-value pairs to consider. This is a recursive algorithm that depends on three termination conditions
        1. If the entire set of labels (y) is pure (all y = only 0 or only 1), then return that label
        2. If the set of attribute-value pairs is empty (there is nothing to split on), then return the most common
           value of y (majority label)
        3. If the max_depth is reached (pre-pruning bias), then return the most common value of y (majority label)
    Otherwise the algorithm selects the next best attribute-value pair using INFORMATION GAIN as the splitting criterion
    and partitions the data set based on the values of that attribute before the next recursive call to ID3.

    The tree we learn is a BINARY tree, which means that every node has only two branches. The splitting criterion has
    to be chosen from among all possible attribute-value pairs. That is, for a problem with two features/attributes x1
    (taking values a, b, c) and x2 (taking values d, e), the initial attribute value pair list is a list of all pairs of
    attributes with their corresponding values:
    [(x1, a),
     (x1, b),
     (x1, c),
     (x2, d),
     (x2, e)]
     If we select (x2, d) as the best attribute-value pair, then the new decision node becomes: [ (x2 == d)? ] and
     the attribute-value pair (x2, d) is removed from the list of attribute_value_pairs.

    The tree is stored as a nested dictionary, where each entry is of the form
                    (attribute_index, attribute_value, True/False): subtree
    * The (attribute_index, attribute_value) determines the splitting criterion of the current node. For example, (4, 2)
    indicates that we test if (x4 == 2) at the current node.
    * The subtree itself can be nested dictionary, or a single label (leaf node).
    * Leaf nodes are (majority) class labels

    Returns a decision tree represented as a nested dictionary, for example
    {(4, 1, False):
        {(0, 1, False):
            {(1, 1, False): 1,
             (1, 1, True): 0},
         (0, 1, True):
            {(1, 1, False): 0,
             (1, 1, True): 1}},
     (4, 1, True): 1}
    """
    # If attribute_value_pairs weren't passed in initially...
    if attribute_value_pairs is None and depth == 0:
        attribute_value_pairs = get_attribute_value_pairs(X)

    # Base Cases
    if weights is None:
        weights = np.ones(len(X)) / len(X)
    if len(set(y)) == 1 or not attribute_value_pairs or depth == max_depth:
        return int(np.mean(y) >= 0.5)

    # Find best attribute-value pair
    best_gain = -1
    best_feature = best_value = None
    for feature, value in attribute_value_pairs:
        X_copy = X.copy()
        X_copy[:, feature][X_copy[:, feature] != value] = 0
        gain = mutual_information(X_copy[:, feature], y, weights)
        if gain > best_gain:
            best_gain = gain
            best_feature, best_value = feature, value

    # Split dataset by best attribute-value pair
    # Split by False examples
    mask_false = X[:, best_feature] != best_value
    X_false = X[mask_false]
    y_false = y[mask_false]
    weights_false = weights[mask_false]

    # Split by True examples
    mask_true = X[:, best_feature] == best_value
    X_true = X[mask_true]
    y_true = y[mask_true]
    weights_true = weights[mask_true]

    # Remove attribute-value pair(s) for each split
    false_attribute_value_pairs = attribute_value_pairs.copy()
    false_attribute_value_pairs.remove((best_feature, best_value))
    true_attribute_value_pairs = [
        avp for avp in attribute_value_pairs if avp[0] != best_feature
    ]

    return {
        (best_feature, best_value, False): id3(
            X_false,
            y_false,
            false_attribute_value_pairs,
            depth + 1,
            max_depth,
            weights_false,
        ),
        (best_feature, best_value, True): id3(
            X_true,
            y_true,
            true_attribute_value_pairs,
            depth + 1,
            max_depth,
            weights_true,
        ),
    }


def bootstrap_sampler(X, y, num_samples):
    n = len(X)
    samples = []
    for _ in range(num_samples):
        idxes = np.random.randint(n, size=n)
        x_sample = X[idxes]
        y_sample = y[idxes]
        samples.append((x_sample, y_sample))
    return samples


def bagging(X, y, max_depth, num_trees):
    """
    Implements bagging of multiple id3 trees where each tree trains on a boostrap sample of the original dataset
    """
    return [
        (1, id3(X_boot, y_boot, max_depth=max_depth))
        for X_boot, y_boot in bootstrap_sampler(X, y, num_trees)
    ]


def boosting(X, y, max_depth, num_stumps):
    """
    Implements an adaboost algorithm using the id3 algorithm as a base decision tree
    """
    weights = np.ones(len(X)) / len(X)
    weak_learners = []

    for _ in range(num_stumps):
        tree = id3(X, y, max_depth=max_depth, weights=weights)
        # visualize(tree, depth=0)
        y_pred = [predict_example(x, tree) for x in X]

        error = np.sum(weights[y_pred != y])
        alpha = 0.5 * np.log((1 - error) / error)

        weights *= np.exp(alpha * np.where(y != y_pred, 1, -1))
        weights /= np.sum(weights)

        weak_learners.append((alpha, tree))

    return weak_learners


def predict_example(x, tree):
    """
    Predicts the classification label for a single example x using tree by recursively descending the tree until
    a label/leaf node is reached.

    Returns the predicted label of x according to tree
    """
    if not isinstance(tree, dict):
        return tree

    for (feature, value, bool), sub_tree in tree.items():
        if (x[feature] == value) == bool:
            return predict_example(x, sub_tree)


def predict_example_ens(x, h_ens):
    """
    Predicts the classification label for a single example x using a combination of weighted trees
    Returns the predicted label of x according to tree
    """
    weighted_sum = 0
    for alpha, tree in h_ens:
        prediction = predict_example(x, tree)
        prediction = 2 * prediction - 1  # Convert the prediction from 0/1 to -1/1
        weighted_sum += alpha * prediction
    return 1 if weighted_sum >= 0 else 0


def compute_error(y_true, y_pred):
    """
    Computes the average error between the true labels (y_true) and the predicted labels (y_pred)

    Returns the error = (1/n) * sum(y_true != y_pred)
    """
    return np.sum(y_true != y_pred) / len(y_true)


def visualize(tree, depth=0):
    """
    Pretty prints (kinda ugly, but hey, it's better than nothing) the decision tree to the console. Use print(tree) to
    print the raw nested dictionary representation.
    DO NOT MODIFY THIS FUNCTION!
    """
    if depth == 0:
        print("TREE")

    for index, split_criterion in enumerate(tree):
        sub_trees = tree[split_criterion]

        # Print the current node: split criterion
        print("|\t" * depth, end="")
        print(
            "+-- [SPLIT: x{0} = {1} | {2}]".format(
                split_criterion[0], split_criterion[1], split_criterion[2]
            )
        )

        # Print the children
        if type(sub_trees) is dict:
            visualize(sub_trees, depth + 1)
        else:
            print("|\t" * (depth + 1), end="")
            print("+-- [LABEL = {0}]".format(sub_trees))


if __name__ == "__main__":
    # Load the training data
    M = np.genfromtxt(
        "data/monks-1.train", missing_values=0, skip_header=0, delimiter=",", dtype=int
    )
    ytrn = M[:, 0]
    Xtrn = M[:, 1:]

    # Load the test data
    M = np.genfromtxt(
        "data/monks-1.test", missing_values=0, skip_header=0, delimiter=",", dtype=int
    )
    ytst = M[:, 0]
    Xtst = M[:, 1:]

    # PART A: BAGGING
    print("PART A: BAGGING")
    for d in [3, 5]:
        for bag_size in [10, 20]:
            h_ens = bagging(Xtrn, ytrn, d, bag_size)
            y_pred = [predict_example_ens(x, h_ens) for x in Xtst]

            print(f"FOR ({d=}, {bag_size=})...")
            print(
                f"Test Error ({d=}, {bag_size=}): {compute_error(ytst, y_pred) * 100:.2f}%"
            )
            print(f"Confusion Matrix:\n{confusion_matrix(ytst, y_pred)}")
            print()

    # PART B: BOOSTING
    print("----------------")
    print("PART B: BOOSTING")
    for d in [1, 2]:
        for bag_size in [20, 40]:
            h_ens = boosting(Xtrn, ytrn, d, bag_size)
            y_pred = [predict_example_ens(x, h_ens) for x in Xtst]

            print(f"FOR ({d=}, {bag_size=})...")
            print(
                f"Test Error ({d=}, {bag_size=}): {compute_error(ytst, y_pred) * 100:.2f}%"
            )
            print(f"Confusion Matrix:\n{confusion_matrix(ytst, y_pred)}")
            print()

    # PART C: SKLEARN
    print("----------------")
    print("PART C: SKLEARN")

    # Sklearn Bagging
    print("> Sklearn Bagging")
    for d in [3, 5]:
        for bag_size in [10, 20]:
            bagging_clf = BaggingClassifier(
                estimator=DecisionTreeClassifier(criterion="entropy", max_depth=d),
                n_estimators=bag_size,
            )
            bagging_clf.fit(Xtrn, ytrn)
            y_pred = bagging_clf.predict(Xtst)

            print(f"FOR ({d=}, {bag_size=})...")
            print(
                f"Test Error ({d=}, {bag_size=}): {compute_error(ytst, y_pred) * 100:.2f}%"
            )
            print(f"Confusion Matrix:\n{confusion_matrix(ytst, y_pred)}")
            print()

    # Sklearn Boosting
    print("> Sklearn Boosting")
    for d in [1, 2]:
        for bag_size in [20, 40]:
            bagging_clf = AdaBoostClassifier(
                estimator=DecisionTreeClassifier(criterion="entropy", max_depth=d),
                n_estimators=bag_size,
            )
            bagging_clf.fit(Xtrn, ytrn)
            y_pred = bagging_clf.predict(Xtst)

            print(f"FOR ({d=}, {bag_size=})...")
            print(
                f"Test Error ({d=}, {bag_size=}): {compute_error(ytst, y_pred) * 100:.2f}%"
            )
            print(f"Confusion Matrix:\n{confusion_matrix(ytst, y_pred)}")
            print()
