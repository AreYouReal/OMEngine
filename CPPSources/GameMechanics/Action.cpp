#include "Action.hpp"




void Action::apply(const PlayerController *ctr){
    
}

void Action::remove(){
    if(arrowObj != nullptr){
        arrowObj.reset();
    }
}
