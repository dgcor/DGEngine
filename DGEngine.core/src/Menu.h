#pragma once

#include "StringButton.h"
#include <vector>

class Menu : public UIObject
{
private:
	std::vector<std::shared_ptr<StringButton>> items;
	int verticalPad{ 0 };
	size_t visibleItems{ 0 };
	size_t currentIdx{ 0 };
	sf::Vector2f pos;
	VerticalAlign vertAlign{ VerticalAlign::Bottom };
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool focusEnable{ false };
	bool recalculatePos{ false };
	bool recalculateVisiblePos{ true };
	size_t start{ 0 };
	size_t end{ 0 };
	sf::Vector2f scrollPosition;
	sf::IntRect scrollRect;
	std::shared_ptr<Action> scrollDownAction;
	std::shared_ptr<Action> scrollUpAction;
	bool visible{ true };

public:
	void updateVisibleItems() noexcept;
	void calculatePositions();

	bool isFocusEnabled() const noexcept { return focusEnable; }
	void focusEnabled() noexcept { focusEnable = true; }

	void addItem(const std::shared_ptr<StringButton>& item);
	StringButton* getItem() const;
	StringButton* getItem(size_t idx) const;
	StringButton* getVisibleItem(size_t idx) const;
	size_t getItemCount() const noexcept { return items.size(); }
	size_t getItemPosition(const Button* item) const;
	void ScrollPosition(const sf::Vector2f& position_) noexcept;
	void setVerticalAlign(const VerticalAlign align) noexcept;
	void setVerticalPad(int verticalPad_) noexcept;
	void setVisibleItems(size_t visibleItems_) noexcept;
	void setCurrentIdx(size_t idx) noexcept;

	const sf::Vector2f& DrawPosition() const noexcept override { return pos; }
	const sf::Vector2f& Position() const noexcept override { return pos; }
	void Position(const sf::Vector2f& position_) noexcept override;
	sf::Vector2f Size() const override;
	void Size(const sf::Vector2f& size) noexcept override;

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	Anchor getAnchor() const override { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept override { anchor = anchor_; }
	void updateSize(const Game& game) override;

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	void draw(const Game& game, sf::RenderTarget& target) const override;

	void update(Game& game) override;

	bool getProperty(const std::string_view prop, Variable& var) const override;
};
