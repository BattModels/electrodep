///* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ElecDrivingForce.h"

registerMooseObject("electrodepApp", ElecDrivingForce);

template <>
InputParameters
validParams<ElecDrivingForce>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("constfactor", 0.0, "energy penalty factor");
  params.addRequiredCoupledVar("n", "coupled director of the liquid crystal");
  return params;
}

ElecDrivingForce::ElecDrivingForce(const InputParameters & parameters)
  : AuxKernel(parameters),
    // We can couple in a value from one of our kernels with a call to coupledValueAux
    _grad_n(coupledVectorGradient("n")),
    _constfactor(getParam<Real>("constfactor")),
    _Gox(getMaterialProperty<Real>("Gox")),
    _Gred(getMaterialProperty<Real>("Gred"))
{
}

Real
ElecDrivingForce::computeValue()
{
  Real epen = _constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
  return _Gox[_qp] + exp(-epen)*_Gred[_qp] ;
}
