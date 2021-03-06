#pragma once

#include <memory>

template<typename T>
using sp = std::shared_ptr<T>;
template<typename T>
using up = std::unique_ptr<T>;
template<typename T>
using wp = std::weak_ptr<T>;
using str = std::string;
template<typename T, typename V>
using pair = std::pair<T, V>;