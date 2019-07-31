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

void BindableText::update(Game& game)
{
	if (bindings.size() > 0 &&
		text->Visible() == true)
	{
		triggerOnChange = text->setText(TextUtils::getFormatString(game, format, bindings));
	}
	Text::update(game);
}
