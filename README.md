# DGEngine

DGEngine is an implementation of the Diablo 1 game engine.

https://github.com/dgcor/DGEngine

See the wiki for more information.

https://github.com/dgcor/DGEngine/wiki

## Videos

<details><summary>Videos</summary>

### Diablo
https://user-images.githubusercontent.com/84206890/118375457-4e27c480-b5b1-11eb-8a11-241f9c136f53.mp4

### Hellfire
https://user-images.githubusercontent.com/84206890/118375467-5d0e7700-b5b1-11eb-8c8b-83eeab4f50d6.mp4

### Diablo Flare
https://user-images.githubusercontent.com/84206890/118375470-60096780-b5b1-11eb-8d25-89d8e272d4ba.mp4

### Diablo Extended
https://user-images.githubusercontent.com/84206890/118375472-63045800-b5b1-11eb-9872-1016610e063f.mp4

### Diablo 2
https://user-images.githubusercontent.com/84206890/118375560-d908bf00-b5b1-11eb-99c7-5e57673529b6.mp4

</details>

## Install

You need these files to run the engine:

* DGEngine
* gamefilesd
* gamefilesd2
* gamefilesdex
* gamefilesflare
* gamefileshf
* main.json

<details><summary>Diablo</summary>

To run **Diablo**, you need:

* DIABDAT.MPQ

Run the game like this:

`DGEngine gamefilesd` or `DGEngine` to launch `main.json`

</details>

<details><summary>Hellfire</summary>

To run **Hellfire**, you need:

* DIABDAT.MPQ
* hellfire.mpq
* hfmonk.mpq
* hfmusic.mpq
* hfvoice.mpq

Run the game like this:

`DGEngine gamefileshf` or `DGEngine` to launch `main.json`

</details>

<details><summary>Diablo Flare</summary>

To run **Diablo Flare**, you need:

* empyrean_campaign
* fantasycore

You can get these files from [here](https://github.com/flareteam/flare-game/releases/tag/v1.11).

Run the game like this:

`DGEngine gamefilesflare` or `DGEngine` to launch `main.json`

</details>

<details><summary>Diablo Extended</summary>

To run **Diablo Extended**, you need:

* DIABDAT.MPQ
* empyrean_campaign
* fantasycore

Run the game like this:

`DGEngine gamefilesdex`

</details>

<details><summary>Diablo 2</summary>

To run **Diablo 2**, you need:

* DIABDAT.MPQ
* d2char.mpq
* d2data.mpq
* d2music.mpq
* d2sfx.mpq
* d2speech.mpq
* d2video.mpq

You have to use diablo 2's gamefiles extracted into folders.
Adding an external [StormLib.dll or libstorm.so](https://github.com/ladislav-zezula/StormLib)
next to DGEngine will add support for Diablo 2 MPQ files.

Run the game like this:

`DGEngine gamefilesd2`

</details>

### Portable mode

To run in portable mode, create a folder `portable` next to the DGEngine binary (current working directory).  

On Linux, if using an `AppImage`, create a folder with the same name as the AppImage ending with `.home`.

```
DGEngine.AppImage         (file)
DGEngine.AppImage.home    (folder)
```

## Build

See how to build in [BUILD.txt](BUILD.txt).

## License

DGEngine is dual licensed ([LICENSE.txt](LICENSE.txt)).
If you use diablo file formats support (CEL/CL2/COF/DC6/DCC)
the license is GPL3. If you don't use these formats, the license is zLib.
