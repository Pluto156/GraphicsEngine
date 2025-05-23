#include "stdafx.h"
#include "ShapeFactory.h"
Box* ShapeFactory::CreateBox(std::string name, float width, float height, float depth, CVector Position, CMatrix Rotation, CEuler EulerAngles, float r, float g, float b, bool isShowLocalAxis) {
    return new Box(name, width, height, depth, Position, Rotation, EulerAngles, r, g, b, isShowLocalAxis);  
}

Box* ShapeFactory::CreateBox(std::string name, float width, float height, float depth, CVector Position, float r, float g, float b, bool isShowLocalAxis) {
    return new Box(name, width, height, depth, Position, r, g, b, isShowLocalAxis); 
}