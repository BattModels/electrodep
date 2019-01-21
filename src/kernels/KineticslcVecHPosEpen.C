
#include "KineticslcVecHPosEpen.h"
#include <cmath>

registerMooseObject("electrodepApp", KineticslcVecHPosEpen);

template <>
InputParameters
validParams<KineticslcVecHPosEpen>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add in KineticslcVecHPosEpen");
  params.addParam<Real>("constfactor", "energy penalty factor for lc");
  params.addRequiredCoupledVar("n", "coupled director of the liquid crystal");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addCoupledVar("args", "Vector of arguments of the f_name");
  params.addRequiredParam<MaterialPropertyName>("h_name", "The switching function");
  return params;
}

KineticslcVecHPosEpen::KineticslcVecHPosEpen(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>(parameters),
    _n_id(coupled("n")),
    _h(getMaterialProperty<Real>("h_name")),
    _Fbv(getMaterialProperty<Real>("f_name")),
    _dFbvdu(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _dhdu(getMaterialPropertyDerivative<Real>("h_name", _var.name())),
    _dFbvdarg(_coupled_moose_vars.size()),
    _grad_n(coupledVectorGradient("n")),
    _n_var(*getVectorVar("n", 0)),
    _vec_grad_phi(_assembly.gradPhi(_n_var)),
    _constfactor(getParam<Real>("constfactor"))
{
  for (unsigned int i = 0; i < _dFbvdarg.size(); ++i)
    _dFbvdarg[i] = &getMaterialPropertyDerivative<Real>("f_name", _coupled_moose_vars[i]->name());
}

Real
KineticslcVecHPosEpen::computeQpResidual()
{
  Real epen = (1.0 - _h[_qp]) * _constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
  return exp( epen ) * _Fbv[_qp] * _test[_i][_qp];
}

Real
KineticslcVecHPosEpen::computeQpJacobian()
{
  Real epen = (1.0 - _h[_qp]) * _constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
  //  return ( -_dhdu[_qp] * _constfactor * _grad_n[_qp].contract(_grad_n[_qp]) ) * exp(epen) * _dFbvdu[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  return ( -_dhdu[_qp] * _constfactor * _grad_n[_qp].contract(_grad_n[_qp]) ) * _phi[_j][_qp] * exp(epen) * _Fbv[_qp] * _test[_i][_qp] + exp(epen) * _dFbvdu[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}

Real
KineticslcVecHPosEpen::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real epen = (1.0 - _h[_qp]) * _constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
  if (jvar == _n_id)
    return 2.0 * (1.0 - _h[_qp]) * _constfactor * ( _vec_grad_phi[_j][_qp].contract(_grad_n[_qp]) ) * exp( epen ) * _Fbv[_qp] * _test[_i][_qp];
  else
    {
      const unsigned int cvar = mapJvarToCvar(jvar);
      return exp(epen) * (*_dFbvdarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
    }
}
