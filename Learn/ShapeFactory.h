#pragma once
class ShapeFactory
{
public:
	static Box* CreateBox(std::string name, float width, float height, float depth, CVector3 Position, CMatrix4 Rotation= CMatrix4(), CEuler EulerAngles= CEuler(), float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);
	static Box* CreateBox(std::string name, float width, float height, float depth, CVector3 Position, float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);

	
};

