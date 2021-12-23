/** @format */

const nodeExternals = require('webpack-node-externals');
const path = require('path');
const CopyPlugin = require('copy-webpack-plugin');

module.exports = {
  name: 'server',
  entry: {
    server: path.resolve(__dirname, 'server/server.tsx'),
  },
  mode: 'production',
  output: {
    path: path.resolve(__dirname, 'build'),
    filename: '[name].js',
  },
  resolve: {
    extensions: ['.ts', '.tsx'],
    alias: {
      client: path.resolve('./client'),
      server: path.resolve('./server'),
      shared: path.resolve('./shared'),
    },
  },
  externals: [nodeExternals()],
  target: 'node',
  node: {
    __dirname: false,
  },
  module: {
    rules: [
      {
        test: /\.tsx?$/,
        exclude: /node_modules/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: ['@babel/preset-env'],
          },
        },
      },
    ],
  },
  plugins: [
    new CopyPlugin({
      patterns: [
        // { from: 'assets', to: 'static' },
        { from: 'server/views', to: 'views' },
      ],
    }),
  ],
};
