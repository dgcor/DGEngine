#pragma once

#include "Action.h"
#include "Game.h"
#include "GameUtils.h"
#include "Game/Level.h"

class ActLevelAddColorLayer : public Action
{
private:
	std::string id;
	sf::Color color;
	sf::FloatRect viewportOffset;
	bool automap;

public:
	ActLevelAddColorLayer(const std::string_view id_, sf::Color color_,
		const sf::FloatRect& viewportOffset_, bool automap_) : id(id_),
		color(color_), viewportOffset(viewportOffset_), automap(automap_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->addLayer(color, viewportOffset, automap);
		}
		return true;
	}
};

class ActLevelAddTextureLayer : public Action
{
private:
	std::string id;
	std::string idTexture;
	sf::IntRect textureRect;
	sf::FloatRect viewportOffset;
	bool automap;

public:
	ActLevelAddTextureLayer(const std::string_view id_, const std::string_view idTexture_,
		const sf::IntRect& textureRect_, const sf::FloatRect& viewportOffset_,
		bool automap_) : id(id_), idTexture(idTexture_), textureRect(textureRect_),
		viewportOffset(viewportOffset_), automap(automap_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			auto texture = game.Resources().getTexture(idTexture);
			if (texture != nullptr)
			{
				TextureLevelLayer layer(texture);
				layer.textureRect = textureRect;
				level->addLayer(layer, viewportOffset, automap);
			}
		}
		return true;
	}
};

class ActLevelAnchorDrawable : public Action
{
private:
	std::string id;
	std::string idDrawable;
	std::string idAnchor;
	sf::Vector2f offset;

public:
	ActLevelAnchorDrawable(const std::string_view id_, const std::string_view idDrawable_,
		const std::string_view idAnchor_, const sf::Vector2f& offset_) : id(id_),
		idDrawable(idDrawable_), idAnchor(idAnchor_), offset(offset_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			auto drawable = level->getLevelDrawable(idDrawable);
			if (drawable != nullptr)
			{
				drawable->anchorTo = level->getLevelObjectPtr(idAnchor);
				drawable->offset = offset;
			}
		}
		return true;
	}
};

class ActLevelClearAllObjects : public Action
{
private:
	std::string id;
	std::vector<std::string> excludeIds;

public:
	ActLevelClearAllObjects(const std::string_view id_,
		std::vector<std::string>&& excludeIds_)
		: id(id_), excludeIds(std::move(excludeIds_)) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->clearLevelObjects<LevelObject>(excludeIds);
		}
		return true;
	}
};

class ActLevelClearItems : public Action
{
private:
	std::string id;
	std::vector<std::string> excludeIds;

public:
	ActLevelClearItems(const std::string_view id_,
		std::vector<std::string>&& excludeIds_)
		: id(id_), excludeIds(std::move(excludeIds_)) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->clearLevelObjects<Item>(excludeIds);
		}
		return true;
	}
};

class ActLevelClearLevelObjects : public Action
{
private:
	std::string id;
	std::vector<std::string> excludeIds;

public:
	ActLevelClearLevelObjects(const std::string_view id_,
		std::vector<std::string>&& excludeIds_)
		: id(id_), excludeIds(std::move(excludeIds_)) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->clearLevelObjects<LevelObject>(excludeIds);
		}
		return true;
	}
};

class ActLevelClearPlayerClasses : public Action
{
private:
	std::string id;

public:
	ActLevelClearPlayerClasses(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->clearPlayerClasses();
		}
		return true;
	}
};

class ActLevelClearPlayers : public Action
{
private:
	std::string id;
	std::vector<std::string> excludeIds;

public:
	ActLevelClearPlayers(const std::string_view id_,
		std::vector<std::string>&& excludeIds_)
		: id(id_), excludeIds(std::move(excludeIds_)) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->clearLevelObjects<Player>(excludeIds);
		}
		return true;
	}
};

class ActLevelClearPlayerTextures : public Action
{
private:
	std::string id;

public:
	ActLevelClearPlayerTextures(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) noexcept override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->clearPlayerTextures();
		}
		return true;
	}
};

class ActLevelClearQuests : public Action
{
private:
	std::string id;

public:
	ActLevelClearQuests(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->clearQuests();
		}
		return true;
	}
};

class ActLevelEnableHover : public Action
{
private:
	std::string id;
	bool enableHover;

public:
	ActLevelEnableHover(const std::string_view id_, bool enableHover_)
		: id(id_), enableHover(enableHover_) {}

	bool execute(Game& game) noexcept override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->EnableHover(enableHover);
		}
		return true;
	}
};

class ActLevelMove : public Action
{
private:
	std::string id;
	PairFloat pos;
	bool smooth;

public:
	ActLevelMove(const std::string_view id_, const PairFloat& pos_, bool smooth_)
		: id(id_), pos(pos_), smooth(smooth_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->move(pos, smooth);
		}
		return true;
	}
};

class ActLevelMoveToClick : public Action
{
private:
	std::string id;
	bool smooth;

public:
	ActLevelMoveToClick(const std::string_view id_, bool smooth_)
		: id(id_), smooth(smooth_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->move(smooth);
		}
		return true;
	}
};

class ActLevelMoveToPlayer : public Action
{
private:
	std::string id;
	std::string idPlayer;
	bool smooth;

public:
	ActLevelMoveToPlayer(const std::string_view id_, const std::string_view idPlayer_,
		bool smooth_) : id(id_), idPlayer(idPlayer_), smooth(smooth_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			auto player = level->getLevelObject<Player>(idPlayer);
			if (player != nullptr)
			{
				level->move(player->MapPosition(), smooth);
			}
		}
		return true;
	}
};

class ActLevelPause : public Action
{
private:
	std::string id;
	bool pause;

public:
	ActLevelPause(const std::string_view id_, bool pause_) : id(id_), pause(pause_) {}

	bool execute(Game& game) noexcept override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->Pause(pause);
		}
		return true;
	}
};

class ActLevelSave : public Action
{
private:
	std::string id;
	std::string file;
	Save::Properties props;

public:
	ActLevelSave(const std::string_view id_, const std::string_view file_,
		Save::Properties&& props_) : id(id_), file(file_), props(std::move(props_)) {}

	bool execute(Game& game) noexcept override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->save(GameUtils::replaceStringWithVarOrProp(file, game), props, game);
		}
		return true;
	}
};

class ActLevelSetAutomap : public Action
{
private:
	std::string id;
	std::string idTexturePack;
	std::pair<uint32_t, uint32_t> tileSize;
	uint16_t layerIdx;
	sf::FloatRect viewportOffset;

public:
	ActLevelSetAutomap(const std::string_view id_, const std::string_view idTexturePack_,
		const std::pair<uint32_t, uint32_t>& tileSize_, uint16_t layerIdx_,
		const sf::FloatRect& viewportOffset_) : id(id_), idTexturePack(idTexturePack_),
		tileSize(tileSize_), layerIdx(layerIdx_), viewportOffset(viewportOffset_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			auto automap = game.Resources().getTexturePack(idTexturePack);
			if (automap != nullptr)
			{
				auto idx = std::min(layerIdx, (uint16_t)LevelCell::FlagsLayer);
				level->setAutomap(
					TilesetLevelLayer(automap, idx, {}),
					tileSize.first,
					tileSize.second,
					viewportOffset
				);
			}
		}
		return true;
	}
};

class ActLevelSetAutomapPosition : public Action
{
private:
	std::string id;
	sf::Vector2f position;

public:
	ActLevelSetAutomapPosition(const std::string_view id_, const sf::Vector2f& position_)
		: id(id_), position(position_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			auto newPos = position;
			if (level->getAutomapRelativeCoords() == false &&
				game.RefSize() != game.DrawRegionSize())
			{
				auto anchor = level->getAutomapAnchor();
				auto size = level->getAutomapSize();
				GameUtils::setAnchorPosSize(anchor, newPos, size, game.RefSize(), game.DrawRegionSize());
			}
			level->setAutomapPosition(position);
		}
		return true;
	}
};

class ActLevelSetAutomapSize : public Action
{
private:
	std::string id;
	sf::Vector2f size;

public:
	ActLevelSetAutomapSize(const std::string_view id_, const sf::Vector2f& size_)
		: id(id_), size(size_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->setAutomapSize(size);
		}
		return true;
	}
};

class ActLevelSetShader : public Action
{
private:
	std::string id;
	std::string idShader;

public:
	ActLevelSetShader(const std::string_view id_, const std::string_view idShader_)
		: id(id_), idShader(idShader_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			auto shader = game.Resources().Shaders().get(idShader);
			level->setShader(shader);
		}
		return true;
	}
};

class ActLevelSetSmoothMovement : public Action
{
private:
	std::string id;
	bool smooth;

public:
	ActLevelSetSmoothMovement(const std::string_view id_, bool smooth_)
		: id(id_), smooth(smooth_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->setSmoothMovement(smooth);
		}
		return true;
	}
};

class ActLevelShowAutomap : public Action
{
private:
	std::string id;
	bool show;

public:
	ActLevelShowAutomap(const std::string_view id_, bool show_)
		: id(id_), show(show_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->ShowAutomap(show);
		}
		return true;
	}
};

class ActLevelUpdateDrawables : public Action
{
private:
	std::string id;

public:
	ActLevelUpdateDrawables(const std::string_view id_): id(id_) {}

	bool execute(Game& game) override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->updateDrawables(game);
		}
		return true;
	}
};

class ActLevelZoom : public Action
{
private:
	std::string id;
	int percentage;
	bool relative;
	bool smooth;

public:
	ActLevelZoom(const std::string_view id_, int percentage_, bool relative_, bool smooth_)
		: id(id_), percentage(percentage_), relative(relative_), smooth(smooth_) {}

	bool execute(Game& game) noexcept override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			float perc = percentage / 100.f;
			if (relative == true)
			{
				perc += level->Zoom();
			}
			level->Zoom(perc, smooth);
		}
		return true;
	}
};

class ActLevelZoomDrawables : public Action
{
private:
	std::string id;
	bool zoomDrawables;

public:
	ActLevelZoomDrawables(const std::string_view id_, bool zoomDrawables_)
		: id(id_), zoomDrawables(zoomDrawables_) {}

	bool execute(Game& game) noexcept override
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->ZoomDrawables(zoomDrawables);
		}
		return true;
	}
};
