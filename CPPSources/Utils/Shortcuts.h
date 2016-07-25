#pragma once

#include <memory>
#include <vector>


template<typename T>
using sp = std::shared_ptr<T>;
template<typename T>
using up = std::unique_ptr<T>;
template<typename T>
using wp = std::weak_ptr<T>;
using string = std::string;

template<typename T>
using vec = std::vector<T>;

#include "btCollisionDispatcher.h"
/* Return true only if you change any variables of the contact point (such as the friction). */
using PhysicContactCallback = bool(*)(btManifoldPoint &point, const btCollisionObjectWrapper *obj0, int part0, int index0,
                                      const btCollisionObjectWrapper *obj1, int part1, int index1);

using PhysicNearCallback = void(*)(btBroadphasePair &pair, btCollisionDispatcher &despatcher, const btDispatcherInfo &info );


