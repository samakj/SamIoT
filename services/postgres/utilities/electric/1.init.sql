CREATE TABLE IF NOT EXISTS electric (
    id                      SERIAL PRIMARY KEY,
    timestamp               TIMESTAMP NOT NULL UNIQUE,
    consumption             NUMERIC(8,4) NOT NULL
);