#pragma once
#include <functional>

#include "gptsolver/subroutine/abi/abaqus_like_types.hpp"

namespace gptsolver::subroutine {
using UmatCallback = std::function<void(MaterialPointData&)>;
void register_umat(UmatCallback cb);
void call_umat(MaterialPointData& data);
}
