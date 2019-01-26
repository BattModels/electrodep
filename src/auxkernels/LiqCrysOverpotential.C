///* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "LiqCrysOverpotential.h"

registerMooseObject("electrodepApp", LiqCrysOverpotential);

template <>
InputParameters
validParams<LiqCrysOverpotential>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("constfactor", 0.0, "energy penalty factor");
  params.addParam<Real>("penaltyconstfactor", "energy penalty constant factor for surface");
  params.addRequiredCoupledVar("eta", "the order parameter");
  params.addRequiredCoupledVar("n", "coupled director of the liquid crystal");
  params.addRequiredCoupledVar("gradeta", "gradient of the order parameter");
  params.addRequiredParam<MaterialPropertyName>("h_name", "The switching function");
  return params;
}

LiqCrysOverpotential::LiqCrysOverpotential(const InputParameters & parameters)
  : DerivativeMaterialInterface<AuxKernel>(parameters),
    // We can couple in a value from one of our kernels with a call to coupledValueAux
    _eta(coupledValue("eta")),
    _eta_var(coupled("eta")),
    _h(getMaterialProperty<Real>("h_name")),
    _dhdu(getMaterialPropertyDerivative<Real>("h_name", getVar("eta", 0)->name())),
    _n(coupledVectorValue("n")),
    _grad_n(coupledVectorGradient("n")),
    _gradeta(coupledVectorValue("gradeta")),
    _grad_gradeta(coupledVectorGradient("gradeta")),
    _constfactor(getParam<Real>("constfactor")),
    _penaltyconstfactor(getParam<Real>("penaltyconstfactor"))
{
}

Real
LiqCrysOverpotential::computeValue()
{
  Real divn = _grad_n[_qp].tr();
  Real gradderiv =  -2.0 * (_n[_qp] * _gradeta[_qp]) * divn - 2.0 * _n[_qp] * (_grad_n[_qp] * _gradeta[_qp] + _grad_gradeta[_qp] * _n[_qp]); 
  Real epen = (- _dhdu[_qp]) * (_constfactor * _grad_n[_qp].contract(_grad_n[_qp]) + _penaltyconstfactor * (_n[_qp] * _gradeta[_qp]) * (_n[_qp] * _gradeta[_qp]) ) + _penaltyconstfactor * (1.0 - _h[_qp]) * gradderiv;
  return epen;
}
