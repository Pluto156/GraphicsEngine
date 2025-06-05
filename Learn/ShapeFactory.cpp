#include "stdafx.h"
#include "ShapeFactory.h"
Box* ShapeFactory::CreateBox(std::string name, float width, float height, float depth, CVector3 Position, CMatrix4 Rotation, CEuler EulerAngles, float r, float g, float b, bool isShowLocalAxis) {
    return new Box(name, width, height, depth, Position, Rotation, EulerAngles, r, g, b, isShowLocalAxis);  
}

Box* ShapeFactory::CreateBox(std::string name, float width, float height, float depth, CVector3 Position, float r, float g, float b, bool isShowLocalAxis) {
    return new Box(name, width, height, depth, Position, r, g, b, isShowLocalAxis); 
}

Sphere* ShapeFactory::CreateSphere(std::string name, float radius, CVector3 Position, float r, float g, float b, bool isShowLocalAxis) {
    return new Sphere(name, radius, Position, r, g, b, isShowLocalAxis);
}



