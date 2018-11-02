///* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SurfacePenalty.h"

registerMooseObject("electrodepApp", SurfacePenalty);

template <>
InputParameters
validParams<SurfacePenalty>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("penalty", 0.0, "surface energy penalty factor");
  params.addRequiredCoupledVar("nx", "Coupled variable");
  params.addRequiredCoupledVar("ny", "Coupled variable");
  params.addRequiredCoupledVar("eta", "Coupled variable");
  return params;
}

SurfacePenalty::SurfacePenalty(const InputParameters & parameters)
  : AuxKernel(parameters),
    // We can couple in a value from one of our kernels with a call to coupledValueAux
    _nx(coupledValue("nx")),
    _ny(coupledValue("ny")),
    _grad_eta(coupledGradient("eta"))
{
}

/**
 * Auxiliary Kernels override computeValue() instead of computeQpResidual().  Aux Variables
 * are calculated either one per elemenet or one per node depending on whether we declare
 * them as "Elemental (Constant Monomial)" or "Nodal (First Lagrange)".  No changes to the
 * source are necessary to switch from one type or the other.
 */
Real
SurfacePenalty::computeValue()
{
  return _penalty * (_nx[_qp] * _grad_eta[_qp](0) + _ny[_qp] * _grad_eta[_qp](1)) * (_nx[_qp] * _grad_eta[_qp](0) + _ny[_qp] * _grad_eta[_qp](1));
}
