// Copyright (C) 2012 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef COOLFluiD_Numerics_SimpleGlobalMeshAdapter_GambitMeshGenerator_hh
#define COOLFluiD_Numerics_SimpleGlobalMeshAdapter_GambitMeshGenerator_hh

//////////////////////////////////////////////////////////////////////////////

#include "SimpleMeshAdapterData.hh"

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {

  namespace Numerics {

    namespace SimpleGlobalMeshAdapter {

//////////////////////////////////////////////////////////////////////////////

  /**
   * This class represents a NumericalCommand action to be
   * sent to generate a new mesh
   *
   * @author Thomas Wuilbaut
   *
   */
class GambitMeshGenerator : public SimpleMeshAdapterCom {
public:

  /**
   * Defines the Config Option's of this class
   * @param options a OptionList where to add the Option's
   */
  static void defineConfigOptions(Config::OptionList& options);

  /**
   * Constructor.
   */
  explicit GambitMeshGenerator(const std::string& name);

  /**
   * Destructor.
   */
  ~GambitMeshGenerator()
  {
  }

  /**
   * Execute Processing actions
   */
  void execute();

private:

  //Name of the mesh file
  std::string _filename;

  //Name of the journal file
  std::string _journalFile;

}; // class GambitMeshGenerator

//////////////////////////////////////////////////////////////////////////////

    } // namespace SimpleGlobalMeshAdapter

  } // namespace Numerics

} // namespace COOLFluiD

//////////////////////////////////////////////////////////////////////////////

#endif // COOLFluiD_Numerics_SimpleGlobalMeshAdapter_GambitMeshGenerator_hh

