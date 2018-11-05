///* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "EnergyPenaltyVec.h"

registerMooseObject("electrodepApp", EnergyPenaltyVec);

template <>
InputParameters
validParams<EnergyPenaltyVec>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("constfactor", 0.0, "energy penalty factor");
  params.addRequiredCoupledVar("n", "coupled director of the liquid crystal");
  return params;
}

EnergyPenaltyVec::EnergyPenaltyVec(const InputParameters & parameters)
  : AuxKernel(parameters),
    // We can couple in a value from one of our kernels with a call to coupledValueAux
    _grad_n(coupledVectorGradient("n")),
    _constfactor(getParam<Real>("constfactor"))
{
}

/**
 * Auxiliary Kernels override computeValue() instead of computeQpResidual().  Aux Variables
 * are calculated either one per elemenet or one per node depending on whether we declare
 * them as "Elemental (Constant Monomial)" or "Nodal (First Lagrange)".  No changes to the
 * source are necessary to switch from one type or the other.
 */
Real
EnergyPenaltyVec::computeValue()
{
  return _constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
}
