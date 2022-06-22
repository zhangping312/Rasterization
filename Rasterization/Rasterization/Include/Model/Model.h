#pragma once

#include <string>
#include <vector>
#include <Math/Vectors.h>

class Model
{
private:
	std::vector<FW::Math::Vector3> m_Positions;
	std::vector<FW::Math::Vector2> m_TexCoordinates;
	std::vector<std::vector<int>> m_Faces;

public:
	Model(const std::string& filePath);
	Model(const Model& rhs) = delete;
	Model& operator=(const Model& rhs) = delete;
	~Model() = default;

	const FW::Math::Vector3& GetPosition(int index) { return m_Positions[index]; }
	const FW::Math::Vector2& GetTexCoordinate(int index) { return m_TexCoordinates[index]; }
	const std::vector<int>& GetFace(int index) { return m_Faces[index]; }

	int GetVerticesNum() const { return (int)m_Positions.size(); }
	int GetFacesNum() const { return (int)m_Faces.size(); }
};