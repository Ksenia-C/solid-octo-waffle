import time
from pyspark.sql import SparkSession

spark = SparkSession.builder \
    .appName("ArticleExampleNoParts") \
    .master("spark://spark-master:7077") \
    .getOrCreate()

poems_data = [
    ('id1', 'Alice in the tree', 14000, "Wonderson"),
    ('id2', 'Bob outside the top', 15000, "Sleepson"),
    ('id3', 'Sandy and a candy', 4000, "Davidson"),
    ('id4', 'Cry out loud', 84000, "Coolson"),
    ('id5', 'The big liar', 19000, "Cupson"),
    ('id6', 'who wants A+ say it out loud', 100000, "Zanzibar gem"),
    ('id7', 'we dont believe whats on tv', 500, "21 pilots"),
    ('id8', 'the mind electric', 10, "Miracle Musical"),
]
rdd_poems = spark.sparkContext.parallelize(poems_data)

novels_data = [
    ('The good function', 2015, "Wonderson"),
    ('Plants', 2000, "Sleepson"),
    ('dirty reads and writes', 2121, "Davidson"),
    ('please water me', 1923, "Zanzibar gem"),
    ('migraine', 2002, "21 pilots"),
    ('Labyrinth', 2013, "Miracle Musical"),
]
rdd_novels = spark.sparkContext.parallelize(novels_data)


rdd_poems = rdd_poems.map(lambda x: (x[1], x[3]))

rdd_novels = rdd_novels.map(lambda x: (x[0], x[2]))



reviews = [
    ('Alice in the tree', 9),
    ('Bob outside the top', 6),
    ('Sandy and a candy', 5),
    ('Cry out loud', 10),
    ('The big liar', 9),
    ('who wants A+ say it out loud', 3),
    ('we dont believe whats on tv', 9),
    ('the mind electric', 10),
    ('The good function', 4),
    ('Plants', 10),
    ('dirty reads and writes', 6),
    ('please water me', 5),
    ('migraine', 9),
    ('Labyrinth', 9),
]

rdd_reviews = spark.sparkContext.parallelize(reviews)
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

time.sleep(60*60)
spark.stop()

