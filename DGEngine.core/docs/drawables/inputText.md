# InputText

An `inputText` is a UI element that captures keyboard input and displays it.
Input texts are derived from `text` elements, so you use the same properties,
except the `binding` properties.

### Example

```json
"inputText": {
  "id": "name",
  "font": "font1",
  "minLength": 1,
  "maxLength": 15,
  "regex": "[\\w ]*",
  "onClick": { "name": "load", "file": "level/newGame.json" },
  "onMinLength": { "name": "load", "file": "ui/singlePlayer/newNameTooShort.json" }
}
```

### Properties

Name          | Type     | Default | Description
------------- | -------- | ------- | ----------------------------
`minLength`   | int      |         | minimum text length
`maxLength`   | int      |         | maximum text length
`minValue`    | variable |         | minimum value
`maxValue`    | variable |         | maximum value
`regex`       | text     |         | input validation regex
`onClick`     | text     |         | action to execute on enter key if the input is valid
`onMinLength` | int      |         | action to execute on enter key if the length is smaller than the minimum length

Properties in **bold** are required.  

The `minValue` and `maxValue` check if the input is included (value >= min and value <= max)
and ignore input text that invalidates this condition.  

If you set a `minValue` of 10 on an empty `inputText`, you won't be able to type in any value.  

Regex expressions, if set, prevent the you from typing values that don't satisfy the final text value.
Use them only to filter out certain chars. Don't define regexes that require a minimum length.
Use `minLength` instead.  

The `onClick` and `onMinLength` actions are executed when the `inputText.click` action is executed.

### Examples

#### InputText with a length between 1 and 15 chars

```json
{
  "init": true,
  "inputText": {
    "id": "name",
    "font": "font",
    "minLength": 1,
    "maxLength": 15,
    "onClick": { "name": "game.close" },
    "onMinLength": { "name": "game.close" }
  }
}
```

#### InputText that only accepts numbers bigger than 0

```json
{
  "init": true,
  "inputText": {
    "id": "name",
    "font": "font",
    "regex": "[1-9]\\d*",
    "onClick": { "name": "game.close" },
    "onMinLength": { "name": "game.close" }
  }
}
```

#### InputText that only accepts numbers between 1 and 9000

```json
{
  "init": true,
  "inputText": {
    "id": "name",
    "font": "font",
    "minLength": 1,
    "maxLength": 4,
    "maxValue": 9000,
    "regex": "[1-9]\\d*",
    "onClick": { "name": "game.close" },
    "onMinLength": { "name": "game.close" }
  }
}
```
