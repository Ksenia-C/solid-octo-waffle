#!/bin/sh

cleanup() {
  $SPARK_HOME/sbin/stop-master.sh
  exit 0
}
trap cleanup SIGTERM

export JAVA_HOME=/usr/lib/jvm/java-17-openjdk
hdfs namenode -format -force && hdfs --daemon start namenode


export SPARK_HOME=/opt/spark
export SPARK_MASTER_HOST="spark-master"
$SPARK_HOME/sbin/start-master.sh

# ping would be more appropriate
sleep 10
hdfs dfs -mkdir -p /data/
hdfs dfs -put /spark/data_for_jobs/novels.txt /data/novels.txt
hdfs dfs -put /spark/data_for_jobs/poems.txt /data/poems.txt
hdfs dfs -put /spark/data_for_jobs/reviews.txt /data/reviews.txt


hdfs dfs -mkdir -p /images/
hdfs dfs -put /spark/images/Lenna.png /images/simple_image.png
hdfs dfs -put /spark/images/Lenna1.png /images/complex_image.png
hdfs dfs -put /spark/images/Lenna2.png /images/favorite_image.png

tail -f /dev/null & wait
