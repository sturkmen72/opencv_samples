#Classification

After we preprocess and segment all possible parts of an image, we now need to decide if each segment is (or is not) a license plate. To do this, we will use a Support Vector Machine (SVM) algorithm.

A Support Vector Machine is a pattern recognition algorithm included in a family of supervised-learning algorithms originally created for binary classification. Supervised learning is machine-learning algorithm that learns through the use of labeled data. We need to train the algorithm with an amount of data that is labeled; each data set needs to have a class.

The SVM creates one or more hyperplanes that are used to discriminate each class of the data.
