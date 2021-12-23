/** @format */

module.exports = (api) => {
  api.cache(true);

  return {
    presets: ['react-app'],
    plugins: ['babel-plugin-styled-components'],
  };
};
