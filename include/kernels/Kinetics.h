
#ifndef KINETICS_H
#define KINETICS_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"
#include "JvarMapInterface.h"

class Kinetics;

template <>
InputParameters validParams<Kinetics>();

class Kinetics: public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>
{
public:
  Kinetics(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const MaterialProperty<Real> & _Fbv;
  const MaterialProperty<Real> & _dFbvdu;
  std::vector<const MaterialProperty<Real> *> _dFbvdarg;
};

#endif // KINETICS_H
