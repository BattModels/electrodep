
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
  params.addRequiredCoupledVar("n", "coupled director of the liquid crystal");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addCoupledVar("args", "Vector of arguments of the f_name");
  return params;
}

KineticslcVec::KineticslcVec(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>(parameters),
    _n_id(coupled("n")),
    _Fbv(getMaterialProperty<Real>("f_name")),
    _dFbvdu(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
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
KineticslcVec::computeQpResidual()
{
  Real epen = _constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
  return exp( -epen ) * _Fbv[_qp] * _test[_i][_qp];
}

Real
KineticslcVec::computeQpJacobian()
{
  Real epen = _constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
  return exp(-epen) * _dFbvdu[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}

Real
KineticslcVec::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real epen = _constfactor * _grad_n[_qp].contract(_grad_n[_qp]);
  if (jvar == _n_id)
    return -2.0 * _constfactor * ( _vec_grad_phi[_j][_qp].contract(_grad_n[_qp]) ) * exp( -epen ) * _Fbv[_qp] * _test[_i][_qp];
  else
    {
      const unsigned int cvar = mapJvarToCvar(jvar);
      return exp(-epen) * (*_dFbvdarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
    }
}
