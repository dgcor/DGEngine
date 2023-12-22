#include "Text.h"
#include "BitmapText.h"
#include <cassert>
#include "StringText.h"
#include "Utils/StringHash.h"

Text::Text(const Font& font, unsigned int characterSize)
{
	assert(holdsNullFont(font) == false);

	if (holdsNullFont(font) == true)
	{
		return;
	}
	if (holdsFreeTypeFont(font) == true)
	{
		text = std::make_unique<StringText>(std::get<std::shared_ptr<FreeTypeFont>>(font), characterSize);
	}
	else
	{
		text = std::make_unique<BitmapText>(std::get<std::shared_ptr<BitmapFont>>(font));
	}
}

void Text::setFont(const Font& font)
{
	if (holdsNullFont(font) == true)
	{
		return;
	}

	auto text = getDrawableText();
	if (text == nullptr)
	{
		return;
	}

	auto bitmapText = dynamic_cast<BitmapText*>(text);
	if (bitmapText != nullptr &&
		holdsBitmapFont(font) == true)
	{
		bitmapText->setFont(std::get<std::shared_ptr<BitmapFont>>(font));
		return;
	}

	auto stringText = dynamic_cast<StringText*>(text);
	if (stringText != nullptr &&
		holdsFreeTypeFont(font) == true)
	{
		stringText->setFont(std::get<std::shared_ptr<FreeTypeFont>>(font));
	}
}

std::shared_ptr<Action> Text::getAction(uint16_t nameHash16) const noexcept
{
	switch (nameHash16)
	{
	case str2int16("change"):
		return changeAction;
	default:
		return nullptr;
	}
}

bool Text::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("change"):
		changeAction = action;
		break;
	default:
		return false;
	}
	return true;
}

void Text::update(Game& game)
{
	if (triggerOnChange == true)
	{
		triggerOnChange = false;
		if (changeAction != nullptr)
		{
			changeAction->execute(game);
		}
	}
}
