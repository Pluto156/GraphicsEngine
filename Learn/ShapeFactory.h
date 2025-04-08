#pragma once
class ShapeFactory
{
public:
	static Box* CreateBox(std::string name, float width, float height, float depth, CVector Position, CMatrix Rotation= CMatrix(), CEuler EulerAngles= CEuler(), float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);
	static Box* CreateBox(std::string name, float width, float height, float depth, CVector Position, float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);

	
};

