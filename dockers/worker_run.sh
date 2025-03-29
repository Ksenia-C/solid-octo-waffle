#!/bin/sh

cleanup() {
  $SPARK_HOME/sbin/stop-worker.sh
  exit 0
}
export SPARK_HOME=/opt/spark
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk
trap cleanup SIGTERM

# ping would be more appropriate
sleep 5
hdfs --daemon start datanode


$SPARK_HOME/sbin/start-worker.sh spark://spark-master:7077
tail -f /dev/null & wait
