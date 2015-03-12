#pragma once

#include <limits>

#include "CodeLocation.hpp"
#include "String.hpp"
#include "Size.hpp"
#include "Point.hpp"

namespace geogen
{
	/// Numeric type corresponding to script numbers. 
	typedef double Number;

	/// Height in range HEIGHT_MIN, HEIGHT_MAX.
	typedef short Height;

	/// Scale ratio.
	typedef double Scale;

	/// The minimum height.
	const Height HEIGHT_MIN = -32767;

	/// The maximum height.
	const Height HEIGHT_MAX = 32767;

	/// Mathematical constant Pi.
	const Number NUMBER_PI = 3.14159265358979323846;

	/// Converts String to Number.
	/// @param str The string.
	/// @return The number. 0 if the conversion failed.
	Number StringToNumber(String const& str);

	/// Determines whether a Number is an integer.
	/// @param n The number.
	/// @return true if the number is an integer, false otherwise.
	bool IsNumberInt(Number n);
	
	/// Converts a Number to an integer.
	/// @param n The number.
	/// @return The converted integer.
	int NumberToInt(Number n);

	/// Attempts to convert a Number to a Height. The number is expected to be in range <-1, 1>.
	/// @param n The Number to convert.
	/// @param out The conversion result.
	/// @return true if the conversion was successful, false otherwise.
	bool TryNumberToHeight(Number n, Height& out);

	/// Attempts to convert a Number to a Scale. The number is expected to be in range <0.1, 10>.
	/// @param n The Number to convert.
	/// @param out The conversion result.
	/// @return true if the conversion was successful, false otherwise.
	bool TryNumberToScale(Number n, Scale& out);

	/// Attempts to convert a Number to Size1D. The number is expected to be in range SIZE_MIN, SIZE_MAX.
	/// @param n The Number to convert.
	/// @param out The conversion result.
	/// @return true if the conversion was successful, false otherwise.
	bool TryNumberToSize(Number n, Size1D& out);

	/// Converts a Number to a Height. The number is expected to be in range <-1, 1>.
	/// @param n The Number to convert.
	/// @return The converted Height.
	Height NumberToHeight(Number n);

	/// Returns sign of a numeric value.
	/// @tparam T Numeric type.
	/// @param val The value.
	/// @return -1 if the value was less than 0, 0 if it was 0 and 1 if it was greater than 0.
	template <typename T> int Sign(T val) {
		return (T(0) < val) - (val < T(0));
	}

	/// Returns @v squared.
	/// @tparam T Numeric type.
	/// @param val The value.
	/// @return The squared value.
	template <typename T> T Square(T val) {
		return val * val;
	}

	/// Linear interpolation between two values on two 1D coordinates. The two coordinates must not be the same. 
	/// @param fromCoord From coordinate (x0).
	/// @param toCoord To coordinate (x1).
	/// @param fromHeight From height (y0).
	/// @param toHeight To height (y1).
	/// @param point The point (x).
	/// @return An interpolated height.
	/// @see http://en.wikipedia.org/wiki/Linear_interpolation
	static inline Height Lerp(Coordinate fromCoord, Coordinate toCoord, Height fromHeight, Height toHeight, double point)
	{
		return Height(fromHeight + (toHeight - fromHeight) * (point - fromCoord) / (toCoord - fromCoord));
	}

	/// Adds two heights together in saturation arithmetic (the result will be clamped to range <geogen::HEIGHT_MIN, geogen::HEIGHT_MAX>).
	/// @param x Height 1.
	/// @param y Height 2.
	/// @return A height.
	/// @see http://en.wikipedia.org/wiki/Saturation_arithmetic Saturation arithmetic in Wikipedia
	static inline Height AddHeights(Height x, Height y) {
		// http://stackoverflow.com/questions/17580118/signed-saturated-add-of-64-bit-ints

		// Determine the lower or upper bound of the result
		Height ret = (x < 0) ? HEIGHT_MIN : HEIGHT_MAX;

		// This is always well defined:
		// If x < 0 this adds a positive value to HEIGHT_MIN
		// If x > 0 this subtracts a positive value from HEIGHT_MAX
		Height comp = ret - x;

		// The condition is equivalent to:
		// ((x < 0) && (y > comp)) || ((x >=0) && (y <= comp))
		if ((x < 0) == (y > comp)) ret = x + y;

		return ret;
	}

	/// Rounds a number to the next integer away from zero.
	/// @param x The number.
	/// @return The rounded number.
	static inline double RoundAway(double x)
	{
		return x > 0 ? ceil(x) : floor(x);
	}

}