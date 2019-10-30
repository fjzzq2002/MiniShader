# MiniShader

A toy renderer written in C++.

Note that the samples in the samples folder might be rendered using old versions.

#### How to build

If gnu make is working for you, run `build.sh` to build.

Otherwise you can use `cmake-gui` to build or build manually.

#### Scenes

Open folder `scenes`, do for example `py -3 classroom.py >scene.json`.

When running, the shader will automatically read scene from `scenes\scene.json`.

#### Usage

If you only need rendering, input your desired ppm and the rendered image will be stored in `output.bmp`.

Otherwise, input 0 for ppm and a websocket server will be opened.

#### Client

Open `webclient.html` for interactive rendering. It will communicate with the program through websocket.

It's only tested under latest Firefox and Chrome. If it's not working try updating your browser.