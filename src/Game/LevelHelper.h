#include "Level.h"
#include "Min.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace LevelHelper
{
	std::vector<std::shared_ptr<sf::Texture>> loadTilesetSprite(CelFrameCache& cel, Min& min, bool top);
}
