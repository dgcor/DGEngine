#pragma once

#include "Game/Action.h"
#include "Game/Drawables/Panel.h"
#include "Game/Game.h"

class ActPanelAddDrawable : public Action
{
private:
	std::string id;
	std::string idDrawable;

public:
	ActPanelAddDrawable(const std::string_view id_, const std::string_view idDrawable_)
		: id(id_), idDrawable(idDrawable_) {}

	bool execute(Game& game) override
	{
		auto panel = game.Resources().getDrawable<Panel>(id);
		if (panel != nullptr)
		{
			auto drawable = game.Resources().getDrawableSharedPtr<UIObject>(idDrawable);
			if (drawable != nullptr &&
				panel->addDrawable(drawable) == true)
			{
				game.Resources().deleteDrawable(drawable.get());
			}
		}
		return true;
	}
};
