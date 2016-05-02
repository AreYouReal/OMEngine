#include "GameObject.hpp"
#include <queue>

class PlayerController{
public:
    PlayerController(GameObject * const gameObject);
    ~PlayerController();
    
    void onTouch();
    
    void update();
    
    std::queue<float> actions{};
    
private:
    
    float playerSpeed = 10;
    
// Fields
    GameObject *mGo;
    RigidBodyComponent *mRigidBodyComp;

// Helpers
    void rotate();
    void refreshVelocity();
    
};