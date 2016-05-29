#pragma once

#include <memory>



template<typename T>
using sp = std::shared_ptr<T>;
template<typename T>
using up = std::unique_ptr<T>;
template<typename T>
using wp = std::weak_ptr<T>;
using string = std::string;


#include "btCollisionDispatcher.h"
/* Return true only if you change any variables of the contact point (such as the friction). */
using PhysicContactCallback = bool(*)(btManifoldPoint &point, const btCollisionObjectWrapper *obj0, int part0, int index0,
                                      const btCollisionObjectWrapper *obj1, int part1, int index1);

using PhysicNearCallback = void(*)(btBroadphasePair &pair, btCollisionDispatcher &despatcher, const btDispatcherInfo &info );


