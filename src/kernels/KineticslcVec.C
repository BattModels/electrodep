
#include "KineticslcVec.h"
#include <cmath>

registerMooseObject("electrodepApp", KineticslcVec);

template <>
InputParameters
validParams<KineticslcVec>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add in KineticslcVec");
  params.addParam<Real>("K", 1.0, "energy penalty factor");
  params.addParam<Real>("m3permol", 1.0, "energy penalty factor");
  params.addParam<Real>("Rmol", 8.314, "energy penalty factor");
  params.addParam<Real>("T", 1.0, "energy penalty factor");
  params.addRequiredCoupledVar("cp", "coupled variable");
  params.addRequiredCoupledVar("cv", "coupled variable 2");
  params.addRequiredCoupledVar("n", "coupled director of the liquid crystal");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  return params;
}

KineticslcVec::KineticslcVec(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _K(getParam<Real>("K")),
    _m3permol(getParam<Real>("m3permol")),
    _Rmol(getParam<Real>("Rmol")),
    _T(getParam<Real>("T")),
    _cp_var(coupled("cp")),
    _cp(coupledValue("cp")),
    _cv_var(coupled("cv")),
    _cv(coupledValue("cv")),
    _n_id(coupled("n")),
    _grad_n(coupledVectorGradient("n")),
    _n_var(*getVectorVar("n", 0)),
    _vec_grad_phi(_assembly.gradPhi(_n_var)),
    _F(getMaterialProperty<Real>("f_name")),
    _dFe(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _dFv(getMaterialPropertyDerivative<Real>("f_name", getVar("cv", 0)->name())),
    _dF(getMaterialPropertyDerivative<Real>("f_name", getVar("cp", 0)->name()))
{
}

Real
KineticslcVec::computeQpResidual()
{
  Real constfactor = 1/2.0 * _K * _m3permol * pow(10., -12)  / ( _Rmol *  _T );
  //  Real epen = constfactor * _grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp];
  Real epen = constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
  return exp( -epen ) * _F[_qp] * _test[_i][_qp];
}

Real
KineticslcVec::computeQpJacobian()
{
  Real constfactor = 1/2.0 * _K * _m3permol * pow(10., -12)  / ( _Rmol *  _T );
  return _dFe[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}

Real
KineticslcVec::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real constfactor = 1/2.0 * _K * _m3permol * pow(10., -12)  / (_Rmol *  _T);
  if (jvar == _cp_var)
    return _dF[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _cv_var)
    return _dFv[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _n_id)
    {
      Real epen = constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
      return -constfactor * ( _vec_grad_phi[_j][_qp].contract(_grad_n[_qp]) + _grad_n[_qp].contract(_vec_grad_phi[_j][_qp]) ) * exp( -epen ) * _F[_qp] * _test[_i][_qp];
    }
  else
    return 0;
}
