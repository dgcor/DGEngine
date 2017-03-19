#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/Level.h"

class ActLevelClearObjects : public Action
{
private:
	std::string id;

public:
	ActLevelClearObjects(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->clearLevelObjects();
		}
		return true;
	}
};

class ActLevelClearPlayerClasses : public Action
{
private:
	std::string id;
	size_t clearIdx;

public:
	ActLevelClearPlayerClasses(const std::string& id_, size_t clearIdx_)
		: id(id_), clearIdx(clearIdx_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->clearPlayerClasses(clearIdx);
		}
		return true;
	}
};

class ActLevelClearPlayers : public Action
{
private:
	std::string id;
	size_t clearIdx;

public:
	ActLevelClearPlayers(const std::string& id_, size_t clearIdx_)
		: id(id_), clearIdx(clearIdx_) {}

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->clearPlayers(clearIdx);
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
			level->move(game);
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
			auto player = level->getPlayer(idPlayer);
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

	virtual bool execute(Game& game)
	{
		auto level = game.Resources().getLevel(id);
		if (level != nullptr)
		{
			level->Pause(pause);
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

	virtual bool execute(Game& game)
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
