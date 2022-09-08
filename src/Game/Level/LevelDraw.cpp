#include "LevelDraw.h"
#include "Game/Drawables/Panel.h"
#include "Game/Game.h"
#include "Level.h"
#include "LevelLayer.h"

void LevelDraw::draw(const Level& level, const Game& game, sf::RenderTarget& target)
{
	if (level.visible == false)
	{
		return;
	}

	auto origView = target.getView();

	// layers

	level.surface.clear(sf::Color::Black);
	level.automapSurface.clear(sf::Color::Transparent);

	SpriteShaderCache spriteCache;

	for (size_t i = 0; i < level.levelLayers.size(); i++)
	{
		const auto& levelLayer = level.levelLayers[i];

		if (holdsNullLevelLayer(levelLayer.layer) == true)
		{
			continue;
		}
		if (levelLayer.automap == false)
		{
			if (level.surface.visible == false)
			{
				continue;
			}
		}
		else
		{
			if (level.automapSurface.visible == false)
			{
				continue;
			}
		}

		auto& layerSurface = (levelLayer.automap == false ? level.surface : level.automapSurface);

		layerSurface.updateDrawView(levelLayer.viewportOffset);

		if (holdsTilesetLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<TilesetLevelLayer>(levelLayer.layer);
			layer.draw(layerSurface, spriteCache,
				game.Shaders().Sprite, level,
				i == level.indexToDrawLevelObjects,
				levelLayer.automap
			);
		}
		else if (holdsTextureLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<TextureLevelLayer>(levelLayer.layer);
			layer.draw(layerSurface);
		}
		else if (holdsColorLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<ColorLevelLayer>(levelLayer.layer);
			layer.draw(layerSurface);
		}
	}

	// lighting

	if (level.lights.empty() == false && level.map.getDefaultLight() < 255)
	{
#if SFML_VERSION_MAJOR >= 2 && SFML_VERSION_MINOR >= 6
		const static sf::BlendMode lightBlend(
			sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add,
			sf::BlendMode::One, sf::BlendMode::Zero, sf::BlendMode::Min
		);
#else
		const static sf::BlendMode lightBlend(
			sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add,
			sf::BlendMode::DstAlpha, sf::BlendMode::Zero, sf::BlendMode::Add
		);
#endif

		sf::RenderStates lightStates;
		lightStates.blendMode = lightBlend;

		for (const auto& light : level.lights)
		{
			level.surface.draw(light, lightStates);
		}
	}

	auto surfaceStates(sf::RenderStates::Default);
	if (level.gameShader != nullptr)
	{
		auto shader = level.gameShader->shader.get();
		surfaceStates.shader = shader;
		for (auto uniformHash : level.gameShader->uniforms)
		{
			switch (uniformHash)
			{
			case str2int16("elapsedTime"):
			{
				shader->setUniform("elapsedTime", game.getTotalElapsedTime().asSeconds());
				break;
			}
			case str2int16("mousePosition"):
			{
				if (level.hasMouseInside == true)
				{
					shader->setUniform("mousePosition", sf::Glsl::Vec2(
						(game.MousePositionf().x - level.surface.Position().x) /
						level.surface.Size().x,
						(game.MousePositionf().y - level.surface.Position().y) /
						level.surface.Size().y
					));
				}
				break;
			}
			case str2int16("textureSize"):
			{
				shader->setUniform("textureSize", sf::Glsl::Vec2(
					level.surface.Size().x,
					level.surface.Size().y
				));
				break;
			}
			case str2int16("visibleRect"):
			{
				shader->setUniform("visibleRect", sf::Glsl::Vec4(
					level.surface.visibleRect.left,
					level.surface.visibleRect.top,
					level.surface.visibleRect.width,
					level.surface.visibleRect.height
				));
				break;
			}
			case str2int16("defaultLight"):
			{
				shader->setUniform("defaultLight", (float)(255 - level.map.getDefaultLight()) / 255);
				break;
			}
			default:
				break;
			}
		}
	}
	level.surface.draw(target, surfaceStates);

	// level drawables

	if (level.drawables.empty() == false)
	{
		if (level.zoomDrawables == true)
		{
			level.surface.clear(sf::Color::Transparent);
			bool hasDrawn = false;
			for (const auto& item : level.drawables)
			{
				if (auto obj = item.drawable.lock())
				{
					hasDrawn |= level.surface.draw(game, *obj);
				}
			}
			if (hasDrawn == true)
			{
				level.surface.draw(target);
			}
		}
		else
		{
			for (const auto& item : level.drawables)
			{
				if (auto obj = item.drawable.lock())
				{
					obj->draw(game, target);
				}
			}
		}
	}

	// automap

	level.automapSurface.draw(target, sf::RenderStates::Default);

	target.setView(origView);
}
