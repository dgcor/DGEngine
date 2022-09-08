# Variable

A `variable` is a global key/value map of values.

### Example

```json
"variable": [
  { "colorCycleSpeed": 50 },
  { "textGold": "0xEDD689" }
]
```

### Properties

Name        | Type                 | Default | Description
----------- | -------------------- | ------- | ----------------------------
**`<key>`** | bool/int/double/text |         | key is used as the variable key

Properties in **bold** are required.  

### Examples

#### Set variables

```json
"variable": [
  { "boolean": true },
  { "number": 50 },
  { "double": 0.22 },
  { "text": "DGEngine" }
]
```
