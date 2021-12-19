/** @format */
import express from 'express';
import fs from 'fs';
import path from 'path';
import React from 'react';
import ReactDOMServer from 'react-dom/server';
import { App } from 'client/App';

const server = express();

server.set('view engine', 'ejs');
server.set('views', path.join(__dirname, 'views'));

server.use('/', express.static(path.join(__dirname, 'static')));

const manifest = fs.readFileSync(path.join(__dirname, 'static/manifest.json'), 'utf-8');
const assets = JSON.parse(manifest);

server.get('/', (_request, response) => {
  const component = ReactDOMServer.renderToString(React.createElement(App));
  response.render('index', { assets, component });
});

server.listen(process.env.FRONTEND_PORT, () => {
  console.log(`Server running on http://${process.env.FRONTEND_HOST}:${process.env.FRONTEND_PORT}`);
});
