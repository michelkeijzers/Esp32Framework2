import { defineConfig } from 'vite';

function apiRequestLogger() {
  return {
    name: 'api-request-logger',
    configureServer(server) {
      server.middlewares.use((req, _res, next) => {
        if (req.url && req.url.startsWith('/api/')) {
          console.log(`[vite-api] ${req.method} ${req.url}`);
        }
        next();
      });
    },
  };
}

export default defineConfig({
  plugins: [apiRequestLogger()],
  server: {
    proxy: {
      '/api': {
        target: 'http://127.0.0.1:8080',
        changeOrigin: true,
      },
    },
  },
});
