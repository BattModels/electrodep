#ifndef KINETICSLCVECH_H
#define KINETICSLCVECH_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"
#include "JvarMapInterface.h"

class KineticslcVecH;

template <>
InputParameters validParams<KineticslcVecH>();

class KineticslcVecH: public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>
{
public:
  KineticslcVecH(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  unsigned int _n_id;
  const MaterialProperty<Real> & _h;
  const MaterialProperty<Real> & _Fbv;
  const MaterialProperty<Real> & _dFbvdu;
  const MaterialProperty<Real> & _dhdu;
  std::vector<const MaterialProperty<Real> *> _dFbvdarg;
  const VectorVariableGradient & _grad_n;
  VectorMooseVariable & _n_var;
  const VectorVariablePhiGradient & _vec_grad_phi;
  const Real _constfactor;
};

#endif // KINETICSLCVECH_H
