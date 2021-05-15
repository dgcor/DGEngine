#include "BindableText.h"
#include "TextUtils.h"

void BindableText::setBinding(const std::string& binding)
{
	bindings.clear();
	bindings.push_back(binding);
}

void BindableText::setBinding(std::vector<std::string> bindings_)
{
	bindings = std::move(bindings_);
}

void BindableText::updateText(Game& game)
{
	std::string str;
	TextUtils::getFormatString(game, format, bindings, str);
	setText(str);
}

void BindableText::update(Game& game)
{
	if (bindings.size() > 0 &&
		(int)(flags & BindingFlags::Once) == 0 &&
		((int)(flags & BindingFlags::WhenHidden) != 0 || Visible() == true))
	{
		std::string str;
		bool hasText = TextUtils::getFormatString(game, format, bindings, str);
		if ((int)(flags & BindingFlags::Always) != 0 || hasText == true)
		{
			setText(str);
		}
	}
	Text::update(game);
}
