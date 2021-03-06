#ifndef COOLFluiD_Numerics_FluctSplit_WeakSlipWallEuler3DConsHO_hh
#define COOLFluiD_Numerics_FluctSplit_WeakSlipWallEuler3DConsHO_hh

//////////////////////////////////////////////////////////////////////////////



#include "FluctuationSplitData.hh"
#include "NavierStokes/Euler3DCons.hh"
#include "Framework/DataSocketSink.hh"


// basic file operations
#include <iostream>
#include <fstream>
//..................

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {



    namespace FluctSplit {

//////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a strong slip wall bc for Euler3D
 *
 * @author Andrea Lani
 *
 *
 *
 */
class WeakSlipWallEuler3DConsHO : public FluctuationSplitCom {

public:

  /**
   * Defines the Config Option's of this class
   * @param options a OptionList where to add the Option's
   */
  static void defineConfigOptions(Config::OptionList& options);

  /**
   * Constructor.
   */
  WeakSlipWallEuler3DConsHO(const std::string& name);

  /**
   * Default destructor
   */
  ~WeakSlipWallEuler3DConsHO();

  /**
   * Set up private data and data of the aggregated classes
   * in this command before processing phase
   */
  void setup();

  /**
   * Configures this object with supplied arguments.
   */
//   virtual void configure (const Config::ConfigArgs& args);  OLD
     virtual void configure (Config::ConfigArgs& args);  //NEW

  /**
   * Returns the DataSocket's that this command needs as sinks
   * @return a vector of SafePtr with the DataSockets
   */
  std::vector<Common::SafePtr<Framework::BaseDataSocketSink> > needsSockets();

protected:

  /**
   * Execute on a set of dofs
   */
  void executeOnTrs();

  /**
   * Compute the normal flux
   */
  void computeNormalFlux(const RealVector& state,
			 const RealVector& normal,
			 RealVector& flux) const;

  /**
   * Compute the face normal
   */
  void setFaceNormal(const CFuint faceID,
		     RealVector& normal);

private:

  // the socket to the data handle of the rhs
  Framework::DataSocketSink<CFreal> socket_rhs;

  // the socket to the data handle of the state's
  Framework::DataSocketSink < Framework::State* , Framework::GLOBAL > socket_states;

  // the socket to the data handle of the isUpdated flags
  Framework::DataSocketSink<bool> socket_isUpdated;

  // the socket to the data handle of the normals
  Framework::DataSocketSink<InwardNormalsData*> socket_normals;

  /// handle to the neighbor cell
  Framework::DataSocketSink<
  	Common::Trio<CFuint, CFuint, Common::SafePtr<Framework::TopologicalRegionSet> > >
        socket_faceNeighCell;

  /// physical model (in conservative variables)
  Common::SelfRegistPtr<Physics::NavierStokes::Euler3DCons> _varSet;

  /// temporary storage for the fluxes
  std::vector<RealVector>     _fluxes;

  /// say if the states are flagged inside this TRS
  std::valarray<bool>   _flagState;

  /// distribution coefficient
  CFreal                         _alpha;

//.........................
//  /// CD output flux file
//   std::ofstream CDfile;
//   CFreal Pnx;
//   void computeCD(const RealVector& state,
// 			 const RealVector& normal);
// //.........................

//...... EXACT NORMALS ........
  bool m_exact_norm;
//.............................





}; // end of class WeakSlipWallEuler3DConsHO

//////////////////////////////////////////////////////////////////////////////

    } // namespace FluctSplit



} // namespace COOLFluiD

//////////////////////////////////////////////////////////////////////////////

#endif // COOLFluiD_Numerics_FluctSplit_WeakSlipWallEuler3DConsHO_hh
