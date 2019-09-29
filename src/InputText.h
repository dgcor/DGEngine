#pragma once

#include <optional>
#include <regex>
#include "Text.h"

class InputText : public Text
{
private:
	std::shared_ptr<Action> enterAction;
	std::shared_ptr<Action> minLengthAction;
	size_t minLength{ 0 };
	size_t maxLength{ 0 };
	std::optional<Variable> minValue;
	std::optional<Variable> maxValue;
	std::optional<std::regex> regex;

	bool isValidMin(const std::string& str) const noexcept;
	bool isValidMax(const std::string& str) const noexcept;

public:
	using Text::Text;

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	void setMinLength(size_t length) noexcept { minLength = length; }
	void setMaxLength(size_t length) noexcept { maxLength = length; }
	void setMinValue(const Variable& var) { minValue = var; }
	void setMaxValue(const Variable& var) { maxValue = var; }
	void setRegex(const std::string& regex_);

	void click(Game& game);

	void update(Game& game) override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
