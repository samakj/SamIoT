FROM node:17.8-alpine

ARG FRONTEND_PORT

WORKDIR /app

COPY ./package.json /app/package.json
COPY ./package-lock.json /app/package-lock.json

RUN npm install

COPY . /app


EXPOSE $FRONTEND_PORT

CMD ["npm", "run", "start:dev"]