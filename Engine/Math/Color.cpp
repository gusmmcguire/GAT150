#include "Color.h"
#include <string>

namespace gme {
	const Color Color::white{ 1,1,1 };
	const Color Color::red{ 1,0,0 };
	const Color Color::green{ 0,1,0 };
	const Color Color::blue{ 0,0,1 };
	const Color Color::purple{ .502f,0,.502f };
	const Color Color::cyan{ 0,1,1 };
	const Color Color::orange{ 1,0.5f,0 };
	const Color Color::black{ 0,0,0 };
	const Color Color::yellow{ 1,1,0 };


	std::istream& operator>>(std::istream& stream, Color& c)
	{
		std::string line;
		std::getline(stream, line);
		
		if (line.find("{") != std::string::npos) {
			//red
			std::string r = line.substr(line.find("{") + 1, line.find(",") - line.find("{") - 1);
			c.r = std::stof(r);
			
			//get rid of the first comma, so that we can get the last two values
			line = line.substr(line.find(",") + 1);

			//green
			std::string g = line.substr(0, line.find(","));
			c.g = std::stof(g);

			//blue
			std::string b = line.substr(line.find(",") +  1, line.find("}") - line.find(",") - 1);
			c.b = std::stof(b);
		}
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, Color& c)
	{
		stream << c.r << " " << c.g << " " << c.b;

		return stream;
	}

}