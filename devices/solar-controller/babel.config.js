/** @format */

module.exports = (api) => {
  api.cache(true);

  return {
    presets: ['react-app'],
    plugins: [
      'babel-plugin-styled-components',
      [
        '@babel/plugin-transform-react-jsx',
        {
          runtime: 'automatic',
          importSource: 'preact',
        },
      ],
      [
        'module-resolver',
        {
          root: ['.'],
          alias: {
            react: 'preact/compat',
            'react-dom': 'preact/compat',
          },
        },
      ],
    ],
  };
};
