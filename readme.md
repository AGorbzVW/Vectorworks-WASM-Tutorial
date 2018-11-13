
Instructions:
-------------

1. Install Emscripten:

    http://emscripten.org

2. Clone this repo:

    ```git clone https://github.com/AGorbzVW/Vectorworks-WASM-Tutorial```
    
    ```Vectorworks-WASM-Tutorial```
    
3. Build index.js and index.wasm:

    For the hello-world wasm tutorial, use this command:
    
    ```emcc hello.c -s WASM=1 -o hello.html```
    
    For the pong wasm tutorial, use the following command: 

    ```emcc main.cpp -std=c++11 -s WASM=1 -s USE_SDL=2 -O3 -o index.js```
    
    For debugging in the browser, use the following:
     ```emcc main.cpp -std=c++11 -s WASM=1 -s USE_SDL=2 -O3 -o index.js -g4 --source-map-base http://localhost:8080/```
     
     NOTE: This is only supported in builds of Firefox Nightly and Chrome Canary as of November 12, 2018.

4. Open index.html in a web browser.

    Chrome doesn't support file:// XHR requests, so you need to first start a webserver:

    ```python -m SimpleHTTPServer 8080```

    and then open this URL:

    http://localhost:8080/
