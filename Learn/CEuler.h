#pragma once
#include <string>
#include "CVector3.h"
#include "CMatrix4.h"

class CEuler {
public:
    float h, p, b; // 欧拉角：偏航（heading），俯仰（pitch），滚转（bank）

    void Set(float fh, float fp, float fb);
    CVector3 ToCVector();
    CMatrix4 ToCMatrix() const;
    CQuaternion ToCQuaternion() const;
    void CEuler::Normal();//h,b角度在-180~180，p角度在-90~90。万向锁时,h代替所有b的度数。
    CEuler CEuler::GetNormal();//h,b角度在-180~180，p角度在-90~90。万向锁时,h代替所有b的度数。

    operator float* () { return &h; }

    CEuler operator+(const CEuler& other) const {
        return CEuler(h + other.h, p + other.p, b + other.b);
    }
    CEuler operator-(const CEuler& other) const {
        return CEuler(h - other.h, p - other.p, b - other.b);
    }
    CEuler operator-() const {
        return CEuler(-h, -p, -b);
    }

    CEuler();
    CEuler(float fh, float fp, float fb);
    std::string ToString();
};
