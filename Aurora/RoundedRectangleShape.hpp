////////////////////////////////////////////////////////////
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

//Class written by Overdrivr: https://github.com/Overdrivr

#ifndef ROUNDEDRECTANGLESHAPE_HPP
#define ROUNDEDRECTANGLESHAPE_HPP

#include <SFML/Graphics/Shape.hpp>

namespace sf
{
	class RoundedRectangleShape : public sf::Shape
	{
	public:
		explicit RoundedRectangleShape(const Vector2f& size = Vector2f(0, 0), float radius = 0, unsigned int cornerPointCount = 0);
		void setSize(const Vector2f& size);
		const Vector2f& getSize() const;
		void setCornersRadius(float radius);
		const float getCornersRadius() const;
		void setCornerPointCount(unsigned int count);
		virtual std::size_t getPointCount() const;
		virtual sf::Vector2f getPoint(std::size_t index) const;

	private:
		Vector2f mySize;
		float myRadius;
		unsigned int myCornerPointCount;
	};
}
#endif