FROM python:3.10-alpine

ARG SOLAR_MANAGER_PORT

WORKDIR /app

COPY ./requirements.txt /app/requirements.txt

RUN pip install -r requirements.txt

COPY . /app

EXPOSE $SOLAR_MANAGER_PORT

CMD ["sh", "/app/run.sh"]