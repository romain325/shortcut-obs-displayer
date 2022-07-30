# Shortcut OBS Displayer

**LINUX ONLY -- based on /dev/input**

This extension listen to your keyboards entry and display them as widget in OBS  

## Dependencies

```shell
sudo apt install libevdev-dev cmake node python3
```

## Installation 

Git clone this project, in the cmake-build-debug folder run `cmake ..`, run make and then you can run keyboard_listener with sudo as it reads /dev/input  
Once run go back to the obsBrowserPage, use `npm i && npm run build`  
You can now open a web server (personnaly with `python3 -m http.server port`) and reference this webserver in OBS  
Your shortcuts will now be displayed as you type them

## Lazyness

- add a webserver instead of python3 http.server
- Configure which device you want to read in C !! (hardcoded /dev/input/device8)
- Configure if you want to be azerty or not