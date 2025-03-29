Hi, can't say this code is genius but it serves it purpose.

# installation & Usage

```bash
cd dockers
docker build .. --tag spark_simple/base:1.0 -f Dockerfile.base
docker-compose up # --build
# wait for about the 30 seconds
docker exec -it spark-master /opt/spark/bin/spark-submit /spark/scripts/different_image_compressions.py
# after modifying file without restarting anything:
docker cp /home/login/path_to_scripts/different_image_compressions.py spark-master:/spark/scripts/different_image_compressions.py
```
