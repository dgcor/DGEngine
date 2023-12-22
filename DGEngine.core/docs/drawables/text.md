# Text

A `text` is a UI element that draws a text using a `font`. You have `BitmapText`s
and `StringText`s, depending on whether you use a `BitmapFont` or a `FreeTypeFont`.  

You can specify `binding` variables that are evaluated at runtime and the text is updated.

### Example

```json
"text": {
  "id": "text1",
  "font": "font1",
  "text": "hello"
}
```

### Properties

Name                    | Type    | Default            | Description
----------------------- | ------- | ------------------ | ----------------------------
`color`                 | color   | white              | text color
`file`                  | text    |                    | file to load text from
**`font`**              | text    |                    | font to use
`fontSize`              | int     | 12 or font default | font size (TrueType)
`horizontalAlign`       | int     | left               | horizontal align text. `left`, `center` or `right`
`horizontalSpaceOffset` | int     | 0                  | horizontal space offset between lines
`onChange`              | text    |                    | on text change action
`text`                  | text    |                    | text to display
`wordWrap`              | int     |                    | split text when lines have more than n chars
`verticalAlign`         | color   | bottom             | vertical align text. `top`, `center` or `bottom`
`verticalSpaceOffset`   | int     | 0                  | vertical space offset between chars

Properties in **bold** are required.  

Once a text is created with one type of font (Bitmap, TrueType), you can't change the font to another type.  

Horizontal and vertical aligned text use the position as reference. A text at position [0,0] that
is left aligned starts at the left edge of the game window. A text at position [640,0] that is
right aligned starts at the right edge of the game window.  

#### Binding properties

Name           | Type    | Default  | Description
-------------- | ------- | -------- | ----------------------------
`binding`      | textArr |          | binding variables
`bindingFlags` | text    | onChange | binding flags. `onChange`, `once`, `always` and `whenHidden`
`bindToFormat` | bool    | true     | use format property to bind or the text/file property
`format`       | text    | [1]      | binding text format

Binding variables are evaluated at runtime. they can contain queries. All text elements are created
as `BindingText` drawables, so they can be updated to use `binding` variables after being created.  

When the `once` and `always` bindingFlags are set, the binding is always refreshed.

BindingFlag  | Description
------------ | ----------------------------
`always`     | always evaluate binding variables and update the display text
`once`       | only evaluate binding variables once
`onChange`   | update text when binding variable's values change
`whenHidden` | update text when the control is not visible

### Examples

#### BitmapText

```json
{
  "init": true,
  "text": {
    "id": "text1",
    "font": "bitmapFont1",
    "text": "hello"
  }
}
```

#### TrueTypeText

```json
{
  "init": true,
  "text": {
    "id": "text1",
    "font": "trueTypeFont1",
    "fontSize": 20,
    "text": "hello"
  }
}
```

#### Red text

```json
{
  "init": true,
  "text": {
    "id": "text1",
    "font": "trueTypeFont1",
    "color": "0xFF0000",
    "text": "hello"
  }
}
```

#### Binding text that displays text position

```json
{
  "init": true,
  "text": {
    "id": "text1",
    "font": "font",
    "position": ["center", "center"],
    "binding": ["%text1.size.x%", "%text1.size.y%"],
    "format": "text1 is at position [1], [2]"
  }
}
```

#### Binding text that displays text position in the center of the screen

```json
{
  "init": true,
  "text": {
    "id": "text1",
    "font": "font",
    "position": ["center", "center"],
    "horizontalAlign": "center",
    "binding": ["%text1.size.x%", "%text1.size.y%"],
    "format": "text1 is at position [1], [2]"
  }
}
```

#### Binding text that displays text position in the right corner of the screen

```json
{
  "init": true,
  "text": {
    "id": "text1",
    "font": "font",
    "position": [640, "center"],
    "horizontalAlign": "right",
    "binding": ["%text1.size.x%", "%text1.size.y%"],
    "format": "text1 is at position [1], [2]"
  }
}
```

#### Binding text loaded from a file

```json
{
  "init": true,
  "text": {
    "id": "text1",
    "font": "font",
    "position": [640, "center"],
    "horizontalAlign": "right",
    "bindToFormat": false,
    "binding": ["%text1.size.x%", "%text1.size.y%"],
    "file": "help/textPosition.txt"
  }
}
```
