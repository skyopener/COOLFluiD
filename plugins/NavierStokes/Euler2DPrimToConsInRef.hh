#ifndef COOLFluiD_Physics_NavierStokes_Euler2DPrimToConsInRef_hh
#define COOLFluiD_Physics_NavierStokes_Euler2DPrimToConsInRef_hh

//////////////////////////////////////////////////////////////////////////////

#include "Framework/VarSetMatrixTransformer.hh"

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {

  namespace Physics {

    namespace NavierStokes {
      class EulerTerm;

//////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a transformer of variables from roe to
 * characteristic starting from reference values
 *
 * @author Andrea Lani
 */
class Euler2DPrimToConsInRef : public Framework::VarSetMatrixTransformer {
public:

  /**
   * Default constructor without arguments
   */
  Euler2DPrimToConsInRef(Common::SafePtr<Framework::PhysicalModelImpl> model);

  /**
   * Default destructor
   */
  ~Euler2DPrimToConsInRef();

  /**
   * Set the transformation matrix
   */
  void setMatrixFromRef();

private:

  /**
   * Set the flag telling if the transformation is an identity one
   * @pre this method must be called during set up
   */
  bool getIsIdentityTransformation() const
  {
    return false;
  }

private: //data

  /// acquaintance of the PhysicalModel
  Common::SafePtr<EulerTerm> _model;

}; // end of class Euler2DPrimToConsInRef

//////////////////////////////////////////////////////////////////////////////

    } // namespace NavierStokes

  } // namespace Physics

} // namespace COOLFluiD

//////////////////////////////////////////////////////////////////////////////

#endif // COOLFluiD_Physics_NavierStokes_Euler2DPrimToConsInRef_hh
