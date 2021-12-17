CREATE TABLE IF NOT EXISTS electricity (
    id                      SERIAL PRIMARY KEY,
    timestamp               TIMESTAMP NOT NULL UNIQUE,
    consumption             NUMERIC(8,4) NOT NULL
);