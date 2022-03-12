# Problem B-2 — Patient list

This repository connets to a public websocket of patient data and requests data for patients.

# Requirements

* `c++ 17` as a minimum.
* `make`. Technically, the commands from the make file can be typed manually, but it's easier to have `make` installed.
* `gcc`. Clang can work too, you just need to replace the CC variable in the Makefile.

2 libraries are used, but they are build along with the source code, so no need to worry about anything.

* [`json`](https://github.com/nlohmann/json)
* [`easywebsocket`](https://github.com/dhbaird/easywsclient)

# Usage

* `make` or `make help` - will display a help for all available targets.
* `make build` - creates a build directory and builds a single binary from the source files. This takes some time thanks to `json.hpp`...
* `make patient_list` - connects to <span style="text-decoration: underline">ws://apply.viewray.ai:4645</span> and list brief information about all patients.
* `make patient_list_with_details` - connects to <span style="text-decoration: underline">ws://apply.viewray.ai:4645</span> and list detailed information about all patients, their diagnoses, prescriptions and treatment plans.

# Demo GIF

![Demo Recording Gif](./docs/demo.gif "Demo usage")
