#include "Euler2DSAPuvtToCons.hh"
#include "Framework/PhysicalModel.hh"
#include "Environment/ObjectProvider.hh"
#include "NavierStokes/EulerPhysicalModel.hh"
#include "SA.hh"

//////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace COOLFluiD::Framework;
using namespace COOLFluiD::MathTools;
using namespace COOLFluiD::Physics::NavierStokes;

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {

  namespace Physics {

    namespace SA {

//////////////////////////////////////////////////////////////////////////////

Environment::ObjectProvider<Euler2DSAPuvtToCons, VarSetTransformer, SAModule, 1> 
euler2DSAPuvtToConsProvider("Euler2DSAPuvtToCons");

//////////////////////////////////////////////////////////////////////////////

Euler2DSAPuvtToCons::Euler2DSAPuvtToCons
(Common::SafePtr<Framework::PhysicalModelImpl> model) :
  VarSetTransformer(model),
  _model(model->getConvectiveTerm().
	 d_castTo<MultiScalarTerm<EulerTerm> >())
{
}

//////////////////////////////////////////////////////////////////////////////

Euler2DSAPuvtToCons::~Euler2DSAPuvtToCons()
{
}

//////////////////////////////////////////////////////////////////////////////

void Euler2DSAPuvtToCons::transform(const State& state, State& result)
{

  const CFreal R = _model->getR();
  const CFreal p = state[0];
  const CFreal u = state[1];
  const CFreal v = state[2];
  const CFreal T = state[3];
  const CFreal Nutil = state[4];
  const CFreal rho = p/(R*T);
  const CFreal rhoV2 = rho*(u*u + v*v);

  result[0] = rho;
  result[1] = rho*u;
  result[2] = rho*v;
  result[3] = p/(_model->getGamma() - 1.) + 0.5*rhoV2; //rhoE
  result[4] = rho*Nutil;
}

//////////////////////////////////////////////////////////////////////////////

void Euler2DSAPuvtToCons::transformFromRef(const RealVector& data, State& result)
{
  const CFreal rho = data[EulerSATerm::RHO];

  result[0] = rho;
  result[1] = rho*data[EulerSATerm::VX];
  result[2] = rho*data[EulerSATerm::VY];
  result[3] = rho*data[EulerSATerm::H] - data[EulerSATerm::P]; //rhoE
  
  const CFuint iNutil = _model->getFirstScalarVar(0);
  result[4] = rho*data[iNutil];
}

//////////////////////////////////////////////////////////////////////////////

    } // namespace SA

  } // namespace Physics

} // namespace COOLFluiD

//////////////////////////////////////////////////////////////////////////////
