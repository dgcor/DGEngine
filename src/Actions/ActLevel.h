#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/Level.h"

class ActLevelClearAllObjects : public Action
{
private:
	std::string id;
	std::vector<std::string> excludeIds;

public:
	ActLevelClearAllObjects(const std::string& id_,
		const std::vector<std::string>& excludeIds_)
		: id(id_), excludeIds(excludeIds_) {}

	virtual bool execute(Game& game)
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
	ActLevelClearItems(const std::string& id_,
		const std::vector<std::string>& excludeIds_)
		: id(id_), excludeIds(excludeIds_) {}

	virtual bool execute(Game& game)
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
	ActLevelClearLevelObjects(const std::string& id_,
		const std::vector<std::string>& excludeIds_)
		: id(id_), excludeIds(excludeIds_) {}

	virtual bool execute(Game& game)
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
	ActLevelClearPlayerClasses(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
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
	ActLevelClearPlayers(const std::string& id_,
		const std::vector<std::string>& excludeIds_)
		: id(id_), excludeIds(excludeIds_) {}

	virtual bool execute(Game& game)
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
	ActLevelClearPlayerTextures(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game) noexcept
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
	ActLevelClearQuests(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
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
	ActLevelEnableHover(const std::string& id_, bool enableHover_)
		: id(id_), enableHover(enableHover_) {}

	virtual bool execute(Game& game) noexcept
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
	MapCoord pos;

public:
	ActLevelMove(const std::string& id_, const MapCoord& pos_) : id(id_), pos(pos_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->move(pos);
		}
		return true;
	}
};

class ActLevelMoveToClick : public Action
{
private:
	std::string id;

public:
	ActLevelMoveToClick(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->move();
		}
		return true;
	}
};

class ActLevelMoveToPlayer : public Action
{
private:
	std::string id;
	std::string idPlayer;

public:
	ActLevelMoveToPlayer(const std::string& id_, const std::string& idPlayer_)
		: id(id_), idPlayer(idPlayer_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			auto player = level->getLevelObject<Player>(idPlayer);
			if (player != nullptr)
			{
				level->move(player->MapPosition());
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
	ActLevelPause(const std::string& id_, bool pause_) : id(id_), pause(pause_) {}

	virtual bool execute(Game& game) noexcept
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
	bool saveDefaults;
	bool saveCurrentPlayer;
	bool saveQuests;

public:
	ActLevelSave(const std::string& id_, const std::string& file_,
		bool saveDefaults_, bool saveCurrentPlayer_, bool saveQuests_)
		: id(id_), file(file_), saveDefaults(saveDefaults_),
		saveCurrentPlayer(saveCurrentPlayer_), saveQuests(saveQuests_) {}

	virtual bool execute(Game& game) noexcept
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			Save::Properties props;
			props.saveDefaults = saveDefaults;
			props.saveCurrentPlayer = saveCurrentPlayer;
			props.saveQuests = saveQuests;

			level->save(GameUtils::replaceStringWithVarOrProp(file, game).c_str(),
				props, game);
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

public:
	ActLevelSetAutomap(const std::string& id_, const std::string& idTexturePack_,
		const std::pair<uint32_t, uint32_t>& tileSize_) : id(id_),
		idTexturePack(idTexturePack_), tileSize(tileSize_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			auto automap = game.Resources().getTexturePack(idTexturePack);
			if (automap != nullptr)
			{
				level->setAutomap(automap, tileSize.first, tileSize.second);
			}
		}
		return true;
	}
};

class ActLevelSetAutomapBackground : public Action
{
private:
	std::string id;
	sf::Color background;

public:
	ActLevelSetAutomapBackground(const std::string& id_, const sf::Color color_)
		: id(id_), background(color_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->setAutomapBackgroundColor(background);
		}
		return true;
	}
};

class ActLevelSetAutomapPosition : public Action
{
private:
	std::string id;
	sf::Vector2i position;

public:
	ActLevelSetAutomapPosition(const std::string& id_, const sf::Vector2i& position_)
		: id(id_), position(position_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->setAutomapRelativePosition(position);
		}
		return true;
	}
};

class ActLevelSetAutomapSize : public Action
{
private:
	std::string id;
	sf::Vector2i size;

public:
	ActLevelSetAutomapSize(const std::string& id_, const sf::Vector2i& size_)
		: id(id_), size(size_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->setAutomapRelativeSize(size);
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
	ActLevelShowAutomap(const std::string& id_, bool show_)
		: id(id_), show(show_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->ShowAutomap(show);
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
	ActLevelZoom(const std::string& id_, int percentage_, bool relative_, bool smooth_)
		: id(id_), percentage(percentage_), relative(relative_), smooth(smooth_) {}

	virtual bool execute(Game& game) noexcept
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
