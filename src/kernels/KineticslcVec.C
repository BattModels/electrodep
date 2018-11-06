
#include "KineticslcVec.h"
#include <cmath>

registerMooseObject("electrodepApp", KineticslcVec);

template <>
InputParameters
validParams<KineticslcVec>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add in KineticslcVec");
  params.addParam<Real>("constfactor", 0.0, "energy penalty factor for lc");
  params.addRequiredCoupledVar("cp", "coupled variable");
  params.addRequiredCoupledVar("cv", "coupled variable 2");
  params.addRequiredCoupledVar("n", "coupled director of the liquid crystal");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  return params;
}

KineticslcVec::KineticslcVec(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _cp_var(coupled("cp")),
    _cv_var(coupled("cv")),
    _n_id(coupled("n")),
    _cp(coupledValue("cp")),
    _cv(coupledValue("cv")),
    _grad_n(coupledVectorGradient("n")),
    _n_var(*getVectorVar("n", 0)),
    _vec_grad_phi(_assembly.gradPhi(_n_var)),
    _F(getMaterialProperty<Real>("f_name")),
    _dFe(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _dFv(getMaterialPropertyDerivative<Real>("f_name", getVar("cv", 0)->name())),
    _dF(getMaterialPropertyDerivative<Real>("f_name", getVar("cp", 0)->name())),
  _constfactor(getParam<Real>("constfactor"))
{
}

Real
KineticslcVec::computeQpResidual()
{
  Real epen = _constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
  return exp( -epen ) * _F[_qp] * _test[_i][_qp];
}

Real
KineticslcVec::computeQpJacobian()
{
  return _dFe[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}

Real
KineticslcVec::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _cp_var)
    return _dF[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _cv_var)
    return _dFv[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _n_id)
    {
      Real epen = _constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
      return -_constfactor * ( _vec_grad_phi[_j][_qp].contract(_grad_n[_qp]) + _grad_n[_qp].contract(_vec_grad_phi[_j][_qp]) ) * exp( -epen ) * _F[_qp] * _test[_i][_qp];
    }
  else
    return 0;
}
