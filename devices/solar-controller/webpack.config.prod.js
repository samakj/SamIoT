/** @format */

const path = require('path');
const webpack = require('webpack');
const { CleanWebpackPlugin } = require('clean-webpack-plugin');
const CompressionPlugin = require('compression-webpack-plugin');

module.exports = {
  name: 'client',
  entry: {
    client: path.resolve(__dirname, 'client/client.tsx'),
  },
  mode: 'production',
  output: {
    path: path.resolve(__dirname + '/data/js'),
    filename: '[name].js',
    publicPath: '',
  },
  resolve: {
    extensions: ['.ts', '.tsx', '.js'],
    alias: {
      client: path.resolve('./client'),
      shared: path.resolve('./shared'),
      // Preact override
      react: 'preact/compat',
      'react-dom/test-utils': 'preact/test-utils',
      'react-dom': 'preact/compat',
      'react/jsx-runtime': 'preact/jsx-runtime',
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
    new webpack.DefinePlugin({
      'process.env': JSON.stringify({
        IOT_API_HOST: process.env.IOT_API_HOST,
        IOT_API_PORT: process.env.IOT_API_PORT,
        UTILITIES_API_HOST: process.env.UTILITIES_API_HOST,
        UTILITIES_API_PORT: process.env.UTILITIES_API_PORT,
        WEATHER_API_HOST: process.env.WEATHER_API_HOST,
        WEATHER_API_PORT: process.env.WEATHER_API_PORT,
      }),
    }),
    // new webpack.optimize.UglifyJsPlugin(),
    new webpack.optimize.AggressiveMergingPlugin(),
    new CompressionPlugin(),
  ],
};
