# Shader

A `shader` is a resource that applies an OpenGL shader to `game`, `level` and `sprite` drawables.

### Example

```json
"shader": {
  "id": "grain",
  "fragmentFile": "res/shaders/game/grain.frag",
  "uniforms": ["elapsedTime", "fade", "gamma"],
  "setShader": "game"
}
```

### Properties

Name               | Type          | Default | Description
------------------ | ------------- | ------- | ----------------------------
**`fragmentFile`** | text          |         | fragment shader file
`geometryFile`     | text          |         | geometry shader file
`vertexFile`       | text          |         | vertex shader file
`uniforms`         | text or array |         | list of uniforms to pass to the shader. `texture` is always passed
`setShader`        | text          |         | set the current shader for `game`, `level` and `sprite`

Properties in **bold** are required.  

There are 3 types of shaders:

Shader type | Description
----------- | ----------------------------
`game`      | shader applied to the whole game window
`level`     | shader applied to the level area, if implemented
`sprite`    | shader applied to drawables based on DGEngine's `Sprite` or `CompositeSprite`

The `game` shader does the fade in/out effect and applies gamma to the game window.  

The `sprite` shader applies a palette to a texture, if that texture needs one.  

You need to specify in `uniforms` which uniforms to pass to the shader.  

There are a number of uniforms that can be passed to shaders:

Uniform         | Type      | Shaders       | Description
--------------- | --------- | ------------- | ----------------------------
`elapsedTime`   | float     | game          | elapsed time since game started in seconds
`fade`          | vec4      | game          | current fade color with fade value in the alpha channel
`gamma`         | float     | game          | current gamma value from 0 to 100
`hasPalette`    | bool      | sprite        | bool that indicates if the palette texture is set. *used with palette*
`ignore`        | vec4      | sprite        | color to ignore. *used with outline*
`loading`       | float     | game          | current loading state from 0 (not loaded) to 1 (fully loaded)
`mousePosition` | vec2      | game          | current mouse position from [0,0] to [1,1]
`outline`       | vec4      | sprite        | color to outline/highlight
`palette`       | sampler2D | sprite        | 256 color palette to apply
`pixelSize`     | vec2      | sprite        | pixel size of the input texture
`textureSize`   | vec2      | game, sprite  | current texture size

Images that use palettes should have the palette index to use in the `red` channel and no alpha.
Only apply the palette to images with alpha equal to 1.

### Default fragment shaders

#### Game

```glsl
#version 110
uniform sampler2D texture;
uniform vec4 fade;
uniform float gamma;

vec3 Gamma(vec3 value, float param)
{
    return vec3(pow(abs(value.r), param), pow(abs(value.g), param), pow(abs(value.b), param));
}

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy) - fade;
    float gamma2 = 1.0 - (0.007 * gamma);
    gl_FragColor = vec4(Gamma(pixel.rgb, gamma2), pixel.a);
}
```

#### Level

```glsl
#version 110
uniform sampler2D texture;

void main()
{
    gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
}
```

#### Sprite

```glsl
#version 110
uniform sampler2D palette;
uniform sampler2D texture;
uniform vec2 pixelSize;
uniform vec4 outline;
uniform vec4 ignore;
uniform bool hasPalette;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    if (hasPalette == true && pixel.a == 1.0)
    {
        pixel = texture2D(palette, vec2(pixel.r, 0.0));
    }

    if (outline.a > 0.0 && pixel.a == 0.0)
    {
        vec4 vu = texture2D(texture, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y - pixelSize.y));
        vec4 vd = texture2D(texture, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y + pixelSize.y));
        vec4 vl = texture2D(texture, vec2(gl_TexCoord[0].x - pixelSize.x, gl_TexCoord[0].y));
        vec4 vr = texture2D(texture, vec2(gl_TexCoord[0].x + pixelSize.x, gl_TexCoord[0].y));

        if (vu.a == 1.0 || vd.a == 1.0 || vl.a == 1.0 || vr.a == 1.0)
        {
            if (ignore.a == 0.0)
            {
                pixel = outline;
            }
            else
            {
                if (vu != ignore &&
                    vd != ignore &&
                    vl != ignore &&
                    vr != ignore)
                {
                    pixel = outline;
                }
            }
        }
    }
    gl_FragColor = pixel;
}
```

### Examples

#### Load fragment shader that uses palette and outline

```json
{
  "init": true,
  "shader": {
    "id": "highlight",
    "fragmentFile": "res/shaders/sprite/highlight.frag",
    "uniforms": ["palette", "outline"]
  }
}
```

#### Load and set sprite fragment shader that uses palette and outline

The `hasPalette` and `ignore` uniforms are used implicitly.

```json
{
  "init": true,
  "shader": {
    "id": "highlight",
    "fragmentFile": "res/shaders/sprite/highlight.frag",
    "uniforms": ["palette", "outline"],
    "setShader": "sprite"
  }
}
```
