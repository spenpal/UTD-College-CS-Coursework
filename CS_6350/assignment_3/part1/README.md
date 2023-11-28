# Spark Streaming with Real Time Data and Kafka

## Project Setup

This entire project setup will be done locally.

-   Install [Apache Kafka](https://kafka.apache.org/quickstart) and start Zookeeper and Kafka servers.
    -   You will need to create 2 topics. We named ours `RawDataTopic` and `NamedEntitiesCountTopic`.
        -   `~/kafka_*/bin/kafka-topics.sh --create --topic <topic-name> --bootstrap-server localhost:9092 --partitions 1 --replication-factor 1`
    -   To see what topics are available on your Kafka server, you can use the following command: `~/kafka_*/bin/kafka-topics.sh --list --bootstrap-server localhost:9092`
-   Install the latest [Apache Spark](https://spark.apache.org/downloads.html).
    -   We used this article to help setup [Apache Spark](https://www.virtono.com/community/tutorial-how-to/how-to-install-apache-spark-on-ubuntu-22-04-and-centos/), since we have WSL2.
-   Install [Elasticsearch, Kibana, and Logstash](https://www.elastic.co/downloads/).
    -   We installed the EKL stack using the `apt-get` package manager.
    -   Article Helps
        -   [Installation and Setup of Elasticsearch](https://www.cherryservers.com/blog/install-elasticsearch-ubuntu)
        -   [Installation and Setup of Logstash](https://devconnected.com/how-to-install-logstash-on-ubuntu-18-04-and-debian-9/#3_-_Install_Logstash_with_apt)
        -   [Logstash Directory Layout](https://www.elastic.co/guide/en/logstash/current/dir-layout.html)
    -   In `/etc/elasticsearch/elasticsearch.yml` config file, make sure you set `network.host` to **localhost**, and if you face authentication issues with Elasticsearch, you can set `xpack.security.enabled` to **False**.
    -   Make sure the user and group of the `/usr/share/logstash/data` folder is by `logstash`. If not, use the following commands to fix it:
        -   `chown -R logstash.logstash /usr/share/logstash`
        -   `chmod 777 /usr/share/logstash/data`
-   Get an API Key from [NewsAPI](https://newsapi.org/).
-   Install project dependencies: `pip install requirements.txt`
    -   We are using `spacy` for NER, which requires to download one of their models: `python -m spacy download en_core_web_sm`
    -   **NOTE**: For this project, we are using **Python 3.11**, however **Python 3.9+** should work.
-   Create a new file at `./src/.env`.
    -   Inside the `src/` directory, there is a `config.env` file, which is an example **.env** file you will have to create. It contains all neccesary environment variables needed for this project, as an alternative to using command arguments. **Fill in all of the variables from `config.env` into your newly created `.env` file, based on the instructions listed in the `config.env`.**
    -   You will also need to create a _checkpoints_ directory for the PySpark code to work. We created ours at our **home directory** called `spark_checkpoints/`. This allows us to have write access to the _checkpoints_ directory.

## Data Flow

The flow of our program is defined as follows:

1. Get news data from NewsAPI and send it to the **RawDataTopic** (`newsapi.py`)
2. Get news data from **RawDataTopic** by uploading it to a `PySpark` streaming dataframe, transform the data to keep a running count of named entities found, and send these tallies to the **NamedEntitiesCountTopic** in a JSON format (_ex: {"named_entity":"Tesla","count":"4"}_) (`streamer.py`).
3. Setup `Logstash` to parse the data stored in **NamedEntitiesCountTopic** and send it to a pre-defined index in `Elasticsearch`.
4. Use `Kibana` to explore the data and create bar plots of the top 10 named entities by counts, in several intervals.

## Getting Started

Because this is a streaming data application, we have to setup our program in _reverse order_ of the flow of the program (_in other words, setup recievers before producers_).

1. Start up Elasticsearch and Kibana services (_because we installed these through `apt-get`, we will be using `sudo service` to start them up_).
    - `sudo service elasticsearch start`
        - To check if Elasticsearch has started: `sudo service elasticsearch status`
        - Verify Elasticsearch is working: `curl http://localhost:9200/`
    - `sudo service kibana start`
        - To check if Kibana has started: `sudo service kibana status`
1. Create an index at Elasticsearch for named entity data to be sent to. We called ours `named_entities`.
    - `curl -X PUT "localhost:9200/your_index_name?pretty`
1. Copy the `logstash.conf` file (_located inside this project directory_) to the config directory of `Logstash`. If you installed `Logstash` through `apt-get`, the config directory is located at `/etc/logstash/conf.d`.
    - `cp logstash.conf /etc/logstash/conf.d`
1. Go to home directory of `Logstash` and run Logstash with the configuration file: `bin/logstash -f /etc/logstash/conf.d/logstash.conf`
    - To find the home directory of **Logstash**: `whereis -b logstash`
1. Come back to this directory and start the streaming script by submitting it to the Spark cluster: `spark-submit --packages org.apache.spark:spark-sql-kafka-0-10_2.12:<your-spark-version> src/streamer.py`
    - We have the latest Spark version (3.5.0), so we would use: `spark-submit --packages org.apache.spark:spark-sql-kafka-0-10_2.12:3.5.0 src/streamer.py`
1. Start the NewsAPI script: `python src/newsapi.py`
    - It is currently configured to fetch from the API every 60 mins, as fetching any earlier will result in duplicate news.
1. Data should slowly start populating in Elasticsearch. You can access the data in Kibana at [http://localhost:5601/](http://localhost:5601/).
    - Currently, the **count** field in Kibana is of a _text_ property, so aggregations won't work. In order to do proper visualizations (_like bar plots_), you need to re-index the `count` field and cast it to an _integer_.
        1. Create conversion pipeline: `curl -XPUT "http://localhost:9200/_ingest/pipeline/convert_pipeline" -H 'Content-Type: application/json' -d '{"description":"Converts count field to integer","processors":[{"convert":{"field":"count","type":"integer"}}]}'`
        1. Re-index: `curl -XPOST "http://localhost:9200/_reindex" -H 'Content-Type: application/json' -d '{"source":{"index":"named_entities"},"dest":{"index":"named_entities_converted","pipeline":"convert_pipeline"}}'`
