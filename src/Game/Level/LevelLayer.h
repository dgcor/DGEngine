#pragma once

#include "LevelLayers/ColorLevelLayer.h"
#include "LevelLayers/TextureLevelLayer.h"
#include "LevelLayers/TilesetLevelLayer.h"
#include <variant>

struct LevelLayer
{
private:
	using LevelLayerVariant = std::variant<
		std::nullptr_t,
		TilesetLevelLayer,
		TextureLevelLayer,
		ColorLevelLayer>;

public:
	LevelLayerVariant layer;
	sf::FloatRect viewportOffset;
	bool automap{ false };

	LevelLayer() {}
	LevelLayer(const LevelLayerVariant& layer_,
		const sf::FloatRect& viewportOffset_, bool automap_)
		: layer(layer_), viewportOffset(viewportOffset_), automap(automap_) {}
};

template <class... T>
constexpr bool holdsNullLevelLayer(const std::variant<T...>& v) noexcept { return std::holds_alternative<std::nullptr_t>(v); }

template <class... T>
constexpr bool holdsColorLevelLayer(const std::variant<T...>& v) noexcept { return std::holds_alternative<ColorLevelLayer>(v); }

template <class... T>
constexpr bool holdsTextureLevelLayer(const std::variant<T...>& v) noexcept { return std::holds_alternative<TextureLevelLayer>(v); }

template <class... T>
constexpr bool holdsTilesetLevelLayer(const std::variant<T...>& v) noexcept { return std::holds_alternative<TilesetLevelLayer>(v); }
