#include "KineticsLiqCrysVar.h"
#include <cmath>
#include "Assembly.h"

registerMooseObject("electrodepApp", KineticsLiqCrysVar);

template <>
InputParameters
validParams<KineticsLiqCrysVar>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add in KineticsLiqCrysVar");
  params.addParam<Real>("constfactor", "energy penalty factor for lc");
  //  params.addParam<Real>("penalty", "energy penalty factor for surface");
  params.addParam<Real>("penaltyconstfactor", "energy penalty constant factor for surface");
  params.addRequiredCoupledVar("n", "coupled director of the liquid crystal");
  params.addRequiredCoupledVar("gradeta", "gradient of the order parameter");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addCoupledVar("args", "Vector of arguments of the f_name");
  params.addRequiredParam<MaterialPropertyName>("h_name", "The switching function");
  return params;
}

KineticsLiqCrysVar::KineticsLiqCrysVar(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>(parameters),
    _n_id(coupled("n")),
    _gradeta_id(coupled("gradeta")),
    _h(getMaterialProperty<Real>("h_name")),
    _Fbv(getMaterialProperty<Real>("f_name")),
    _dFbvdu(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _dhdu(getMaterialPropertyDerivative<Real>("h_name", _var.name())),
    _d2hdu2(getMaterialPropertyDerivative<Real>("h_name", _var.name(), _var.name())),
    _dFbvdarg(_coupled_moose_vars.size()),
    _n(coupledVectorValue("n")),
  _grad_n(coupledVectorGradient("n")),
  _gradeta(coupledVectorValue("gradeta")),
  _grad_gradeta(coupledVectorGradient("gradeta")),
  _gradeta_var(*getVectorVar("gradeta", 0)),
  _gradeta_phi(_assembly.phi(_gradeta_var)),
  _gradeta_grad_phi(_assembly.gradPhi(_gradeta_var)),
  _n_var(*getVectorVar("n", 0)),
  _n_phi(_assembly.phi(_n_var)),
  _n_grad_phi(_assembly.gradPhi(_n_var)),
  _constfactor(getParam<Real>("constfactor")),
  _penaltyconstfactor(getParam<Real>("penaltyconstfactor"))
{
  for (unsigned int i = 0; i < _dFbvdarg.size(); ++i)
    _dFbvdarg[i] = &getMaterialPropertyDerivative<Real>("f_name", _coupled_moose_vars[i]->name());
}

Real
KineticsLiqCrysVar::computeQpResidual()
{
  Real divn = _grad_n[_qp].tr();
  Real gradderiv =  -2.0 * (_n[_qp] * _gradeta[_qp]) * divn - 2.0 * _n[_qp] * (_grad_n[_qp] * _gradeta[_qp] + _grad_gradeta[_qp] * _n[_qp]); //There is a divergence in 1st term, not implemented yet
  Real epen = (- _dhdu[_qp]) * (_constfactor * _grad_n[_qp].contract(_grad_n[_qp]) + _penaltyconstfactor * (_n[_qp] * _gradeta[_qp]) * (_n[_qp] * _gradeta[_qp]) ) + _penaltyconstfactor * (1.0 - _h[_qp]) * gradderiv;
  return exp( epen ) * _Fbv[_qp] * _test[_i][_qp];
}

Real
KineticsLiqCrysVar::computeQpJacobian()
{
  Real divn = _grad_n[_qp].tr();
  Real gradderiv =  -2.0 * (_n[_qp] * _gradeta[_qp]) * divn -2.0 * _n[_qp] * (_grad_n[_qp] * _gradeta[_qp] + _grad_gradeta[_qp] * _n[_qp]); //There is a divergence in 1st term, not implemented yet
  Real epen = (- _dhdu[_qp]) * (_constfactor * _grad_n[_qp].contract(_grad_n[_qp]) + _penaltyconstfactor * (_n[_qp] * _gradeta[_qp]) * (_n[_qp] * _gradeta[_qp]) ) + _penaltyconstfactor * (1.0 - _h[_qp]) * gradderiv;
  Real pepenpuj = -_d2hdu2[_qp] * _phi[_j][_qp] * (_constfactor * _grad_n[_qp].contract(_grad_n[_qp]) + _penaltyconstfactor * (_n[_qp] * _gradeta[_qp]) * (_n[_qp] * _gradeta[_qp])) - _dhdu[_qp] * _phi[_j][_qp] * _penaltyconstfactor *  gradderiv;
  return pepenpuj * exp(epen) * _Fbv[_qp] * _test[_i][_qp] + exp( epen ) * _dFbvdu[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}

Real
KineticsLiqCrysVar::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real divn = _grad_n[_qp].tr();
  Real gradderiv =  -2.0 * (_n[_qp] * _gradeta[_qp]) * divn - 2.0 * _n[_qp] * (_grad_n[_qp] * _gradeta[_qp] + _grad_gradeta[_qp] * _n[_qp]); //There is a divergence in 1st term, not implemented yet
  Real epen = (- _dhdu[_qp]) * (_constfactor * _grad_n[_qp].contract(_grad_n[_qp]) + _penaltyconstfactor * (_n[_qp] * _gradeta[_qp]) * (_n[_qp] * _gradeta[_qp]) ) + _penaltyconstfactor * (1.0 - _h[_qp]) * gradderiv;
  if (jvar == _n_id)
    {
      Real divphi = _n_grad_phi[_j][_qp].tr();
      Real pgradderivpn = -2.0 * (_n_phi[_j][_qp] * _gradeta[_qp]) * divn - 2.0 * (_n[_qp] * _gradeta[_qp]) * divphi  - 2.0 * _n_phi[_j][_qp] * (_grad_n[_qp] * _gradeta[_qp] + _grad_gradeta[_qp] * _n[_qp]) - 2.0 * _n[_qp] * ( _n_grad_phi[_j][_qp] * _gradeta[_qp] + _grad_gradeta[_qp] * _n_phi[_j][_qp]);
      Real pepenpn = (- _dhdu[_qp]) * ( 2.0 * _constfactor * _n_grad_phi[_j][_qp].contract(_grad_n[_qp]) + 2.0 * _penaltyconstfactor * (_n_phi[_j][_qp] * _gradeta[_qp]) * (_n[_qp] * _gradeta[_qp]) ) + _penaltyconstfactor * (1.0 - _h[_qp]) * pgradderivpn;
      return pepenpn * exp( epen ) * _Fbv[_qp] * _test[_i][_qp];
    }
  else if(jvar == _gradeta_id)
    {
      Real pgradderivpgradeta = -2.0 * (_n[_qp] * _gradeta_phi[_j][_qp]) * divn - 2.0 * _n[_qp] * (_grad_n[_qp] * _gradeta_phi[_j][_qp] + _gradeta_grad_phi[_j][_qp] * _n[_qp]);
      Real pepenpgradeta = (- _dhdu[_qp]) * ( 2.0 * _penaltyconstfactor * (_n[_qp] * _gradeta_phi[_j][_qp]) * (_n[_qp] * _gradeta[_qp]) ) + _penaltyconstfactor * (1.0 - _h[_qp]) * pgradderivpgradeta;
      return pepenpgradeta * exp( epen ) * _Fbv[_qp] * _test[_i][_qp];
    }
  else
    {
      const unsigned int cvar = mapJvarToCvar(jvar);
      return exp(epen) * (*_dFbvdarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
    }
}
