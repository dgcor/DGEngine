#pragma once

#include "Text.h"
#include <vector>

class BindableText : public Text
{
private:
	std::string format;
	std::vector<std::string> bindings;

public:
	using Text::Text;

	void setBinding(const std::string& binding);
	void setBinding(std::vector<std::string> bindings_);
	void setFormat(const std::string_view format_) { format = format_; }

	virtual void update(Game& game);
};
