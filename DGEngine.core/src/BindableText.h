#pragma once

#include "BindingFlags.h"
#include "Text.h"
#include <vector>

class BindableText : public Text
{
private:
	std::string format;
	std::vector<std::string> bindings;
	BindingFlags flags{ BindingFlags::OnChange };

public:
	using Text::Text;

	void setBinding(const std::string& binding);
	void setBinding(std::vector<std::string> bindings_);
	void setFormat(const std::string_view format_) { format = format_; }
	void setBindingFlags(BindingFlags flags_) { flags = flags_; }

	void updateText(Game& game);

	void update(Game& game) override;
};
