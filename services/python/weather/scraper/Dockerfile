FROM python:3.10-alpine

ARG WEATHER_SCRAPER_PORT

WORKDIR /app

COPY ./requirements.txt /app/requirements.txt

RUN pip install -r requirements.txt

COPY . /app

EXPOSE $WEATHER_SCRAPER_PORT

CMD ["sh", "/app/run.sh"]