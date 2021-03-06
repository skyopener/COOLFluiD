#include "FiniteVolume/FiniteVolume.hh"
#include "FiniteVolume/SteadyMeshMovementUpdate.hh"
#include "FiniteVolume/FVMCC_PolyRec.hh"

#include "Framework/ComputeDummyStates.hh"
#include "Framework/MethodCommandProvider.hh"
#include "Framework/MeshData.hh"
#include "Framework/VolumeCalculator.hh"
#include "Framework/Node.hh"
#include "Framework/SetElementStateCoord.hh"
#include "Framework/SubSystemStatus.hh"
#include "Framework/ComputeFaceNormalsFVMCC.hh"

//////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace COOLFluiD::Framework;
using namespace COOLFluiD::Common;

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {

  namespace Numerics {

    namespace FiniteVolume {

//////////////////////////////////////////////////////////////////////////////

MethodCommandProvider<SteadyMeshMovementUpdate, CellCenterFVMData, FiniteVolumeModule> SteadyMeshMovementUpdateProvider("SteadyMeshMovementUpdate");

//////////////////////////////////////////////////////////////////////////////

SteadyMeshMovementUpdate::SteadyMeshMovementUpdate(const std::string& name) :
  CellCenterFVMCom(name),
  socket_nodes("nodes"),
  socket_states("states"),
  socket_isOutward("isOutward"),
  socket_normals("normals"),
  socket_faceAreas("faceAreas"),
  socket_volumes("volumes"),
  socket_gstates("gstates")
{
}

//////////////////////////////////////////////////////////////////////////////

void SteadyMeshMovementUpdate::execute()
{

  updateCellVolume();

  resetIsOutward();
  updateNormalsData();
  updateFaceAreas();
  updateReconstructor();

  //!->To modify the Ghost nodes, we need the normals -> after updateNormalsData
  modifyOffMeshNodes();

 }

//////////////////////////////////////////////////////////////////////////////

void SteadyMeshMovementUpdate::updateReconstructor()
{
  Common::SafePtr<FVMCC_PolyRec> polyRec = getMethodData().getPolyReconstructor().d_castTo<FVMCC_PolyRec>();

  polyRec->updateWeights();

}

//////////////////////////////////////////////////////////////////////////////

void SteadyMeshMovementUpdate::modifyOffMeshNodes()
{
  /// The cell-center nodes and ghost nodes have NOT been moved during the mesh adaptation!!!
  //
  /// Modify the element state coordinates

  SafePtr<vector<ElementTypeData> > elementType =
    MeshDataStack::getActive()->getElementTypeData();

  CFuint nbElemTypes = elementType->size();

  DataHandle < Framework::State*, Framework::GLOBAL > states = socket_states.getDataHandle();

  SafePtr<TopologicalRegionSet> cells = MeshDataStack::getActive()->
    getTrs("InnerCells");

  SafePtr<GeometricEntityPool<TrsGeoWithNodesBuilder> > geoBuilder =
    getMethodData().getGeoWithNodesBuilder();

  TrsGeoWithNodesBuilder::GeoData& geoData = geoBuilder->getDataGE();
  geoData.trs = cells;

  vector<State*> eState(1);

  CFuint elemID = 0;
  for (CFuint iType = 0; iType < nbElemTypes; ++iType) {

    ///@todo change this for other order!!
    std::string elemName = CFGeoShape::Convert::to_str( (*elementType)[iType].getGeoShape() ) + "LagrangeP1LagrangeP0";

    SelfRegistPtr<SetElementStateCoord> setStateCoord = Environment::Factory<SetElementStateCoord>::getInstance().
      getProvider(elemName)->create();

    const CFuint nbElemPerType = (*elementType)[iType].getNbElems();
    for (CFuint iElem = 0; iElem < nbElemPerType; ++iElem, ++elemID) {
      // build the cell
      geoData.idx = elemID;
      GeometricEntity *const currCell = geoBuilder->buildGE();

      const vector<Node*>& eNodes = *currCell->getNodes();
      eState[0] = states[elemID];

      setStateCoord->update(eNodes,eState);

      // release the cell
      geoBuilder->releaseGE();
    }
  }

  ///@todo Do the same for the ghost states
  // compute the dummy (ghost) states
  ComputeDummyStates computeDummyStates;
  computeDummyStates.setDataSockets(socket_normals, socket_gstates,socket_states, socket_nodes);
  computeDummyStates.setup();
  computeDummyStates.updateAllDummyStates();

}

//////////////////////////////////////////////////////////////////////////////

void SteadyMeshMovementUpdate::resetIsOutward()
{
  DataHandle<CFint> isOutward = socket_isOutward.getDataHandle();

  for(CFuint i=0; i<isOutward.size(); i++)
  {
    isOutward[i] = -1;
  }
}

//////////////////////////////////////////////////////////////////////////////

void SteadyMeshMovementUpdate::updateNormalsData()
{
  CFAUTOTRACE;

  Common::SafePtr<vector<ElementTypeData> > elemTypes =
    MeshDataStack::getActive()->getElementTypeData();

  SafePtr<DataSocketSink<CFreal> > sinkNormalsPtr = &socket_normals;
  SafePtr<DataSocketSink<CFint> > sinkIsOutwardPtr = &socket_isOutward;

  for (CFuint iType = 0; iType < elemTypes->size(); ++iType) {

    const CFuint geoOrder = (*elemTypes)[iType].getGeoOrder();
    const std::string elemName = (*elemTypes)[iType].getShape() + CFPolyOrder::Convert::to_str( geoOrder );

    Common::SelfRegistPtr<ComputeNormals> computeFaceNormals = Environment::Factory<ComputeNormals>::getInstance().
      getProvider("Face" + elemName)->create();

    const CFuint firstElem = (*elemTypes)[iType].getStartIdx();
    const CFuint lastElem  = (*elemTypes)[iType].getEndIdx();

    SelfRegistPtr<ComputeFaceNormalsFVMCC> faceNormalsComputer =
      computeFaceNormals.d_castTo<ComputeFaceNormalsFVMCC>();

    faceNormalsComputer->setSockets(sinkNormalsPtr, sinkIsOutwardPtr);
    (*faceNormalsComputer)(firstElem, lastElem);
  }

}

//////////////////////////////////////////////////////////////////////////////

void SteadyMeshMovementUpdate::updateFaceAreas()
{
  const CFuint totalNbFaces = MeshDataStack::getActive()->Statistics().getNbFaces();

  DataHandle<CFreal> normals = socket_normals.getDataHandle();

  const CFuint nbDim = PhysicalModelStack::getActive()->getDim();
  RealVector faceNormal(nbDim);
  for (CFuint iFace = 0; iFace < totalNbFaces; ++iFace) {
    const CFuint startID = iFace*nbDim;
    //Update the normals
    for (CFuint iDim = 0; iDim < nbDim; ++iDim) {
      faceNormal[iDim] = normals[startID + iDim];
    }

    //Compute-update the face Area
    /// @todo This is valid only for Cranck-Nicholson!!!!!! 0.5* (norm2+ pastFaceNormlas)
    (socket_faceAreas.getDataHandle())[iFace] = faceNormal.norm2();
  }
}

//////////////////////////////////////////////////////////////////////////////

void SteadyMeshMovementUpdate::updateCellVolume()
{
  DataHandle<CFreal> volumes = socket_volumes.getDataHandle();

  Common::SafePtr<TopologicalRegionSet> cells = MeshDataStack::getActive()->
    getTrs("InnerCells");

  Common::SafePtr<GeometricEntityPool<TrsGeoWithNodesBuilder> >
    geoBuilder = getMethodData().getGeoWithNodesBuilder();

  TrsGeoWithNodesBuilder::GeoData& geoData = geoBuilder->getDataGE();
  geoData.trs = cells;

  const CFuint nbElems = cells->getLocalNbGeoEnts();
  for (CFuint iElem = 0; iElem < nbElems; ++iElem) {
    // build the GeometricEntity
    geoData.idx = iElem;
    GeometricEntity *const cell = geoBuilder->buildGE();
    volumes[iElem] = cell->computeVolume();

    //release the GeometricEntity
    geoBuilder->releaseGE();
  }
}

//////////////////////////////////////////////////////////////////////////////

vector<SafePtr<BaseDataSocketSink> >
SteadyMeshMovementUpdate::needsSockets()
{
  std::vector<Common::SafePtr<BaseDataSocketSink> > result;

  result.push_back(&socket_nodes);
  result.push_back(&socket_states);
  result.push_back(&socket_isOutward);
  result.push_back(&socket_normals);
  result.push_back(&socket_faceAreas);
  result.push_back(&socket_volumes);
  result.push_back(&socket_gstates);
  return result;
}

//////////////////////////////////////////////////////////////////////////////

    } // namespace FiniteVolume

  } // namespace Numerics

} // namespace COOLFluiD
