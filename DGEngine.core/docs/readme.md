# DGEngine core documentation

DGEngine core is a 2D game engine.
It uses JSON files as configuration files.

You run the engine by passing as an argument the path of the archive/folder with the gamefiles.

Run like this:

`DGEngine <gamefiles folder>`

An initial json file is loaded that starts a game. Usually it's `main.json` in the root folder.

### Portable mode

to run in portable mode, create a folder `portable` next to the DGEngine binary (current working directory).  

On Linux, if using an `AppImage`, create a folder with the same name as the AppImage ending with `.home`.

```
DGEngine.AppImage         (file)
DGEngine.AppImage.home    (folder)
```

### Configuration files

You can see more information about configuration files [here](configuration-files.md).
