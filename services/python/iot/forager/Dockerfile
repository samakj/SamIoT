FROM python:3.10-alpine

ARG IOT_FORAGER_PORT

WORKDIR /app

COPY ./requirements.txt /app/requirements.txt

RUN pip install -r requirements.txt

COPY . /app

EXPOSE $IOT_FORAGER_PORT

CMD ["sh", "/app/run.sh"]