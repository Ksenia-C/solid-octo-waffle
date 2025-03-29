import time
from pyspark import TaskContext
from pyspark.sql import SparkSession

spark = SparkSession.builder \
    .appName("ArticleExampleHdfs") \
    .master("spark://spark-master:7077") \
    .config("spark.hadoop.fs.defaultFS", "hdfs://spark-master:9000") \
    .config("spark.hadoop.dfs.client.use.datanode.hostname", "true") \
    .config("spark.executorEnv.HADOOP_USER_NAME", "root") \
    .getOrCreate()
# spark.sparkContext.setLogLevel("WARN")


def split_poems(line):
    y = line.split(',')
    return (y[0], y[1], int(y[2]), y[3])
poems_data_path = "/data/poems.txt"
rdd_poems = spark.sparkContext.textFile(poems_data_path).map(split_poems)

def split_novels(line):
    y = line.split(',')
    return (y[0], int(y[1]), y[2])
novels_data_path = "/data/novels.txt"
rdd_novels = spark.sparkContext.textFile(novels_data_path).map(split_novels)

rdd_poems = rdd_poems.map(lambda x: (x[1], x[3]))
rdd_novels = rdd_novels.map(lambda x: (x[0], x[2]))


def split_reviews(line):
    y = line.split(',')
    return (y[0], int(y[1]))
reviews_data_path = "/data/reviews.txt"
rdd_reviews = spark.sparkContext.textFile(reviews_data_path).map(split_reviews)
rdd_reviews.persist()


def hash_by_author(item):
    return hash(item[0]) % 2

rdd_poems = rdd_poems.join(rdd_reviews)
rdd_poems = rdd_poems.map(lambda x: [x[1][0], (x[0], x[1][1])])
rdd_novels = rdd_novels.join(rdd_reviews)
rdd_novels = rdd_novels.map(lambda x: [x[1][0], (x[0], x[1][1])])

rdd_rating = rdd_poems.fullOuterJoin(rdd_novels)
rdd_rating = rdd_rating.flatMap(lambda x: [(x[0], x[1][0]), (x[0], x[1][1])])
rdd_rating = rdd_rating.reduceByKey(lambda x, y: x[1] + y[1] if y is not None else x[1])
rdd_rating = rdd_rating.sortBy(lambda x: x[1])
print("The author with the highest rate", rdd_rating.reduce(lambda x, y: x if x[1] > y[1] else y))
print("All authors", rdd_rating.count())

rdd_comparison = rdd_rating.cartesian(rdd_reviews).filter(lambda x: x[0][1] < x[1][1]) # crossProduct
rdd_comparison = rdd_comparison.map(lambda x: (x[0][0], x[1][1] - x[0][1])).groupByKey().map(lambda x : (x[0], sum(x[1]) / len(x[1])))
print("Average worseness: ", rdd_comparison.collect()) 


debug_string = rdd_comparison.toDebugString().decode("utf-8")
print("Lineage:\n", debug_string)

partitioned_data = rdd_comparison.glom().collect()
for i, partition in enumerate(partitioned_data):
    print(f"Partition {i}: {partition}")

time.sleep(60 * 60)
spark.stop()
