#include "GameObject.hpp"


class PlayerController{
public:
    PlayerController(GameObject * const gameObject);
    ~PlayerController();
    
    void onTouch();
    
private:
    
    float playerSpeed = 10;
    
// Fields
    GameObject *mGo;
    RigidBodyComponent *mRigidBodyComp;

// Helpers
    void rotate();
    void refreshVelocity();
    
};