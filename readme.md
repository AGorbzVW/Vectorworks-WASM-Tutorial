

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


MIT license:
----------------

```
Copyright (c) 2018 Tim Hutton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
