#include "Framework/NullLimiter.hh"
#include "CellCenterFVMData.hh"
#include "Framework/MethodStrategyProvider.hh"
#include "FiniteVolume.hh"

//////////////////////////////////////////////////////////////////////////////

using namespace COOLFluiD::Framework;

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {

  namespace Numerics {

    namespace FiniteVolume {

//////////////////////////////////////////////////////////////////////////////

MethodStrategyProvider<NullLimiter<CellCenterFVMData>,
                CellCenterFVMData,
                Limiter<CellCenterFVMData>,
                FiniteVolumeModule>
nullLimiterProvider("Null");

//////////////////////////////////////////////////////////////////////////////

    } // namespace FiniteVolume

  } // namespace Numerics

} // namespace COOLFluiD

//////////////////////////////////////////////////////////////////////////////
