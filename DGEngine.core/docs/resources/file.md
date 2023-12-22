# File

A `file` is a resource that stores an array of data in memory.

### Example

```json
"file": {
  "id": "file1",
  "file": "data.bin"
}
```

### Properties

Name            | Type | Default | Description
--------------- | ---- | ------- | ----------------------------
**`file`**      | text |         | resource file
`fromId`        | text |         | create an alias from an existing id
`startPosition` | int  | 0       | start position in bytes
`bytesToRead`   | int  | max     | number of bytes to read

Properties in **bold** are required.  

The number of bytes read can be less than `bytesToRead` if the end of
the file is reached.

### Examples

#### File

```json
{
  "init": true,
  "file": {
    "id": "file1",
    "file": "data.bin"
  }
}
```
