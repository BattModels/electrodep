///* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "EnergyPenalty.h"

registerMooseObject("electrodepApp", EnergyPenalty);

template <>
InputParameters
validParams<EnergyPenalty>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("constfactor", 0.0, "energy penalty factor");
  params.addRequiredCoupledVar("nx", "Coupled variable");
  params.addRequiredCoupledVar("ny", "Coupled variable");
  return params;
}

EnergyPenalty::EnergyPenalty(const InputParameters & parameters)
  : AuxKernel(parameters),
    // We can couple in a value from one of our kernels with a call to coupledValueAux
    _grad_nx(coupledGradient("nx")),
    _grad_ny(coupledGradient("ny"))
{
}

/**
 * Auxiliary Kernels override computeValue() instead of computeQpResidual().  Aux Variables
 * are calculated either one per elemenet or one per node depending on whether we declare
 * them as "Elemental (Constant Monomial)" or "Nodal (First Lagrange)".  No changes to the
 * source are necessary to switch from one type or the other.
 */
Real
EnergyPenalty::computeValue()
{
  return _constfactor * (_grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp]);
}
