## Guide

# Build for production

```
npm run build
```

# Serve static file

Remember that `npm run dev` is just for development phase, when on production your need to build ReactJS server as static files, then serve it

```
cd dist (Or build, depend on what Asset Bundling you are using, in my case is Vite)
serve -s .
```

Then look what port it is serving on, then bind into the .lua config of ../connection
