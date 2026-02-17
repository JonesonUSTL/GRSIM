#include "gptsolver/subroutine/abi/subroutine_api.hpp"

namespace gptsolver::subroutine {
namespace { UmatCallback g_umat = nullptr; }
void register_umat(UmatCallback cb) { g_umat = std::move(cb); }
void call_umat(MaterialPointData& data) { if (g_umat) g_umat(data); }
}  // namespace gptsolver::subroutine
