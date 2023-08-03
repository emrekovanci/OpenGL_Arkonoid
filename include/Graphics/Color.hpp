#pragma once

#include <algorithm>

#include <glm/vec3.hpp>

struct Color
{
	constexpr Color();
	constexpr Color(float red, float green, float blue);

	static const Color Black;
	static const Color White;
	static const Color Gray;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Magenta;
	static const Color Cyan;

	glm::vec3 Rgb { };
};

constexpr Color::Color() = default;

constexpr Color::Color(float red, float green, float blue) :
	Rgb
	{
		std::clamp(red, 0.0f, 1.0f),
		std::clamp(green, 0.0f, 1.0f),
		std::clamp(blue, 0.0f, 1.0f)
	}
{ }

inline constexpr Color Color::Black(0.0f, 0.0f, 0.0f);
inline constexpr Color Color::White(1.0f, 1.0f, 1.0f);
inline constexpr Color Color::Gray(0.5f, 0.5f, 0.5f);
inline constexpr Color Color::Red(1.0f, 0.0f, 0.0f);
inline constexpr Color Color::Green(0.0f, 1.0f, 0.0f);
inline constexpr Color Color::Blue(0.0f, 0.0f, 1.0f);
inline constexpr Color Color::Yellow(1.0f, 1.0f, 0.0f);
inline constexpr Color Color::Magenta(1.0f, 0.0f, 1.0f);
inline constexpr Color Color::Cyan(0.0f, 1.0f, 1.0f);