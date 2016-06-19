#include "ParseMenuButton.h"
#include "BitmapText.h"
#include "StringButton.h"
#include "StringText.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<StringButton> parseMenuButton(
		Anchor anchor,
		const sf::Color& color,
		const HorizontalAlign horizAlign,
		const bool isTextFont,
		const std::shared_ptr<Font2>& font,
		const unsigned fontSize,
		const std::shared_ptr<BitmapFont>& bitmapFont,
		const std::shared_ptr<sf::SoundBuffer>& sound,
		const std::shared_ptr<sf::SoundBuffer>& focusSound,
		bool clickUp)
	{
		auto button = std::make_shared<StringButton>();
		std::unique_ptr<DrawableText> drawableText;
		if (isTextFont)
		{
			drawableText = std::make_unique<StringText>("", *font, fontSize);
		}
		else
		{
			drawableText = std::make_unique<BitmapText>("", bitmapFont);
		}
		button->setText(std::move(std::make_unique<Text2>(std::move(drawableText))));

		button->setAnchor(anchor);
		button->setColor(color);
		button->setHorizontalAlign(horizAlign);
		button->setClickUp(clickUp);

		if (sound != nullptr)
		{
			button->setClickSound(sound);
		}

		if (focusSound != nullptr)
		{
			button->setFocusSound(focusSound);
		}
		return button;
	};
}
