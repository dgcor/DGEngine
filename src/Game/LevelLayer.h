#pragma once

#include "ColorLevelLayer.h"
#include "TextureLevelLayer.h"
#include "TilesetLevelLayer.h"
#include <variant>

struct LevelLayer
{
private:
	typedef std::variant<
		std::nullptr_t,
		TilesetLevelLayer,
		TextureLevelLayer,
		ColorLevelLayer>
		LevelLayerVariant;

public:
	LevelLayerVariant layer;
	sf::FloatRect viewportOffset;
	bool automap{ false };

	LevelLayer() {}
	LevelLayer(const LevelLayerVariant& layer_,
		const sf::FloatRect& viewportOffset_, bool automap_)
		: layer(layer_), viewportOffset(viewportOffset_), automap(automap_) {}
};

#define holdsNullLevelLayer std::holds_alternative<std::nullptr_t>
#define holdsTilesetLevelLayer std::holds_alternative<TilesetLevelLayer>
#define holdsTextureLevelLayer std::holds_alternative<TextureLevelLayer>
#define holdsColorLevelLayer std::holds_alternative<ColorLevelLayer>
