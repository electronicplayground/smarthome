FROM python:3.8


COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt


WORKDIR /app

COPY ./src/server.py /app
COPY ./src/mqtt_client.py /app
COPY ./src/configuration/ /app/configuration

CMD [ "python3", "-u", "/app/server.py" ]