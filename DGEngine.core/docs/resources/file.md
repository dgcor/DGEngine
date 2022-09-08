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

Name          | Type | Default | Description
------------- | ---- | ------- | ----------------------------
**`file`**    | text |         | resource file
`fromId`      | text |         | create an alias from an existing id

Properties in **bold** are required.  

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
