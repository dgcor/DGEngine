#include "Game.h"
#include <cmath>
#include "GameQueryable.h"
#include "Game/Utils/FileUtils.h"
#include "Hooks.h"
#include <mutex>
#include "Parser/Parser.h"
#include <ranges>
#include <SFML/Audio/SoundFileFactory.hpp>
#include "SFML/SFMLUtils.h"
#include "SFML/Wave2.h"
#include "Utils/StringHash.h"

uint32_t Game::DefaultSizeX{ 640 };
uint32_t Game::DefaultSizeY{ 480 };

uint32_t Game::MinSizeX{ 640 };
uint32_t Game::MinSizeY{ 480 };

uint32_t Game::RefSizeX{ 640 };
uint32_t Game::RefSizeY{ 480 };

Game::Game(bool reset_)
{
	static std::once_flag initWave2;
	std::call_once(initWave2, []() { sf::SoundFileFactory::registerReader<Wave2FileReader>(); });

	if (reset_ == true)
	{
		reset();
	}
}

Game::~Game()
{
	resourceManager = {};
	if (window.isOpen() == true)
	{
		window.close();
	}
}

void Game::reset()
{
	gameSprite = {};

	position.reset();
	refSize = { RefSizeX, RefSizeY };
	minSize = { MinSizeX, MinSizeY };
	size = { DefaultSizeX, DefaultSizeY };
	windowedSize = size;
	drawRegionSize = size;
	oldDrawRegionSize = {};
	maxHeight = { 0 };
	frameRate = { 60 };
	fullScreen = { false };
	smoothScreen = { false };
	stretchToFit = { false };
	keepAR = { true };
	enableInput = { true };
	paused = { false };
	mouseCursorVisible = { true };

	mousePositioni = {};
	mousePositionf = {};

	mousePressEvt = {};
	mouseReleaseEvt = {};
	mouseMoveEvt = {};
	mouseScrollEvt = {};
	keyPressEvt = {};
	textEnteredEvt = {};
	touchBeganEvt = {};
	touchMovedEvt = {};
	touchEndedEvt = {};
	mousePressed = { false };
	mouseReleased = { false };
	mouseMoved = { false };
	mouseScrolled = { false };
	keyPressed = { false };
	textEntered = { false };
	touchBegan = { false };
	touchMoved = { false };
	touchEnded = { false };

	musicVolume = { 100 };
	soundVolume = { 100 };
	gamma = { 0 };

	elapsedTime = {};
	totalElapsedTime = {};

	path = {};
	title = {};
	version = {};

	resourceManager = {};
	if (Hooks::InitializeShaderManager != nullptr)
	{
		Hooks::InitializeShaderManager(resourceManager.Shaders());
	}
	else
	{
		ShaderManager::init(resourceManager.Shaders());
	}
	resourceManager.Shaders().init(shaders);

	variableManager = {};
	gameInputEventManager = {};

	loadingScreen = {};
	fadeObj = {};

	closeAction.reset();
	resizeAction.reset();
	focusLossAction.reset();
	focusGainAction.reset();
}

void Game::load(const std::string_view gamefilePath, const std::string_view mainFile)
{
	if (window.isOpen() == true)
	{
		window.close();
	}
	reset();
	FileUtils::unmountAll();
	Parser::parseGame(*this, gamefilePath, mainFile);
}

void Game::init()
{
	if (window.isOpen() == true)
	{
		return;
	}

	recreateWindow();

	// forces update of drawing window
	updateWindowSize();

	if (gameSprite.getTexture() == nullptr)
	{
		recreateRenderTexture(gameTexture);
		gameSprite.setTexture(gameTexture.getTexture(), true);
	}
}

std::shared_ptr<Action> Game::getAction(uint16_t nameHash16) const noexcept
{
	return GameQueryable::getAction(*this, nameHash16);
}

bool Game::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	return GameQueryable::setAction(*this, nameHash16, action);
}

void Game::Position(const sf::Vector2i& position_)
{
	position = position_;
	if (fullScreen == false)
	{
		window.setPosition(position_);
	}
}

void Game::Size(sf::Vector2u size_)
{
	size_.x = std::max(minSize.x, size_.x);
	size_.y = std::max(minSize.y, size_.y);
	if (window.isOpen() == true)
	{
		if (fullScreen == true)
		{
			windowedSize = size_;
		}
		else
		{
			window.setSize(size_);
		}
	}
	else
	{
		size = size_;
		windowedSize = size_;
	}
}

void Game::RefSize(const sf::Vector2u& size_)
{
	if (size_.x >= RefSizeX && size_.y >= RefSizeY)
	{
		refSize = size_;
	}
}

void Game::MinSize(const sf::Vector2u& size_)
{
	bool needsUpdate = false;
	if (size_.x >= MinSizeX && size_.y >= MinSizeY)
	{
		minSize = size_;
		if (window.isOpen() == true)
		{
			needsUpdate = true;
		}
	}
	if (size.x < minSize.x && size.y < minSize.y)
	{
		if (window.isOpen() == true)
		{
			window.setSize(minSize);
			return;
		}
		else
		{
			size = minSize;
			windowedSize = minSize;
		}
	}
	if (needsUpdate == true)
	{
		updateWindowSize();
	}
}

void Game::FrameRate(int frameRate_)
{
	if (frameRate_ > 0)
	{
		frameRate = (unsigned)std::clamp(frameRate_, 30, 240);
	}
	else
	{
		frameRate = 0;
	}
	if (window.isOpen() == true)
	{
		window.setFramerateLimit(frameRate);
	}
}

void Game::FullScreen(bool fullScreen_)
{
	if (fullScreen == fullScreen_)
	{
		return;
	}
	if (fullScreen_ == false)
	{
		size = windowedSize;
	}
	fullScreen = fullScreen_;

	if (window.isOpen() == true)
	{
		recreateWindow();
		updateWindowSize();
	}
}

void Game::setMouseCursorVisible(bool show)
{
	mouseCursorVisible = show;
	window.setMouseCursorVisible(show);
}

void Game::Title(const std::string& title_)
{
	title = title_;
	if (window.isOpen() == true)
	{
		window.setTitle(title);
	}
}

void Game::play()
{
	if (window.isOpen() == false)
	{
		return;
	}

	updateMousePosition();
	sf::Clock frameClock;

	while (window.isOpen() == true)
	{
		if (fullScreen == false)
		{
			position.emplace(window.getPosition());
		}

		processEvents();

		elapsedTime = frameClock.restart();
		totalElapsedTime += elapsedTime;

		updateEvents();

		resourceManager.clearFinishedSounds();

		if (drawLoadingScreen() == false)
		{
			update();
			draw();
		}
	}
}

void Game::processEvents()
{
	mousePressed = false;
	mouseReleased = false;
	mouseMoved = false;
	mouseScrolled = false;
	keyPressed = false;
	textEntered = false;

	sf::Event evt;
	while (window.pollEvent(evt))
	{
		switch (evt.type)
		{
		case sf::Event::Closed:
			onClosed();
			break;
		case sf::Event::Resized:
			onResized(evt.size);
			break;
		case sf::Event::LostFocus:
			onLostFocus();
			break;
		case sf::Event::GainedFocus:
			onGainedFocus();
			break;
		case sf::Event::TextEntered:
			onTextEntered(evt.text);
			break;
		case sf::Event::KeyPressed:
			onKeyPressed(evt);
			break;
		case sf::Event::KeyReleased:
			onKeyReleased(evt);
			break;
		case sf::Event::MouseWheelScrolled:
			onMouseWheelScrolled(evt.mouseWheelScroll);
			break;
		case sf::Event::MouseButtonPressed:
			onMouseButtonPressed(evt.mouseButton);
			break;
		case sf::Event::MouseButtonReleased:
			onMouseButtonReleased(evt.mouseButton);
			break;
		case sf::Event::MouseMoved:
			onMouseMoved(evt.mouseMove);
			break;
		case sf::Event::TouchBegan:
			onTouchBegan(evt.touch);
			break;
		case sf::Event::TouchMoved:
			onTouchMoved(evt.touch);
			break;
		case sf::Event::TouchEnded:
			onTouchEnded(evt.touch);
			break;
		default:
			break;
		}
	}
	resourceManager.updateActiveInputEvents();
}

void Game::onClosed()
{
	if (closeAction != nullptr)
	{
		eventManager.addBack(closeAction);
	}
	else
	{
		window.close();
	}
}

void Game::onResized(const sf::Event::SizeEvent& evt)
{
	bool needsResize = false;
	sf::Vector2u newSize(evt.width, evt.height);
	if (newSize.x < minSize.x)
	{
		newSize.x = minSize.x;
		needsResize = true;
	}
	if (newSize.y < minSize.y)
	{
		newSize.y = minSize.y;
		needsResize = true;
	}
	if (needsResize == true)
	{
		window.setSize(newSize);
		return;
	}
	size = newSize;
	if (fullScreen == false)
	{
		windowedSize = newSize;
	}
	updateWindowSize();
	eventManager.tryAddBack(resizeAction);
}

void Game::updateWindowSize()
{
	// update draw region sizes
	oldDrawRegionSize = drawRegionSize;
	drawRegionSize = (stretchToFit == false ? size : minSize);

	bool usingMaxHeight = false;
	if (stretchToFit == false &&
		maxHeight >= minSize.y &&
		drawRegionSize.y > maxHeight)
	{
		usingMaxHeight = true;

		auto factor = (float)(maxHeight) / (float)(drawRegionSize.y);
		drawRegionSize.x = (uint32_t)((float)(drawRegionSize.x) * factor);
		drawRegionSize.y = maxHeight;

		if (drawRegionSize.x < minSize.x)
		{
			drawRegionSize.x = minSize.x;
		}
	}

	// force even size
	auto evenRegionSize = drawRegionSize;
	evenRegionSize.x = ((uint32_t)((int)((float)evenRegionSize.x * 0.5f)) * 2);
	evenRegionSize.y = ((uint32_t)((int)((float)evenRegionSize.y * 0.5f)) * 2);

	// update main view
	auto view = window.getView();
	view.reset({ 0.f, 0.f, (float)evenRegionSize.x, (float)evenRegionSize.y });
	if (stretchToFit == true && keepAR == true)
	{
		SFMLUtils::viewStretchKeepAR(view, size);
	}
	else if (stretchToFit == false && usingMaxHeight == true)
	{
		SFMLUtils::viewStretchKeepAR(view, size);
	}
	else
	{
		float normalizedWidth = (float)evenRegionSize.x / (float)drawRegionSize.x;
		float normalizedHeight = (float)evenRegionSize.y / (float)drawRegionSize.y;
		view.setViewport({ 0.f, 0.f, normalizedWidth, normalizedHeight });
	}
	window.setView(view);

	drawRegionSize = evenRegionSize;

	if (oldDrawRegionSize == drawRegionSize)
	{
		return;
	}

	// clears artefacts
	window.clear();
	window.display();

	// update game texture
	recreateRenderTexture(gameTexture);
	gameSprite.setTexture(gameTexture.getTexture(), true);

	// update drawables
	if (loadingScreen != nullptr)
	{
		loadingScreen->updateSize(*this);
	}
	for (auto& res : resourceManager)
	{
		for (auto obj : res.drawables)
		{
			obj->updateSize(*this);
		}
	}
}

void Game::recreateRenderTexture(sf::RenderTexture& renderTexture) const
{
	renderTexture.create(drawRegionSize.x, drawRegionSize.y);
	renderTexture.setSmooth(smoothScreen);
}

void Game::recreateWindow()
{
	if (fullScreen == true)
	{
		auto videoMode = sf::VideoMode::getDesktopMode();
		windowedSize = size;
		size.x = videoMode.width;
		size.y = videoMode.height;
		window.create(videoMode, title, sf::Style::Fullscreen);
	}
	else
	{
		window.create(sf::VideoMode(size.x, size.y), title);

		if (position.has_value() == true)
		{
			window.setPosition(*position);
		}
		else
		{
			position = window.getPosition();
		}
	}
	window.setMouseCursorVisible(mouseCursorVisible);
	if (frameRate > 0)
	{
		window.setFramerateLimit(frameRate);
	}
}

void Game::onLostFocus() noexcept
{
	eventManager.tryAddBack(focusLossAction);
}

void Game::onGainedFocus() noexcept
{
	eventManager.tryAddBack(focusGainAction);
}

void Game::onTextEntered(const sf::Event::TextEvent& evt) noexcept
{
	if (enableInput == false)
	{
		return;
	}
	textEnteredEvt = evt;
	textEntered = true;
}

void Game::onKeyPressed(sf::Event evt)
{
	if (enableInput == false)
	{
		return;
	}
	keyPressEvt = evt.key;
	keyPressed = true;
	if (loadingScreen != nullptr)
	{
		return;
	}
	eventManager.tryAddBack(resourceManager.getInputAction(evt));
}

void Game::onKeyReleased(sf::Event evt)
{
	if (enableInput == false ||
		loadingScreen != nullptr)
	{
		return;
	}
	eventManager.tryAddBack(resourceManager.getInputAction(evt));
}

void Game::onMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	mouseScrollEvt = evt;
	mouseScrolled = true;
	updateMousePosition();
	mouseScrollEvt.x = mousePositioni.x;
	mouseScrollEvt.y = mousePositioni.y;
}

void Game::onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	mousePressEvt = evt;
	mousePressed = true;
	updateMousePosition();
	mousePressEvt.x = mousePositioni.x;
	mousePressEvt.y = mousePositioni.y;
}

void Game::onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	mouseReleaseEvt = evt;
	mouseReleased = true;
	updateMousePosition();
	mouseReleaseEvt.x = mousePositioni.x;
	mouseReleaseEvt.y = mousePositioni.y;
}

void Game::onMouseMoved(const sf::Event::MouseMoveEvent& evt)
{
	updateMousePosition(sf::Vector2i(evt.x, evt.y));
	if (enableInput == false)
	{
		return;
	}
	mouseMoveEvt = evt;
	mouseMoved = true;
	mouseMoveEvt.x = mousePositioni.x;
	mouseMoveEvt.y = mousePositioni.y;
}

void Game::onTouchBegan(const sf::Event::TouchEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	touchBeganEvt = evt;
	touchBegan = true;
	updateMousePosition();
	touchBeganEvt.x = mousePositioni.x;
	touchBeganEvt.y = mousePositioni.y;
}

void Game::onTouchMoved(const sf::Event::TouchEvent& evt)
{
	updateMousePosition(sf::Vector2i(evt.x, evt.y));
	if (enableInput == false)
	{
		return;
	}
	touchMovedEvt = evt;
	touchMoved = true;
	touchMovedEvt.x = mousePositioni.x;
	touchMovedEvt.y = mousePositioni.y;
}

void Game::onTouchEnded(const sf::Event::TouchEvent& evt)
{
	if (enableInput == false)
	{
		return;
	}
	touchEndedEvt = evt;
	touchEnded = true;
	updateMousePosition();
	touchEndedEvt.x = mousePositioni.x;
	touchEndedEvt.y = mousePositioni.y;
}

void Game::setMousePosition(sf::Vector2i mousePos)
{
	mousePos = window.mapCoordsToPixel({ (float)mousePos.x , (float)mousePos.y });
	sf::Mouse::setPosition(mousePos, window);
	updateMousePosition();
}

void Game::updateMousePosition()
{
	updateMousePosition(sf::Mouse::getPosition(window));
}

void Game::updateMousePosition(const sf::Vector2i mousePos)
{
	mousePositionf = window.mapPixelToCoords(mousePos);
	mousePositionf.x = std::round(mousePositionf.x);
	mousePositionf.y = std::round(mousePositionf.y);
	mousePositioni.x = (int)mousePositionf.x;
	mousePositioni.y = (int)mousePositionf.y;
	updateCursorPosition();
}

void Game::updateCursorPosition()
{
	auto cursor = resourceManager.getCursor();
	if (cursor != nullptr)
	{
		cursor->Position(mousePositionf);
	}
}

void Game::updateEvents()
{
	if (paused == false)
	{
		eventManager.update(*this);
	}
}

bool Game::drawLoadingScreen()
{
	if (loadingScreen == nullptr)
	{
		return false;
	}
	window.clear();
	gameTexture.clear();

	loadingScreen->draw(*this, gameTexture);
	drawWindow();
	return true;
}

void Game::update()
{
	for (auto& res : resourceManager | std::views::reverse)
	{
		if ((int)(res.ignore & IgnoreResource::Update) == 0)
		{
			for (auto obj : res.drawables | std::views::reverse)
			{
				if (paused == false && res.ignore != IgnoreResource::Update)
				{
					obj->update(*this);
				}
			}
		}
		else if ((int)(res.ignore & IgnoreResource::All) != 0)
		{
			break;
		}
	}
}

void Game::drawUI()
{
	for (auto& res : resourceManager)
	{
		if ((int)(res.ignore & IgnoreResource::Draw) == 0)
		{
			for (auto obj : res.drawables)
			{
				obj->draw(*this, gameTexture);
			}
		}
		else if ((int)(res.ignore & IgnoreResource::All) != 0)
		{
			break;
		}
	}
}

void Game::drawCursor()
{
	auto cursor = resourceManager.getCursor();
	if (cursor != nullptr)
	{
		cursor->update(*this);
		cursor->draw(*this, gameTexture);
	}
}

void Game::drawWindow()
{
	gameTexture.display();
	auto states(sf::RenderStates::Default);
	if (shaders.hasGameShader() == true)
	{
		auto shader = shaders.Game->shader.get();
		states.shader = shader;
		for (auto uniformHash : shaders.Game->uniforms)
		{
			switch (uniformHash)
			{
			case str2int16("elapsedTime"):
			{
				shader->setUniform("elapsedTime", totalElapsedTime.asSeconds());
				break;
			}
			case str2int16("gamma"):
			{
				shader->setUniform("gamma", (float)gamma);
				break;
			}
			case str2int16("fade"):
			{
				shader->setUniform("fade", sf::Glsl::Vec4(fadeObj.getColor()));
				break;
			}
			case str2int16("loading"):
			{
				if (loadingScreen != nullptr)
				{
					shader->setUniform("loading", (float)loadingScreen->getProgress() / 100.f);
				}
				else
				{
					shader->setUniform("loading", 1.f);
				}
				break;
			}
			case str2int16("mousePosition"):
			{
				shader->setUniform("mousePosition", sf::Glsl::Vec2(
					(float)std::clamp(mousePositioni.x, 0, (int)drawRegionSize.x) / (float)drawRegionSize.x,
					(float)std::clamp(mousePositioni.y, 0, (int)drawRegionSize.y) / (float)drawRegionSize.y
				));
				break;
			}
			case str2int16("textureSize"):
			{
				shader->setUniform("textureSize", sf::Glsl::Vec2(
					(float)drawRegionSize.x,
					(float)drawRegionSize.y
				));
				break;
			}
			default:
				break;
			}
		}
	}
	fadeObj.update(*this);
	window.draw(gameSprite, states);
	window.display();
}

void Game::draw()
{
	window.clear();
	gameTexture.clear();

	drawUI();
	drawCursor();
	drawWindow();
}

void Game::clearInputEvents(InputEventType e) noexcept
{
	if (e == InputEventType::None)
	{
		return;
	}
	else if (e == InputEventType::All)
	{
		mousePressed = false;
		mouseReleased = false;
		mouseMoved = false;
		mouseScrolled = false;
		keyPressed = false;
		textEntered = false;
		touchBegan = false;
		touchMoved = false;
		touchEnded = false;
		return;
	}
	if (mousePressed == true &&
		(int)(e & InputEventType::MousePress) != 0)
	{
		if ((int)(e & InputEventType::LeftClick) != 0 &&
			mousePressEvt.button == sf::Mouse::Button::Left)
		{
			mousePressed = false;
		}
		else if ((int)(e & InputEventType::MiddleClick) != 0 &&
			mousePressEvt.button == sf::Mouse::Button::Middle)
		{
			mousePressed = false;
		}
		else if ((int)(e & InputEventType::RightClick) != 0 &&
			mousePressEvt.button == sf::Mouse::Button::Right)
		{
			mousePressed = false;
		}
	}
	if (mouseReleased == true &&
		(int)(e & InputEventType::MouseRelease) != 0)
	{
		if ((int)(e & InputEventType::LeftClick) != 0 &&
			mouseReleaseEvt.button == sf::Mouse::Button::Left)
		{
			mouseReleased = false;
		}
		else if ((int)(e & InputEventType::MiddleClick) != 0 &&
			mouseReleaseEvt.button == sf::Mouse::Button::Middle)
		{
			mouseReleased = false;
		}
		else if ((int)(e & InputEventType::RightClick) != 0 &&
			mouseReleaseEvt.button == sf::Mouse::Button::Right)
		{
			mouseReleased = false;
		}
	}
	if (mouseMoved == true &&
		(int)(e & InputEventType::MouseMove) != 0)
	{
		mouseMoved = false;
	}
	if (mouseScrolled == true &&
		(int)(e & InputEventType::MouseScroll) != 0)
	{
		mouseScrolled = false;
	}
	if (keyPressed == true &&
		(int)(e & InputEventType::KeyPress) != 0)
	{
		keyPressed = false;
	}
	if (textEntered == true &&
		(int)(e & InputEventType::TextEnter) != 0)
	{
		textEntered = false;
	}
	if (touchBegan == true &&
		(int)(e & InputEventType::TouchBegin) != 0)
	{
		touchBegan = false;
	}
	if (touchMoved == true &&
		(int)(e & InputEventType::TouchMove) != 0)
	{
		touchMoved = false;
	}
	if (touchEnded == true &&
		(int)(e & InputEventType::TouchEnd) != 0)
	{
		touchEnded = false;
	}
}

void Game::SmoothScreen(bool smooth_)
{
	smoothScreen = smooth_;
	gameTexture.setSmooth(smooth_);
}

void Game::MaxHeight(uint32_t maxHeight_)
{
	if (maxHeight == maxHeight_)
	{
		return;
	}
	maxHeight = maxHeight_;
	if (window.isOpen() == false)
	{
		return;
	}
	updateWindowSize();
	updateMousePosition();
}

void Game::StretchToFit(bool stretchToFit_)
{
	if (stretchToFit == stretchToFit_)
	{
		return;
	}
	stretchToFit = stretchToFit_;
	if (window.isOpen() == false)
	{
		return;
	}
	updateWindowSize();
	updateMousePosition();
}

void Game::KeepAR(bool keepAR_)
{
	if (keepAR == keepAR_)
	{
		return;
	}
	keepAR = keepAR_;
	if (window.isOpen() == false || stretchToFit == false)
	{
		return;
	}
	updateWindowSize();
	updateMousePosition();
}

void Game::addPlayingSound(const sf::SoundBuffer& obj)
{
	sf::Sound sound(obj);
	sound.setVolume((float)soundVolume);
	resourceManager.addPlayingSound(sound);
}

void Game::addPlayingSound(const sf::SoundBuffer* obj)
{
	if (obj != nullptr)
	{
		addPlayingSound(*obj);
	}
}

bool Game::getVarOrPropNoToken(const std::string_view key, Variable& var) const
{
	return variableManager.getVarOrPropNoToken(*this, key, var);
}

bool Game::getVarOrProp(const std::string_view key, Variable& var) const
{
	return variableManager.getVarOrProp(*this, key, var);
}

Variable Game::getVarOrProp(const Variable& var) const
{
	return variableManager.getVarOrProp(*this, var);
}

bool Game::getVarOrPropBoolS(const std::string_view key) const
{
	return variableManager.getVarOrPropBoolS(*this, key);
}

bool Game::getVarOrPropBoolV(const Variable& var) const
{
	return variableManager.getVarOrPropBoolV(*this, var);
}

double Game::getVarOrPropDoubleS(const std::string_view key) const
{
	return variableManager.getVarOrPropDoubleS(*this, key);
}

double Game::getVarOrPropDoubleV(const Variable& var) const
{
	return variableManager.getVarOrPropDoubleV(*this, var);
}

int64_t Game::getVarOrPropInt64S(const std::string_view key) const
{
	return variableManager.getVarOrPropInt64S(*this, key);
}

int64_t Game::getVarOrPropInt64V(const Variable& var) const
{
	return variableManager.getVarOrPropInt64V(*this, var);
}

bool Game::getVarOrPropStringS(const std::string_view key, std::string& outStr) const
{
	return variableManager.getVarOrPropStringS(*this, key, outStr);
}

std::string Game::getVarOrPropStringS(const std::string_view key) const
{
	return variableManager.getVarOrPropStringS(*this, key);
}

std::string Game::getVarOrPropStringV(const Variable& var) const
{
	return variableManager.getVarOrPropStringV(*this, var);
}

std::shared_ptr<Action> Game::getQueryAction(const std::string_view prop) const
{
	return GameQueryable::getQueryAction(*this, prop);
}

bool Game::getProperty(const std::string_view prop, Variable& var) const
{
	return GameQueryable::getProperty(*this, prop, var);
}

bool Game::getProperty(const std::string_view prop1, const std::string_view prop2, Variable& var) const
{
	return GameQueryable::getProperty(*this, prop1, prop2, var);
}

bool Game::getGameProperty(const std::string_view prop, Variable& var) const
{
	return GameQueryable::getGameProperty(*this, prop, var);
}

bool Game::getGameProperty(const std::string_view prop1, const std::string_view prop2, Variable& var) const
{
	return GameQueryable::getGameProperty(*this, prop1, prop2, var);
}

void Game::setGameProperty(const std::string_view prop, const Variable& val)
{
	GameQueryable::setGameProperty(*this, prop, val);
}

QueryObject Game::getQueryable(const std::string_view prop) const
{
	return GameQueryable::getQueryable(*this, prop);
}

QueryObject Game::getQueryable(const std::string_view prop1, const std::string_view prop2) const
{
	return GameQueryable::getQueryable(*this, prop1, prop2);
}

std::vector<VarOrQueryObject> Game::getQueryableList(const std::string_view prop) const
{
	return GameQueryable::getQueryableList(*this, prop);
}

bool Game::getQueryableList(const std::string_view prop1,
	const std::string_view prop2, std::vector<VarOrQueryObject>& queryableList) const
{
	return GameQueryable::getQueryableList(prop1, prop2, queryableList);
}
