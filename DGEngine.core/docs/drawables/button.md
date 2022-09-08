# Button

A `button` is a UI element that inherits from `image` or `text` and reacts to mouse input.
You have `BitmapButton`s and `StringButtons`s, depending on whether you use a `texture`,
`texturePack` or a `font`.  

Bitmap buttons are derived from `image` elements, so you use the same properties.  

String buttons are derived from `text` elements, so you use the same properties.

### Example

```json
"button": {
  "id": "button1",
  "font": "font1",
  "text": "goodbye",
  "onClick": { "name": "game.close" }
}
```

### Properties

#### BitmapButton

Bitmap Buttons share the same properties of `image` drawables.

#### StringButton

String Buttons share the same properties of `text` drawables.

#### Button

Name                 | Type            | Default | Description
-------------------- | --------------- | ------- | ----------------------------
`captureInputEvents` | bool or textArr | false   | capture input events. true captures all events
`clickUp`            | bool            | false   | fire click event on click release
`enable`             | bool            | true    | enable button click
`focusOnClick`       | bool            | true    | focus button on click
`focusSound`         | text            |         | focus sound
`focus`              | bool            | false   | enable focus
`onClickDrag`        | text            |         | click and drag action while mouse is not released
`onClickIn`          | text            |         | click in action
`onClickOut`         | text            |         | click out action
`onClick`            | text            |         | click action
`onDoubleClick`      | text            |         | double click action
`onFocus`            | text            |         | focus action
`onHoverEnter`       | text            |         | hover enter action
`onHoverLeave`       | text            |         | hover leave action
`onRightClick`       | text            |         | right click action
`sound`              | text            |         | click sound

Properties in **bold** are required.  

`captureInputEvents` captures input events when the drawable is updated. if the drawable is
on top of other drawables, it captures events so underlying drawables don't react to them.
This is useful to make fullscreen buttons that capture events of other UI elements when a prompt
is visible that needs user input.  

There is a list of valid `captureInputEvents`:

Name             | Description
---------------- | ----------------------------
`none`           | none
`leftClick`      | left click
`middleClick`    | middle click
`rightClick`     | right click
`mousePress`     | mouse press
`mouseMove`      | mouse move
`mouseRelease`   | mouse release
`mouseScroll`    | mouse scroll
`keyPress`       | key press
`textEnter`      | text enter
`touchBegin`     | touch begin
`touchMove`      | touch move
`touchEnd`       | touch end
`allLeftclick`   | all except left click
`allMiddleClick` | all except middle click
`allRightClick`  | all except right click
`all`            | all

### Examples

#### BitmapButton

```json
{
  "init": true,
  "texture": {
    "id": "red",
    "color": "0xFF0000",
    "size": [1, 1]
  },
  "button": {
    "id": "text1",
    "position": ["center", "center"],
    "texture": "red",
    "textureRect": [80, 20],
    "onClick": { "name": "game.close" }
  }
}
```

#### StringButton

```json
{
  "init": true,
  "button": {
    "id": "text1",
    "position": ["center", "center"],
    "font": "font1",
    "fontSize": 20,
    "text": "close",
    "onClick": { "name": "game.close" }
  }
}
```
