# Game

The `game` drawable is the UI element that handles the whole game engine.
It is responsible for listening for user input, drawing and updating.
The game element is automatically created when starting the engine and
can be configured using a number of properties and actions.

### Properties

Name           | Type   | Default   | Description
-------------- | ------ | --------- | ----------------------------
`framerate`    | int    | 60        | framerate in frames per second. Value between 30 and 240. 0 to disable.
`fullsceen`    | bool   | false     | fullscreen window
`icon`         | text   |           | load window icon image path
`init`         | bool   | false     | initialize the `game` and show the window
`keepAR`       | bool   | true      | keeps the aspect ratio when resizing and `stretchToFit` is `true`
`maxHeight`    | int    | 0         | maximum window height before stretching content. Value >= minWindowSize.Y. 0 to disable
`minSize`      | intVec | [640,480] | minimum window size
`onClose`      | text   |           | close action. If no action is defined, the game is closed, otherwise the action should close the game
`onFocusGain`  | text   |           | focus gain action
`onFocusLoss`  | text   |           | focus loss action
`onResize`     | text   |           | resize action
`position`     | intVec |           | window position
`refSize`      | intVec | [640,480] | reference window size
`saveDir`      | text   |           | save directory
`smoothScreen` | bool   | false     | smooth the screen when stretched
`stretchToFit` | bool   | false     | stretches the window content when resizing
`title`        | text   |           | window's title
`version`      | text   |           | version
`size`         | intVec | [640,480] | window size
`windowedSize` | intVec | [640,480] | windowed size when not in fullscreen (read only)

Properties in **bold** are required.  

### Example

```json
{
  "game": {
    "title" : "DGEngine",
    "version": "1.0",
    "refSize": [640, 480],
    "minSize": [800, 600],
    "size": [800, 600],
    "framerate": 60,
    "keepAR": true,
    "stretchToFit": true,
    "smoothScreen": true,
    "init": true
  }
}
```
