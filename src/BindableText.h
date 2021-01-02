#pragma once

#include "Text.h"
#include <vector>

class BindableText : public Text
{
private:
	std::string format;
	std::vector<std::string> bindings;
	bool alwaysBind{ false };
	bool bindWhenHidden{ false };

public:
	using Text::Text;

	void setBinding(const std::string& binding);
	void setBinding(std::vector<std::string> bindings_);
	void setFormat(const std::string_view format_) { format = format_; }
	void setAlwaysBind(bool alwaysBind_) { alwaysBind = alwaysBind_; }
	void setBindWhenHidden(bool bindWhenHidden_) { bindWhenHidden = bindWhenHidden_; }

	void update(Game& game) override;
};
