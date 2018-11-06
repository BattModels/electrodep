
#include "Kineticslc.h"
#include <cmath>

registerMooseObject("electrodepApp", Kineticslc);

template <>
InputParameters
validParams<Kineticslc>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add in Kineticslc");
  params.addParam<Real>("constfactor", 0.0,"constfactor in liq crys, dimensions of m2");
  params.addRequiredCoupledVar("cp", "coupled variable");
  params.addRequiredCoupledVar("cv", "coupled variable 2");
  params.addRequiredCoupledVar("nx", "coupled variable nx");
  params.addRequiredCoupledVar("ny", "coupled variable ny");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  return params;
}

Kineticslc::Kineticslc(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _cp_var(coupled("cp")),
    _cv_var(coupled("cv")),
    _nx_var(coupled("nx")),
    _ny_var(coupled("ny")),
    _cp(coupledValue("cp")),
    _cv(coupledValue("cv")),
    _grad_nx(coupledGradient("nx")),
    _grad_ny(coupledGradient("ny")),
    _F(getMaterialProperty<Real>("f_name")),
    _dFe(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _dFv(getMaterialPropertyDerivative<Real>("f_name", getVar("cv", 0)->name())),
    _dF(getMaterialPropertyDerivative<Real>("f_name", getVar("cp", 0)->name())),
  _constfactor(getParam<Real>("constfactor"))
{
}

Real
Kineticslc::computeQpResidual()
{
  Real epen = _constfactor * _grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp];
  return exp( -epen ) * _F[_qp] * _test[_i][_qp];
}

Real
Kineticslc::computeQpJacobian()
{
  return _dFe[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}

Real
Kineticslc::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _cp_var)
    return _dF[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _cv_var)
    return _dFv[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _nx_var)
    {
      Real epen = _constfactor * _grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp];
      return -_constfactor * (_grad_phi[_j][_qp] * _grad_nx[_qp] + _grad_nx[_qp] * _grad_phi[_j][_qp]) * exp( -epen ) * _F[_qp] * _test[_i][_qp];
    }
  else if (jvar == _ny_var)
    {
      Real epen = _constfactor * _grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp];
      return -_constfactor * (_grad_phi[_j][_qp] * _grad_ny[_qp] + _grad_ny[_qp] * _grad_phi[_j][_qp]) * exp( -epen ) * _F[_qp] * _test[_i][_qp];
    }
  else
    return 0;
}
