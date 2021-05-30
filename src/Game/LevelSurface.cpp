#include "LevelSurface.h"
#include "LevelObject.h"

void LevelSurface::draw(const LevelObject& obj, GameShader* spriteShader,
	SpriteShaderCache& cache) const
{
	obj.draw(texture, spriteShader, cache);
}
