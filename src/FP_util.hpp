#pragma once

#define Trait struct
#define Object(x) struct x final

#define ABSTRACT(CLS) protected: CLS() = default;

#include <optional>
template <typename T>
using Option = std::optional<T>;