#include "SpectralFV/SpectralFVBaseFunctionFaceLineP3.hh"

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {

  namespace SpectralFV {

//////////////////////////////////////////////////////////////////////////////

CFuint SpectralFVBaseFunctionFaceLineP3::_interpolatorID = 0;

//////////////////////////////////////////////////////////////////////////////

SpectralFVBaseFunctionFaceLineP3::SpectralFVBaseFunctionFaceLineP3()
{
}

//////////////////////////////////////////////////////////////////////////////

void SpectralFVBaseFunctionFaceLineP3::computeFaceJacobianDeterminant(
        const std::vector<RealVector>& m_mappedCoord,
        const std::vector<Framework::Node*>& nodes,
        const Framework::IntegratorPattern& pattern,
              std::vector<RealVector>& faceJacobian)
{
    throw Common::ShouldNotBeHereException (FromHere(),"Spectral finite volume base functions should not be used as geometrical shape functions.");
}

//////////////////////////////////////////////////////////////////////////////

RealVector SpectralFVBaseFunctionFaceLineP3::computeMappedCoordinates(const RealVector& coord,
                                    const std::vector<Framework::Node*>& nodes)
{
  throw Common::ShouldNotBeHereException (FromHere(),"Spectral finite volume base functions should not be used as geometrical shape functions.");
}

//////////////////////////////////////////////////////////////////////////////

RealVector SpectralFVBaseFunctionFaceLineP3::computeMappedCoordinatesPlus1D(const RealVector& coord,
                                    const std::vector<Framework::Node*>& nodes)
{
  throw Common::ShouldNotBeHereException (FromHere(),"Spectral finite volume base functions should not be used as geometrical shape functions.");
}

//////////////////////////////////////////////////////////////////////////////

  } // namespace SpectralFV

} // namespace COOLFluiD

//////////////////////////////////////////////////////////////////////////////
