#ifndef KINETICSLIQCRYSVAR_H
#define KINETICSLIQCRYSVAR_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"
#include "JvarMapInterface.h"

class KineticsLiqCrysVar;

template <>
InputParameters validParams<KineticsLiqCrysVar>();

class KineticsLiqCrysVar: public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>
{
public:
  KineticsLiqCrysVar(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  unsigned int _n_id;
  unsigned int _gradeta_id;
  const MaterialProperty<Real> & _h;
  const MaterialProperty<Real> & _Fbv;
  const MaterialProperty<Real> & _dFbvdu;
  const MaterialProperty<Real> & _dhdu;
  const MaterialProperty<Real> & _d2hdu2;
  std::vector<const MaterialProperty<Real> *> _dFbvdarg;
  const VectorVariableValue & _n;
  const VectorVariableGradient & _grad_n;
  const VectorVariableValue & _gradeta;
  const VectorVariableGradient & _grad_gradeta;
  VectorMooseVariable & _gradeta_var;
  const VectorVariablePhiValue & _gradeta_phi;
  const VectorVariablePhiGradient & _gradeta_grad_phi;
  VectorMooseVariable & _n_var;
  const VectorVariablePhiValue & _n_phi;
  const VectorVariablePhiGradient & _n_grad_phi;
  const Real _constfactor;
  const Real _penaltyconstfactor;
};

#endif // KINETICSLIQCRYSVAR_H
