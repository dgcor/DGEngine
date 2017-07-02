#pragma once

#include <string>

class UIText
{
public:
	virtual std::string getText() const = 0;
	virtual void setText(const std::string& text) = 0;

	virtual void setHorizontalSpaceOffset(int offset) = 0;
	virtual void setVerticalSpaceOffset(int offset) = 0;
};
