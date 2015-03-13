/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

 #pragma once

#include <vector>
#include <map>

#include "../Number.hpp"
#include "../Rectangle.hpp"
#include "../Interval.hpp"
#include "DataObject.hpp"
#include "../Direction.hpp"
#include "NoiseLayersFactory.hpp"
#include "TransformationMatrix.hpp"
#include "../random/RandomSeed.hpp"
#include "../InternalErrorException.hpp"

namespace geogen
{
	namespace genlib
	{
		class HeightProfile;

		/// A height map.
		class HeightMap : public DataObject
		{
		private:
			Rectangle rectangle;
			Height* heightData;
			Scale scale;
		public:
			HeightMap(Rectangle rectangle, Height height = 0, Scale scale = 1);
			~HeightMap();
			HeightMap(HeightMap const& other);
			HeightMap(HeightMap const& other, Rectangle cutoutRect);
			HeightMap& operator=(HeightMap const& other);

			/// Gets memory size of a height map with specified rectangle.
			/// @param interval The rectangle.
			/// @return The memory size.
			inline static unsigned GetMemorySize(Rectangle rect, Scale scale) { return sizeof(HeightMap) + sizeof(Height) * (rect * scale).GetSize().GetTotalLength(); };

			/// Gets memory size of the height map.
			/// @return The memory size.
			virtual unsigned GetMemorySize() const { return GetMemorySize(this->rectangle, this->scale); };

			inline Rectangle GetRectangle() const { return this->rectangle; }
			inline Height* GetHeightDataPtr() { return this->heightData; }

			inline Height& operator() (Coordinate x, Coordinate y)
			{
				return this->heightData[x + this->rectangle.GetSize().GetWidth() * y];
			}

			inline Height operator() (Coordinate x, Coordinate y) const
			{
				return this->heightData[x + this->rectangle.GetSize().GetWidth() * y];
			}

			inline Height& operator() (Point p)
			{
				return this->heightData[p.GetX() + this->rectangle.GetSize().GetWidth() * p.GetY()];
			}

			inline Height operator() (Point p) const
			{
				return this->heightData[p.GetX() + this->rectangle.GetSize().GetWidth() * p.GetY()];
			}

			inline Height operator() (double x, double y)
			{
				// Rudimentary protection against some floating point math errors
				if (floor(x) + 0.00005 > x) x = floor(x);
				if (floor(y) + 0.00005 > y) y = floor(y);

				Coordinate leftCoord = Coordinate(floor(x));
				Coordinate rightCoord = Coordinate(ceil(x));
				Coordinate topCoord = Coordinate(floor(y));
				Coordinate bottomCoord = Coordinate(ceil(y));
				
				// TODO: Optimize.
				Height top = leftCoord == rightCoord ? (*this)(leftCoord, topCoord) : Lerp(leftCoord, rightCoord, (*this)(leftCoord, topCoord), (*this)(rightCoord, topCoord), x);
				Height bottom = leftCoord == rightCoord ? (*this)(leftCoord, bottomCoord) : Lerp(leftCoord, rightCoord, (*this)(leftCoord, bottomCoord), (*this)(rightCoord, bottomCoord), x);
				return topCoord == bottomCoord ? top : Lerp(topCoord, bottomCoord, top, bottom, y);
			}

			inline Coordinate GetOriginX() const { return this->rectangle.GetPosition().GetX(); }
			inline Coordinate GetOriginY() const { return this->rectangle.GetPosition().GetY(); }
			inline Size1D GetWidth() const { return this->rectangle.GetSize().GetWidth(); }
			inline Size1D GetHeight() const { return this->rectangle.GetSize().GetHeight(); }
			inline Scale GetScale() const { return this->scale; }

			inline Rectangle GetPhysicalRectangleUnscaled(Rectangle logicalRectangle) const { return logicalRectangle - this->rectangle.GetPosition(); }
			inline Rectangle GetPhysicalRectangle(Rectangle logicalRectangle) const { return logicalRectangle * this->scale - this->rectangle.GetPosition(); }
			inline Point GetPhysicalPointUnscaled(Point logicalPoint) const { return logicalPoint - this->rectangle.GetPosition(); }
			inline Point GetPhysicalPoint(Point logicalPoint) const { return logicalPoint * this->scale - this->rectangle.GetPosition(); }

			inline Coordinate GetPhysicalCoordinate(Coordinate logicalCoordinate, Direction direction) const 
			{ 
				switch (direction)
				{
				case DIRECTION_HORIZONTAL:
					return Coordinate(logicalCoordinate * this->scale) - this->rectangle.GetPosition().GetX();
				case DIRECTION_VERTICAL:
					return Coordinate(logicalCoordinate * this->scale) - this->rectangle.GetPosition().GetY();
				default:
					throw InternalErrorException(GG_STR("Invalid direction."));
				}
			}

			inline double GetPhysicalCoordinate(double logicalCoordinate, Direction direction) const
			{
				switch (direction)
				{
				case DIRECTION_HORIZONTAL:
					return logicalCoordinate * this->scale - this->rectangle.GetPosition().GetX();
				case DIRECTION_VERTICAL:
					return logicalCoordinate * this->scale - this->rectangle.GetPosition().GetY();
				default:
					throw InternalErrorException(GG_STR("Invalid direction."));
				}
			}

			inline Point GetLogicalPoint(Point physicalPoint) const { return (physicalPoint + this->rectangle.GetPosition()) / this->scale; }
			inline double GetLogicalX(double physicalCoordinate) const { return (physicalCoordinate + this->rectangle.GetPosition().GetX()) / this->scale; }
			inline double GetLogicalY(double physicalCoordinate) const { return (physicalCoordinate + this->rectangle.GetPosition().GetY()) / this->scale; }

			inline Coordinate GetLogicalCoordinate(Coordinate physicalCoordinate, Direction direction) const 
			{ 
				switch (direction)
				{
				case DIRECTION_HORIZONTAL:
					return Coordinate((physicalCoordinate + this->rectangle.GetPosition().GetX()) / this->scale);
				case DIRECTION_VERTICAL:
					return Coordinate((physicalCoordinate + this->rectangle.GetPosition().GetY()) / this->scale);
				default:
					throw InternalErrorException(GG_STR("Invalid direction."));
				}
				
			}

			inline double GetLogicalCoordinate(double physicalCoordinate, Direction direction) const
			{
				switch (direction)
				{
				case DIRECTION_HORIZONTAL:
					return (physicalCoordinate + this->rectangle.GetPosition().GetX()) / this->scale;
				case DIRECTION_VERTICAL:
					return (physicalCoordinate + this->rectangle.GetPosition().GetY()) / this->scale;
				default:
					throw InternalErrorException(GG_STR("Invalid direction."));
				}
			}

			inline Size1D GetScaledSize(Size1D size) const { return Size1D(size * this->scale); }
			
			void Abs();
			void Add(Height addend);
			void AddMasked(Height addend, HeightMap* mask);
			void AddMap(HeightMap* addend);
			void AddMapMasked(HeightMap* addend, HeightMap* mask);
			void Blur(Size1D radius);
			void Blur(Size1D radius, Direction direction);
			void ClampHeights(Height min, Height max);
			void Combine(HeightMap* other, HeightMap* mask);
			void ConvexityMap(Size1D radius);
			void Crop(Rectangle fillRectangle, Height height);
			void CropHeights(Height min, Height max, Height replace);
			void DistanceMap(Size1D distance, Direction direction);
			void DistanceMap(Size1D distance);
			void Distort(HeightMap* horizontalDistortionMap, HeightMap* verticalDistortionMap, Size1D maxDistance);
			void DrawLine(Point start, Point end, Height height);
			void FillRectangle(Rectangle fillRectangle, Height height);
			void Gradient(Point source, Point destination, Height fromHeight, Height toHeight);
			void Intersect(HeightMap* other);
			void Invert();
			void Move(Point offset);
			void Multiply(double factor);
			void MultiplyMap(HeightMap* factor);
			void Noise(NoiseLayers const& layers, random::RandomSeed seed);
			void NoiseLayer(Size1D waveLength, Height amplitude, random::RandomSeed seed, unsigned seedStep, bool isRidged);
			//void NormalMap();
			//void Outline();

			/// Projections.
			/// @param [in,out] profile If non-null, the profile.
			/// @param direction The direction.
			void Projection(HeightProfile* profile, Direction direction);
			void RadialGradient(Point point, Size1D radius, Height fromHeight, Height toHeight);
			//void Pattern(Rectangle repeatRectangle);
			void Pattern(HeightMap* pattern, Rectangle repeatRectangle);
			void Rescale(Scale horizontalScale, Scale verticalScale);
			void Resize(Rectangle rectangle, Height height);
			void Shift(HeightProfile* profile, Size1D maximumDistance, Direction direction);
			//void SelectHeights(Height min, Height max);
			//void SlopeMap();
			//void Shift(HeightProfile* profile, Size1D maxDistance);
			void Transform(TransformationMatrix const& matrix, Rectangle transformedRectangle);
			void TransformHeights(HeightProfile* function, Interval interval, Height min, Height max);
			void Unify(HeightMap* other);
		};
	}	
}