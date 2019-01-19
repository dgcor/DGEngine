#pragma once

#include "EventManager.h"
#include "FadeInOut.h"
#include "InputEvent.h"
#include "LoadingScreen.h"
#include <memory>
#include "Menu.h"
#include "Parser/ParseVariable.h"
#include "Queryable.h"
#include "ResourceManager.h"
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Variable.h"
#include <vector>

class Game : public sf::NonCopyable, public Queryable
{
private:
	sf::RenderWindow window;
	sf::RenderTexture gameTexture;
	sf::Sprite gameSprite;

	sf::Vector2u refSize{ 640, 480 };
	sf::Vector2u minSize{ 640, 480 };
	sf::Vector2u size{ 640, 480 };
	sf::Vector2u oldSize;
	sf::Vector2u drawRegionSize;
	unsigned framerate{ 0 };
	bool smoothScreen{ false };
	bool stretchToFit{ false };
	bool keepAR{ true };
	bool enableInput{ true };
	bool pauseOnFocusLoss{ false };
	bool paused{ false };

	sf::Vector2i mousePositioni;
	sf::Vector2f mousePositionf;

	sf::Event::MouseButtonEvent mousePressEvt{};
	sf::Event::MouseButtonEvent mouseReleaseEvt{};
	sf::Event::MouseMoveEvent mouseMoveEvt{};
	sf::Event::MouseWheelScrollEvent mouseScrollEvt{};
	sf::Event::KeyEvent keyPressEvt{};
	sf::Event::TextEvent textEnteredEvt{};
	sf::Event::TouchEvent touchBeganEvt{};
	sf::Event::TouchEvent touchMovedEvt{};
	sf::Event::TouchEvent touchEndedEvt{};
	bool mousePressed{ false };
	bool mouseReleased{ false };
	bool mouseMoved{ false };
	bool mouseScrolled{ false };
	bool keyPressed{ false };
	bool textEntered{ false };
	bool touchBegan{ false };
	bool touchMoved{ false };
	bool touchEnded{ false };

	unsigned musicVolume{ 100 };
	unsigned soundVolume{ 100 };
	unsigned gamma{ 30 };

	sf::Time elapsedTime;

	std::string path;
	std::string title;
	std::string version;

	ResourceManager resourceManager;
	EventManager eventManager;

	std::map<std::string, Variable> variables;

	std::unique_ptr<LoadingScreen> loadingScreen;
	std::unique_ptr<FadeInOut> fadeInOut;

	void processEvents();
	void onClosed();
	void onResized(const sf::Event::SizeEvent& evt);
	void onLostFocus() noexcept;
	void onGainedFocus() noexcept;

	void onTextEntered(const sf::Event::TextEvent& evt) noexcept;
	void onKeyPressed(const sf::Event::KeyEvent& evt);
	void onMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent& evt);
	void onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt);
	void onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt);
	void onMouseMoved(const sf::Event::MouseMoveEvent& evt);
	void onTouchBegan(const sf::Event::TouchEvent& evt);
	void onTouchMoved(const sf::Event::TouchEvent& evt);
	void onTouchEnded(const sf::Event::TouchEvent& evt);

	void updateMousePosition(const sf::Vector2i mousePos);
	void updateEvents();
	void drawCursor();
	void drawUI();
	void update();
	void drawAndUpdate();
	void drawFadeEffect();
	void drawWindow();

	void updateSize();
	void updateGameTexture();

public:
	~Game();

	void init();

	// recreates the given renderTexture using the current draw region size
	// and sets the smooth property.
	void recreateRenderTexture(sf::RenderTexture& renderTexture) const;

	unsigned getOpenGLDepthBits() const { return window.getSettings().depthBits; }
	unsigned getOpenGLStencilBits() const { return window.getSettings().stencilBits; }
	unsigned getOpenGLAntialiasingLevel() const { return window.getSettings().antialiasingLevel; }
	unsigned getOpenGLMajorVersion() const { return window.getSettings().majorVersion; }
	unsigned getOpenGLMinorVersion() const { return window.getSettings().minorVersion; }
	bool getOpenGLSRgbCapable() const { return window.getSettings().sRgbCapable; }

	const sf::Vector2u& OldWindowSize() const noexcept { return oldSize; }
	const sf::Vector2u& WindowSize() const noexcept { return size; }
	const sf::Vector2u& DrawRegionSize() const noexcept { return drawRegionSize; }
	sf::Vector2f DrawRegionSizef() const
	{
		return sf::Vector2f((float)drawRegionSize.x, (float)drawRegionSize.y);
	}
	void WindowSize(const sf::Vector2u& size_);
	const sf::Vector2u& RefSize() const noexcept { return refSize; }
	void RefSize(const sf::Vector2u& size_);
	const sf::Vector2u& MinSize() const noexcept { return minSize; }
	void MinSize(const sf::Vector2u& size_);
	unsigned Framerate() const noexcept { return framerate; }
	bool SmoothScreen() const noexcept { return smoothScreen; }
	bool StretchToFit() const noexcept { return stretchToFit; }
	bool KeepAR() const noexcept { return keepAR; }

	const sf::Vector2i& MousePositioni() const noexcept { return mousePositioni; }
	const sf::Vector2f& MousePositionf() const noexcept { return mousePositionf; }

	const sf::Event::MouseButtonEvent& MousePress() const noexcept { return mousePressEvt; }
	const sf::Event::MouseButtonEvent& MouseRelease() const noexcept { return mouseReleaseEvt; }
	const sf::Event::MouseMoveEvent& MouseMove() const noexcept { return mouseMoveEvt; }
	const sf::Event::MouseWheelScrollEvent& MouseScroll() const noexcept { return mouseScrollEvt; }
	const sf::Event::KeyEvent& KeyPress() const noexcept { return keyPressEvt; }
	const sf::Event::TextEvent& TextEntered() const noexcept { return textEnteredEvt; }
	const sf::Event::TouchEvent& TouchBegan() const noexcept { return touchBeganEvt; }
	const sf::Event::TouchEvent& TouchMoved() const noexcept { return touchMovedEvt; }
	const sf::Event::TouchEvent& TouchEnded() const noexcept { return touchEndedEvt; }

	bool wasMousePressed() const noexcept { return mousePressed; }
	bool wasMouseReleased() const noexcept { return mouseReleased; }
	bool wasMouseMoved() const noexcept { return mouseMoved; }
	bool wasMouseScrolled() const noexcept { return mouseScrolled; }
	bool wasKeyPressed() const noexcept { return keyPressed; }
	bool wasTextEntered() const noexcept { return textEntered; }
	bool hasTouchBegan() const noexcept { return touchBegan; }
	bool hasTouchMoved() const noexcept { return touchMoved; }
	bool hasTouchEnded() const noexcept { return touchEnded; }

	void clearMousePressed() noexcept { mousePressed = false; }
	void clearMouseReleased() noexcept { mouseReleased = false; }
	void clearMouseMoved() noexcept { mouseMoved = false; }
	void clearMouseScrolled() noexcept { mouseScrolled = false; }
	void clearKeyPressed() noexcept { keyPressed = false; }
	void clearTextEntered() noexcept { textEntered = false; }
	void clearTouchBegan() noexcept { touchBegan = false; }
	void clearTouchMoved() noexcept { touchMoved = false; }
	void clearTouchEnded() noexcept { touchEnded = false; }

	void clearInputEvents(InputEvent e) noexcept;

	void MinWidth(unsigned width_) noexcept { size.x = width_; }
	void MinHeight(unsigned height_) noexcept { size.y = height_; }
	void Framerate(unsigned framerate_);
	void SmoothScreen(bool smooth_);
	void StretchToFit(bool stretchToFit_);
	void KeepAR(bool keepAR_);
	void PauseOnFocusLoss(bool pause_) noexcept { pauseOnFocusLoss = pause_; }

	unsigned MusicVolume() const noexcept { return musicVolume; }
	void MusicVolume(unsigned volume) noexcept
	{
		musicVolume = std::min(volume, 100u);
	}
	unsigned SoundVolume() const noexcept { return soundVolume; }
	void SoundVolume(unsigned volume) noexcept
	{
		soundVolume = std::min(volume, 100u);
	}
	unsigned Gamma() const noexcept { return gamma; }
	void Gamma(unsigned gamma_) noexcept
	{
		gamma = std::clamp(gamma_, 30u, 100u);
	}

	void addPlayingSound(const sf::SoundBuffer& obj);
	void addPlayingSound(const sf::SoundBuffer* obj);

	const sf::Time& getElapsedTime() const noexcept { return elapsedTime; }

	const std::string& getPath() const noexcept { return path; }
	const std::string& getTitle() const noexcept { return title; }
	const std::string& getVersion() const noexcept { return version; }

	bool isInputEnabled() const noexcept { return enableInput; }
	void EnableInput(bool enable) noexcept { enableInput = enable; }

	ResourceManager& Resources() noexcept { return resourceManager; }
	const ResourceManager& Resources() const noexcept { return resourceManager; }
	EventManager& Events() noexcept { return eventManager; }

	void close() { window.close(); }
	void setIcon(unsigned int width, unsigned int height, const sf::Uint8* pixels)
	{
		window.setIcon(width, height, pixels);
	}
	void setMouseCursorVisible(bool show) { window.setMouseCursorVisible(show); }
	void setPath(const std::string& path_) { path = path_; }
	void setTitle(const std::string& title_);
	void setVersion(const std::string& version_) { version = version_; }

	LoadingScreen* getLoadingScreen() noexcept { return loadingScreen.get(); }
	void setLoadingScreen(std::unique_ptr<LoadingScreen> loadingScreen_) noexcept
	{
		loadingScreen = std::move(loadingScreen_);
	}

	void draw();
	bool drawLoadingScreen();

	FadeInOut* getFadeInOut() const noexcept { return fadeInOut.get(); }
	void setFadeInOut(std::unique_ptr<FadeInOut> fadeInOut_) noexcept
	{
		fadeInOut = std::move(fadeInOut_);
	}

	void setMousePosition(sf::Vector2i mousePos);
	void updateMousePosition();
	void updateCursorPosition();

	void play();

	const std::map<std::string, Variable>& getVariables() const noexcept { return variables; }

	// gets variable without tokens. ex: "var"
	bool getVariableNoToken(const std::string& key, Variable& var) const;

	template <class T, class U>
	U getVarOrProp(const Variable& var, U defVal = U())
	{
		if (std::holds_alternative<T>(var) == true)
		{
			return (U)std::get<T>(var);
		}
		else if (std::holds_alternative<std::string>(var) == true)
		{
			Variable var2;
			if (getVarOrProp(std::get<std::string>(var), var2) == true)
			{
				if (std::holds_alternative<T>(var2) == true)
				{
					return (U)std::get<T>(var2);
				}
			}
		}
		return defVal;
	}

	bool getVarOrPropNoToken(const std::string_view key, Variable& var) const;
	bool getVarOrProp(const std::string_view key, Variable& var) const;
	Variable getVarOrProp(const Variable& var) const;
	bool getVarOrPropBoolS(const std::string_view key) const;
	bool getVarOrPropBoolV(const Variable& var) const;
	double getVarOrPropDoubleS(const std::string_view key) const;
	double getVarOrPropDoubleV(const Variable& var) const;
	int64_t getVarOrPropLongS(const std::string_view key) const;
	int64_t getVarOrPropLongV(const Variable& var) const;
	std::string getVarOrPropStringS(const std::string_view key) const;
	std::string getVarOrPropStringV(const Variable& var) const;

	// no tokens in key.
	void clearVariable(const std::string& key);

	// no tokens in key.
	void setVariable(const std::string& key, const Variable& value);

	void saveVariables(const std::string& filePath, const std::vector<std::string>& varNames) const;

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
	void setProperty(const std::string_view prop, const Variable& val);

	virtual const Queryable* getQueryable(const std::string_view prop) const;
};
