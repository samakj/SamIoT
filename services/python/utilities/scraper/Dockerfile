FROM python:3.10-alpine

ARG UTILITIES_SCRAPER_PORT

WORKDIR /app

COPY ./requirements.txt /app/requirements.txt

RUN pip install -r requirements.txt

COPY . /app

EXPOSE $UTILITIES_SCRAPER_PORT

CMD ["sh", "/app/run.sh"]