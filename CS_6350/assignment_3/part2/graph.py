import argparse

from graphframes import GraphFrame
from pyspark.sql import SparkSession
from pyspark.sql.functions import col

# Parse command line arguments
parser = argparse.ArgumentParser(
    description="Social Network Analysis with PySpark and GraphFrames."
)
parser.add_argument("input_file", type=str, help="Input file path")
parser.add_argument("output_dir", type=str, help="Output directory path")
args = parser.parse_args()

# Initialize Spark Session
spark = (
    SparkSession.builder.appName("SocialNetworkAnalysis")
    .config("spark.jars.packages", "graphframes:graphframes:0.8.1-spark3.0-s_2.12")
    .getOrCreate()
)
spark.sparkContext.setLogLevel("ERROR")
sc = spark.sparkContext
sc.setCheckpointDir("checkpoint")

edges_df = spark.read.csv(
    args.input_file, sep="\t", inferSchema=True, comment="#"
).toDF("src", "dst")

vertices_df = (
    edges_df.select(col("src").alias("id"))
    .distinct()
    .union(edges_df.select(col("dst").alias("id")).distinct())
)

g = GraphFrame(vertices_df, edges_df)


# Queries
out_degrees = g.outDegrees.orderBy(col("outDegree").desc()).limit(5)

in_degrees = g.inDegrees.orderBy(col("inDegree").desc()).limit(5)

page_rank = (
    g.pageRank(resetProbability=0.15, tol=0.01)
    .vertices.orderBy(col("pagerank").desc())
    .limit(5)
)

largest_components = (
    g.connectedComponents()
    .groupBy("component")
    .count()
    .orderBy(col("count").desc())
    .limit(5)
)

triangle_counts = g.triangleCount().orderBy(col("count").desc()).limit(5)

# Outputs
out_degrees.coalesce(1).write.mode("overwrite").csv(
    f"{args.output_dir}/out_degrees.csv", header=True
)

in_degrees.coalesce(1).write.mode("overwrite").csv(
    f"{args.output_dir}/in_degrees.csv", header=True
)

page_rank.coalesce(1).write.mode("overwrite").csv(
    f"{args.output_dir}/page_rank.csv", header=True
)

largest_components.coalesce(1).write.mode("overwrite").csv(
    f"{args.output_dir}/largest_components.csv", header=True
)

triangle_counts.coalesce(1).write.mode("overwrite").csv(
    f"{args.output_dir}/triangle_counts.csv", header=True
)

# Stop Spark Session
spark.stop()
