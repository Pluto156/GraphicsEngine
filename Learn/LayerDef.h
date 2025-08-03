// LayerDef.h
#pragma once
namespace PhysicsLit
{
    enum Layer : uint32_t {
        DEFAULT = 1<<0,
        PLAYER = 1<<1,
        ENEMY = 1<<2,
        BULLET = 1<<3,
        Item = 1<<4,
        // ...
    };
}

