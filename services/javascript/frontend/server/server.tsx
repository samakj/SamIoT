/** @format */
import express from 'express';
import fs from 'fs';
import path from 'path';
import React from 'react';
import ReactDOMServer from 'react-dom/server';
import { App } from 'client/App';
import { StaticRouter } from 'react-router-dom';
import { ServerStyleSheet, StyleSheetManager } from 'styled-components';

const server = express();

server.set('view engine', 'ejs');
server.set('views', path.join(__dirname, 'views'));

server.use('/', express.static(path.join(__dirname, 'static')));

const manifest = fs.readFileSync(path.join(__dirname, 'static/manifest.json'), 'utf-8');
const assets = JSON.parse(manifest);

server.get('*', (request, response) => {
  const context: { [key: string]: string } = {};
  const stylesheet = new ServerStyleSheet();
  const component = ReactDOMServer.renderToString(
    <StyleSheetManager sheet={stylesheet.instance}>
      <App url={request.url} ssr />
    </StyleSheetManager>
  );
  const style = stylesheet.getStyleTags();
  stylesheet.seal();
  if (context.url) response.redirect(301, context.url);
  response.render('index', { assets, component, style });
});

server.listen(process.env.FRONTEND_PORT, () => {
  console.log(`Server running on http://${process.env.FRONTEND_HOST}:${process.env.FRONTEND_PORT}`);
});
