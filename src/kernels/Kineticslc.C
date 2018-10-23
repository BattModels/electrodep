
#include "Kineticslc.h"
#include <cmath>

registerMooseObject("electrodepApp", Kineticslc);

template <>
InputParameters
validParams<Kineticslc>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add in Kineticslc");
  params.addParam<Real>("K", 0.0, "energy penalty factor");
  params.addParam<Real>("m3permol", 1.0, "energy penalty factor");
  params.addParam<Real>("Rmol", 8.314, "energy penalty factor");
  params.addParam<Real>("T", 1.0, "energy penalty factor");
  params.addRequiredCoupledVar("cp", "coupled variable");
  params.addRequiredCoupledVar("cv", "coupled variable 2");
  params.addRequiredCoupledVar("nx", "coupled variable nx");
  params.addRequiredCoupledVar("ny", "coupled variable ny");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  return params;
}

Kineticslc::Kineticslc(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _K(getParam<Real>("K")),
    _m3permol(getParam<Real>("m3permol")),
    _Rmol(getParam<Real>("Rmol")),
    _T(getParam<Real>("T")),
    _cp_var(coupled("cp")),
    _cp(coupledValue("cp")),
    _cv_var(coupled("cv")),
    _cv(coupledValue("cv")),
    _nx_var(coupled("nx")),
    _grad_nx(coupledGradient("nx")),
    _ny_var(coupled("ny")),
    _grad_ny(coupledGradient("ny")),
    _F(getMaterialProperty<Real>("f_name")),
    _dFe(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _dFv(getMaterialPropertyDerivative<Real>("f_name", getVar("cv", 0)->name())),
    _dF(getMaterialPropertyDerivative<Real>("f_name", getVar("cp", 0)->name()))
{
}

Real
Kineticslc::computeQpResidual()
{
  Real constfactor = 1/2.0 * _K * _m3permol * pow(10., -12)  / ( _Rmol *  _T );
  Real epen = constfactor * _grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp];
  return exp( -epen ) * _F[_qp] * _test[_i][_qp];
}

Real
Kineticslc::computeQpJacobian()
{
  Real constfactor = 1/2.0 * _K * _m3permol * pow(10., -12)  / ( _Rmol *  _T );
  return _dFe[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}

Real
Kineticslc::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real constfactor = 1/2.0 * _K * _m3permol * pow(10., -12)  / ( _Rmol *  _T );
  if (jvar == _cp_var)
    return _dF[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _cv_var)
    return _dFv[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _nx_var)
    {
      Real epen = constfactor * _grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp];
      return -constfactor * (_grad_phi[_j][_qp] * _grad_nx[_qp] + _grad_nx[_qp] * _grad_phi[_j][_qp]) * exp( -epen ) * _F[_qp] * _test[_i][_qp];
    }
  else if (jvar == _ny_var)
    {
      Real epen = constfactor * _grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp];
      return -constfactor * (_grad_phi[_j][_qp] * _grad_ny[_qp] + _grad_ny[_qp] * _grad_phi[_j][_qp]) * exp( -epen ) * _F[_qp] * _test[_i][_qp];
    }
  else
    return 0;
}
