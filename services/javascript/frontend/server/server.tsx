/** @format */
import express from 'express';
import fs from 'fs';
import path from 'path';
import React from 'react';
import ReactDOMServer from 'react-dom/server';
import { App } from 'client/App';
import { ServerStyleSheet, StyleSheetManager } from 'styled-components';
import Cookies from 'universal-cookie';
import { DarkTheme, LightTheme } from 'client/style';
import { ThemeVariants } from 'client/style/types';
import { Provider } from 'react-redux';
import { createStore } from 'client/store';

const server = express();

server.set('view engine', 'ejs');
server.set('views', path.join(__dirname, 'views'));

server.use('/', express.static(path.join(__dirname, 'static')));

const manifest = fs.readFileSync(path.join(__dirname, 'static/manifest.json'), 'utf-8');
const assets = JSON.parse(manifest);

server.get('*', (request, response) => {
  const cookies = new Cookies(request.headers.cookie || request.headers.cookies);
  const theme = cookies.get('theme') === ThemeVariants.LIGHT ? LightTheme : DarkTheme;
  const store = createStore({ theme });
  const context: { [key: string]: string } = {};
  const stylesheet = new ServerStyleSheet();
  const component = ReactDOMServer.renderToString(
    <Provider store={store}>
      <StyleSheetManager sheet={stylesheet.instance}>
        <App url={request.url} ssr />
      </StyleSheetManager>
    </Provider>
  );
  const style = stylesheet.getStyleTags();
  const preloadedState = store.getState();
  stylesheet.seal();
  if (context.url) response.redirect(301, context.url);
  response.render('index', {
    assets,
    component,
    style,
    preloadedState: JSON.stringify(preloadedState).replace(/</g, '\\u003c'),
  });
});

server.listen(process.env.FRONTEND_PORT, () => {
  console.log(`Server running on http://${process.env.FRONTEND_HOST}:${process.env.FRONTEND_PORT}`);
});
