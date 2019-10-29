# MiniShader

A toy shader written in C++.

#### How to build

If you're under Linux system, run `build.sh` to build.

Otherwise use `cmake-gui` to build.

#### Scenes

Open folder `scenes`, do for example `py -3 classroom.py >scene.json`.

When running, the shader will automatically read scene from `scenes\scene.json`.

#### Client

Open `test.html` for interactive shading.

The client will communicate with shader through websocket.