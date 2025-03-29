import os
import time
import subprocess

from io import BytesIO
from PIL import Image
from pyspark.sql import SparkSession
    

spark = SparkSession.builder \
    .appName("ImageExample") \
    .master("spark://spark-master:7077") \
    .config("spark.hadoop.fs.defaultFS", "hdfs://spark-master:9000") \
    .config("spark.hadoop.dfs.client.use.datanode.hostname", "true") \
    .config("spark.executorEnv.HADOOP_USER_NAME", "root") \
    .getOrCreate()
spark.sparkContext.setLogLevel("WARN")

img_names_path = [
    '/images/simple_image.png',
    '/images/complex_image.png',
    '/images/favorite_image.png',
]
rdd_images = None
for img_path in img_names_path:
    print("process", img_path)
    rdd_image = spark.sparkContext.binaryFiles(img_path).map(lambda x: (x[0][len("hdfs://spark-master:9000/images/"):-len(".png")], x[1]))
    if rdd_images is not None:
        rdd_images = rdd_images.union(rdd_image)
    else:
        rdd_images = rdd_image

def run_compression_on_file(image, filename, compression_method, compression_fun):
    img = Image.open(BytesIO(image))
    name_original = f"{filename}_original{compression_method}.png"
    name_final = f"{filename}_final_{compression_method}"
    img.save(
        name_original,
        format="PNG",
        optimize=False, 
        compress_level=0
    )
    time_start = time.time()
    compression_fun(name_original, name_final)
    time_end = time.time()
    os.remove(name_original)
    os.remove(name_final)
    return (compression_method, time_end - time_start)

def comp_jpeg(original_filename, end_filename):
    image = Image.open(original_filename).save(original_filename[:-4]+ ".ppm")
    subprocess.run(["cjxl", original_filename[:-4]+ ".ppm", end_filename])

def comp_avif(original_filename, end_filename):
    subprocess.run(["avifenc", original_filename, end_filename]) 

def comp_webp(original_filename, end_filename):
    im = Image.open(original_filename)
    im.save(end_filename, "WEBP", quality=80, method=6) 


rdd_jpeg = rdd_images.map(lambda x : run_compression_on_file(x[1], x[0], "jpeg", comp_jpeg))
rdd_avif = rdd_images.map(lambda x : run_compression_on_file(x[1], x[0], "avif", comp_avif))
rdd_webp = rdd_images.map(lambda x : run_compression_on_file(x[1], x[0], "webp", comp_webp))

rdd_all = rdd_jpeg.union(rdd_avif).union(rdd_webp).groupByKey().map(lambda x: (x[0], sum(x[1]) / len(x[1])))
print(rdd_all.collect())

time.sleep(60*60)
spark.stop()
