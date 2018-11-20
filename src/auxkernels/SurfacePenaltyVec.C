///* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SurfacePenaltyVec.h"

registerMooseObject("electrodepApp", SurfacePenaltyVec);

template <>
InputParameters
validParams<SurfacePenaltyVec>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("penalty", 0.0, "surface energy penalty factor");
  params.addRequiredCoupledVar("n", "Coupled variable");
  params.addRequiredCoupledVar("eta", "Coupled variable");
  return params;
}

SurfacePenaltyVec::SurfacePenaltyVec(const InputParameters & parameters)
  : AuxKernel(parameters),
    _n(coupledVectorValue("n")),
    _grad_eta(coupledGradient("eta")),
    _penalty(getParam<Real>("penalty"))
{
}

Real
SurfacePenaltyVec::computeValue()
{
  return _penalty * (_n[_qp] * _grad_eta[_qp]) * (_n[_qp] * _grad_eta[_qp]);
}
