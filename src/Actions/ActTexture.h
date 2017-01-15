#pragma once

#include "Action.h"
#include "Game.h"

template <class T>
class ActSetTexture : public Action
{
private:
	std::string id;
	std::string idTexture;
	bool resetRect;

public:
	ActSetTexture(const std::string& id_,
		const std::string& idTexture_, bool resetRect_)
		: id(id_), idTexture(idTexture_), resetRect(resetRect_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<T>(id);
		if (item != nullptr)
		{
			auto tex = game.Resources().getTexture(idTexture);
			if (tex != nullptr)
			{
				item->setTexture(*tex, resetRect);
			}
		}
		return true;
	}
};

template <class T>
class ActSetTextureFromInventory : public Action
{
private:
	std::string id;
	std::string idLevel;
	std::string idPlayer;
	size_t invIdx;
	sf::Vector2u itemPos;
	bool isCoordXY;
	bool resetRect;

public:
	ActSetTextureFromInventory(const std::string& id_, const std::string& idLevel_,
		const std::string& idPlayer_, size_t invIdx_, size_t itemIdx_,
		bool resetRect_) : id(id_), idLevel(idLevel_), idPlayer(idPlayer_),
		invIdx(invIdx_), itemPos(itemIdx_, 0), isCoordXY(false), resetRect(resetRect_) {}

	ActSetTextureFromInventory(const std::string& id_, const std::string& idLevel_,
		const std::string& idPlayer_, size_t invIdx_, const sf::Vector2u& itemPos_,
		bool resetRect_) : id(id_), idLevel(idLevel_), idPlayer(idPlayer_),
		invIdx(invIdx_), itemPos(itemPos_), isCoordXY(true), resetRect(resetRect_) {}

	virtual bool execute(Game& game)
	{
		auto resource = game.Resources().getResource<T>(id);
		if (resource != nullptr)
		{
			auto level = game.Resources().getLevel(idLevel);
			if (level != nullptr)
			{
				auto player = level->getPlayerOrCurrent(idPlayer);
				if (player != nullptr)
				{
					const auto& inventory = player->getInventory(invIdx);
					size_t itemIdx = itemPos.x;
					if (isCoordXY == true)
					{
						itemIdx = inventory.getIndex(itemPos);
					}
					auto item = inventory.get(itemIdx).get();
					if (item != nullptr)
					{
						resource->setTexture(item->Class()->getCelInventoryTexture(), resetRect);
					}
				}
			}
		}
		return true;
	}
};

template <class T>
class ActSetTextureRect : public Action
{
private:
	std::string id;
	sf::IntRect rect;

public:
	ActSetTextureRect(const std::string& id_,
		const sf::IntRect& rect_) : id(id_), rect(rect_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<T>(id);
		if (item != nullptr)
		{
			item->setTextureRect(rect);
		}
		return true;
	}
};

template <class T>
class ActSetTextureRectVec : public Action
{
private:
	std::string id;
	std::vector<sf::IntRect> rectVec;
	size_t idx{ 0 };

public:
	ActSetTextureRectVec(const std::string& id_,
		const std::vector<sf::IntRect> & rectVec_)
		: id(id_), rectVec(rectVec_) {}

	virtual bool execute(Game& game)
	{
		if (rectVec.empty() == true)
		{
			return true;
		}

		auto item = game.Resources().getResource<T>(id);
		if (item != nullptr)
		{
			item->setTextureRect(rectVec[idx]);
			if (idx + 1 < rectVec.size())
			{
				idx++;
			}
			else
			{
				idx = 0;
			}
			return false;
		}
		return true;
	}
};
