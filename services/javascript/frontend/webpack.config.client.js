/** @format */

const path = require('path');
const { DefinePlugin } = require('webpack');
const { CleanWebpackPlugin } = require('clean-webpack-plugin');
const { WebpackManifestPlugin } = require('webpack-manifest-plugin');

module.exports = {
  name: 'client',
  entry: {
    client: path.resolve(__dirname, 'client/client.tsx'),
  },
  mode: 'production',
  output: {
    path: path.resolve(__dirname + '/build/static'),
    filename: '[name].[contenthash].js',
    publicPath: '',
  },
  resolve: {
    extensions: ['.ts', '.tsx', '.js'],
    alias: {
      client: path.resolve('./client'),
      server: path.resolve('./server'),
      shared: path.resolve('./shared'),
    },
  },
  target: 'web',
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
    new CleanWebpackPlugin(),
    new WebpackManifestPlugin(),
    new DefinePlugin({
      'process.env': JSON.stringify({
        IOT_API_HOST: process.env.IOT_API_HOST,
        IOT_API_PORT: process.env.IOT_API_PORT,
        UTILITIES_API_HOST: process.env.UTILITIES_API_HOST,
        UTILITIES_API_PORT: process.env.UTILITIES_API_PORT,
        WEATHER_API_HOST: process.env.WEATHER_API_HOST,
        WEATHER_API_PORT: process.env.WEATHER_API_PORT,
      }),
    }),
  ],
};
