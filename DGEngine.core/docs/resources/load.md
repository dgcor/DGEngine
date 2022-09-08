# Load

A `load` property loads a json file and processes it.

### Example

```json
"load": "main.json"
```

### Properties

Name        | Type          | Default | Description
----------- | ------------- | ------- | ----------------------------
**`value`** | text or array |         | path of file to load or array with path and arguments

Properties in **bold** are required.  

A `load` property can load a file and pass a list of parameters.  

All parameters of load properties are processed and `%variable%` variables are replaced
with the current value, if set, before loading.  

The file to load is processed to replace occurrences of [1] to [N] with the
parameter that was passed in the 1st to the Nth position.

### Examples

#### Load a file

```json
{
  "load": "lang/%lang%/level.json"
}
```

#### Load a file and pass 2 parameters

```json
{
  "load": [
    "ui/showTextBox.json",
    "Text box title",
    "lang/%lang%/quests/main.txt"
  ]
}
```
