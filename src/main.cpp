#include <pch.hpp>
#include <iostream>
#include <OWL/OWL.hpp>
#include <OWL/Main.hpp>
#include <OWL/Time.hpp>

void DrawPixel(OWL::Vec4ub* _canvas, const OWL::Vec2i& _size, const OWL::Vec2i& _position, const OWL::Vec4ub& _color) {
	if(_position.x >= 0 && _position.x < _size.x && _position.y >= 0 && _position.y < _size.y) {
		_canvas[_position.y * _size.x + _position.x] = _color;
	}
}

void DrawRect(OWL::Vec4ub* _canvas, const OWL::Vec2i& _size, const OWL::Vec2i& _rectPosition, const OWL::Vec2i& _rectSize, const OWL::Vec4ub& _color) {
	for(int i = _rectPosition.x; i < _rectPosition.x + _rectSize.x; i++) {
		for(int j =_rectPosition.y; j < _rectPosition.y + _rectSize.y; j++) {
			DrawPixel(_canvas, _size, OWL::Vec2i(i, j), _color);
		}
	}
}

OWL::Vec4ub colorPalette[5] = {
	OWL::Vec4ub(4, 15, 15, 255),
	OWL::Vec4ub(36, 130, 50, 255),
	OWL::Vec4ub(43, 168, 74, 255),
	OWL::Vec4ub(170, 166, 168, 255),
	OWL::Vec4ub(252, 255, 252, 255)
};

void DrawStickRep(OWL::Vec4ub* _canvas, const OWL::Vec2i& _size, const OWL::Vec2f& _gamepad, bool _pressed, unsigned int _uiSize, float _x, float _y, float _scale) {
	DrawRect(
		_canvas,
		_size,
		OWL::Vec2f(_size) * OWL::Vec2f(_x, _y) - OWL::Vec2i(float(_uiSize) * _scale / 2),
		OWL::Vec2i(float(_uiSize) * _scale),
		colorPalette[3]
	);

	OWL::Vec2f positioning;
	// positioning = ((OWL::Vec2f)window.mouse.getPosition() / (OWL::Vec2f)window.getSize() - OWL::Vec2f(0.5f)) * OWL::Vec2f(2.0f);
	positioning = _gamepad;

	OWL::Vec4ub col = colorPalette[4];
	if(_pressed) col = colorPalette[2];

	DrawRect(
		_canvas,
		_size,
		OWL::Vec2f(_size) * OWL::Vec2f(_x, _y) - OWL::Vec2i(float(_uiSize) * _scale / 4) + positioning * OWL::Vec2i(float(_uiSize) * _scale / 4),
		OWL::Vec2i(float(_uiSize) * _scale / 2),
		col
	);
}

int main(int argc, char** args) {
	OWL::SoftwareContext context;
	OWL::Window window;
	window.setContext(context);

	window.gamepads.setCount(1);
	
	OWL::FPSLimiter eventDelay(60);
	while(window.isRunning()) {
		eventDelay.start();
		window.pollEvents();

		if(window.keyboard.isKeyPressed(window.keyboard.Escape)) {
			window.close();
		}
		if(window.keyboard.getKeyData().keyEnum == window.keyboard.F11) {
			window.setFullScreen(!window.isFullScreen());
		}

		context.setSize(window.getSize());
		context.clear(colorPalette[0]);

		unsigned int uiSize = (
			(window.getSize().x > window.getSize().y) ? 
			window.getSize().y : 
			window.getSize().x
		);

		DrawStickRep(context.getPixelData(), context.getSize(), window.gamepads[0].getLeftStick(), window.gamepads[0].isButtonPressed(OWL::Gamepad::ThumbLeft), uiSize, 0.5, 0.7, 0.4);

		context.blitToScreen();
		eventDelay.end();
	}

	return 0;
}