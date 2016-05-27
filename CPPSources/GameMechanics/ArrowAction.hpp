#pragma once

#include "IComponent.hpp"
#include "q4d.h"


class PlayerController;

struct ArrowAction : public IComponent{
    
    ArrowAction(GameObject * const gameObject);
    ArrowAction(GameObject * const gameObject, q4d rot);
    virtual ~ArrowAction();
    
    virtual void update() override;

    q4d mActionRotation{0, v3d(0, 0, 1)};
    
    q4d initRotation{45, v3d(1, 0, 0)};
    
    v3d rotationAxis{0, 1, 0};
    
    float rotAngle{0.0f};
    
    void apply(const PlayerController * ctr);
    void show(v3d position, float rotation);
    void hide();
    
private:
    
    void rotationAnim();

    float randomRotationAngle();
    
};