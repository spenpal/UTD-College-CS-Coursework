# HOW TO RUN CODE

You will need the following libraries installed in your environment:

- PySpark (*and Apache Spark*)
- NumPy (*for modeling*)
- Matplotlib (*for visualizations*)

This project was ran locally, however, it can be ran on AWS as well. Use a service like **EMR Serverless** or **Sagemaker** to run this notebook, and ensuring the following libraries are installed in your environment.

The dataset can be found at this [S3 bucket location](https://bd-project-data-fall2023.s3.us-east-2.amazonaws.com/2021.csv) and will need to be downloaded into your environment.

Couple of key important code lines to note:
1. In code block **#3**...
```python
file_path = "2021.csv"
data_2021 = spark.read.csv(file_path, header=True, inferSchema=True)
```
The name of the CSV file is `2021.csv`. Depending on the filesystem and environment you are in, you may need to change the `file_path` variable to where you downloaded the dataset into.

2. In code block **#15**...
```python
preprocessed_data_2021 = preprocessed_data_2021.limit(10000)
```
As this code was tested on a local machine, there wasn't enough allocated RAM to train our model on more rows in our dataset. However, if you are on a more powerful machine or running this code in AWS, you can comment this line out, so the dataset is not limited.