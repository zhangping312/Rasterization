#include "Model/Model.h"

#include <fstream>
#include <sstream>
#include <Logger/Logger.h>

using namespace FW::Math;

Model::Model(const std::string& filePath)
{
	std::ifstream in;
	in.open(filePath, std::ifstream::in);
	LOG_ASSERT(!in.fail(), "Failed to load model!");

	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (0 == line.compare(0, 2, "v "))
		{
			iss >> trash;
			Vector3 v;
			for (int i = 0; i < 3; ++i) iss >> v.values[i];
			m_Positions.push_back(v);
		}
		else if (0 == line.compare(0, 3, "vt "))
		{
			iss >> trash >> trash;
			Vector2 texCoordinate;
			for (int i = 0; i < 2; ++i) iss >> texCoordinate.values[i];
			m_TexCoordinates.push_back(texCoordinate);
		}
		else if (0 == line.compare(0, 2, "f "))
		{
			std::vector<int> f;
			int itrash, index;
			iss >> trash;
			while (iss >> index >> trash >> itrash >> trash >> itrash)
			{
				index--;
				f.push_back(index);
			}
			m_Faces.push_back(f);
		}
	}
}