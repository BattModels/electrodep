
#ifndef NMAGNITUDECONSTRAINTLAGRANGECOMPONENT_H
#define NMAGNITUDECONSTRAINTLAGRANGECOMPONENT_H

#include "Kernel.h"
//*#include "DerivativeMaterialInterface.h"
#include "NonlinearSystem.h"

// Forward Declarations
class NMagnitudeConstraintLagrangeComponent;

template <>
InputParameters validParams<NMagnitudeConstraintLagrangeComponent>();

/**
 * NMagnitudeConstraintLagrangeComponent is a constraint kernel that acts on the
 * lambda lagrange multiplier non-linear variables to
 * enforce \f$ \sum_n h_i(\eta_i) - \epsilon\lambda \equiv 1 \f$.
 */
class NMagnitudeConstraintLagrangeComponent : public Kernel
{
public:
  NMagnitudeConstraintLagrangeComponent(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int) override;

  unsigned int _nx_var, _ny_var;
  const VariableValue & _nx, & _ny;
  Real _epsilon;
};

#endif // NMAGNITUDECONSTRAINTLAGRANGECOMPONENT_H
