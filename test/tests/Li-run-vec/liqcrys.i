# implementation of the grand-potential phase-field model based on M.Plapp PRE 84,031601(2011)+D.Cogswell PRE92, 011301(R) (2015)
# w is the chemical potential, eta is the phase-field, pot is the electric potential, op is the overpotential
# add Bulter-Volmer equation with non-linear kinetics by ZJH Oct 2nd 2017
# major modifs and liquid electrolyte with orientation order by Zeeshan Ahmad
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 200
  xmax = 200
  ny = 200
  ymax = 200
#  elem_type = QUAD9
[]

[GlobalParams]
  enable_jit = false
  penalty = 1.0
  constfactor = 1.007e-8
[]

[Variables]
  [./w]
   # order=SECOND
   # family= LAGRANGE
  [../]
  [./eta]
  #order=SECOND
   # family= LAGRANGE
  [../]
  [./pot]
  #order=SECOND
  # family= LAGRANGE
  [../]
  [./nlc]
  family = LAGRANGE_VEC
  [../]
  [./lambda]
  [../]
[]

#[AuxVariables]
#  [./epen]
#  order = FIRST
#  family = MONOMIAL
#  [../]
#  [./surfpen]
#  order = FIRST
#  family = MONOMIAL
#  [../]
#[]

[Functions]
[./ic_func_eta]
  #type = SmoothCircleIC
  #  variable = eta
  #  outvalue = 0.0
  #  invalue = 1.0
  #   y1=0
  #   x1=75
   #  radius=5
  type = ParsedFunction
  value = 0.5*(1.0-1.0*tanh((x-20)*2))
  [../]
  [./ic_func_c]
  type = ParsedFunction
  value = 0
  [../]
  [./ic_func_pot]
  type = ParsedFunction
#value=0.001*x-0.1
  value = -0.1*(1.0-tanh((x-20)*2)) 
[../]
  #[./ic_func_op]
#type = ParsedFunction
#value = 0
  #[../]
[]

[ICs]
  [./ic_nlc]
  type = VectorConstantIC
  variable = nlc
  x_value = 0
  y_value = 1
  z_value = 0
  [../]
  [./eta]
#  type = SpecifiedSmoothCircleIC
#  outvalue = 0.0
#  invalue = 1.0
#   x_positions='0 0 0 0 0'
#   z_positions='0 0 0 0 0'
#   y_positions='15 45 75 105 135'
#   radii='15 15 15 15 15'
  variable = eta
  type = FunctionIC
  function = ic_func_eta
  [../]
  [./w]
  variable = w
  type = FunctionIC
  function = ic_func_c
  [../]
  [./pot]
  variable = pot
  type = FunctionIC
  function = ic_func_pot
  [../]
[]

[BCs]
  [./bottom_eta]
  type = NeumannBC
  variable = 'eta'
  boundary = 'bottom'
  value = 0
  [../]
  [./top_eta]
  type = NeumannBC
  variable = 'eta'
  boundary = 'top'
  value = 0
  [../]
  [./left_eta]
  type = DirichletBC
  variable = 'eta'
  boundary = 'left'
  value = 1
  [../]
  [./right_eta]
  type = DirichletBC
  variable = 'eta'
  boundary = 'right'
  value = 0
  [../]
  [./bottom_w]
  type = NeumannBC
  variable = 'w'
  boundary = 'bottom'
  value = 0
  [../]
  [./top_w]
  type = NeumannBC
  variable = 'w'
  boundary = 'top'
  value = 0.0
  [../]
  [./left_w]
  type = NeumannBC
  variable = 'w'
  boundary = 'left'
  value = 0
  [../]
  [./right_w]
  type = DirichletBC
  variable = 'w'
  boundary = 'right'
  value = 0.0
  [../]
  [./left_pot]
  type = DirichletBC
  variable = 'pot'
  boundary = 'left'
  value = -0.2
  # -0.45
  [../]
  [./right_pot]
  type = DirichletBC
  variable = 'pot'
  boundary = 'right'
  value = 0
  [../]
  [./left_director]
  type = LagrangeVecDirichletBC
  variable = 'nlc'
  boundary = 'left'
  values = '0 1 0'
  [../]
  [./right_director]
  type = LagrangeVecDirichletBC
  variable = 'nlc'
  boundary = 'right'
  values = '0 1 0'
  [../]
[]

[Kernels]
  [./nlc]
  type = VectorDiffusion #Take care of the minus sign if adding any other terms
  variable = nlc
  [../]
  [./nlcxmag]
  type = NMagnitudeVecConstraintN
  variable = nlc
  lambda = lambda
  [../]
  [./lagrange]
  type = NMagnitudeVecConstraintLagrange
  variable = lambda
  vec = nlc
  epsilon = 1e-4
  [../]
  [./w_dot]
  type = SusceptibilityTimeDerivative
  variable = w
  f_name = chi
  args = 'w' # in this case chi (the susceptibility) is simply a constant
  [../]
  [./Diffusion1]
  type = MatDiffusion
  variable = w
  D_name = D
  [../] 
  [./Diffusion2]
  type = Diff
  variable = w
  cv = eta
  Q_name = zz
  QM_name = DN
  cp = pot
#args = 'w'
  [../]
  [./Noisew]
  type = LangevinNoise
  variable = w
  amplitude = 0.0
  [../]
  [./coupled_etadot]
  type = CoupledSusceptibilityTimeDerivative
  variable = w
  v = eta
  f_name = ft
  args = 'eta'
  [../]
  [./elecN]
  type = Electroneutrality
  variable = pot
  cp=eta
  cv =w
  Q_name = Le1    #before phi
  QM_name=0.
  [../]
  [./coupled_pos]
  type = CoupledSusceptibilityTimeDerivative
  variable = pot
  v = eta
  f_name = ft2
  args = 'eta'
  [../]
  [./BV]
  type = KineticslcVec
  variable = eta
  f_name = G
  cp=pot
  cv=eta
  n = 'nlc'
  [../]
  [./AC_bulk]
  type = AllenCahn
  variable = 'eta'
  f_name = FF
# args = 'w'
  [../]
  [./AC_bulk_lc_penalty]
  type = AnchorPenaltyVec
  variable = eta
  n = 'nlc'
  [../]
  [./AC_int]
  type = ACInterface
  variable = eta
  [../]
 [./Noiseeta]
  type = LangevinNoise
  variable = eta
  amplitude = 0.04
  [../]
  [./e_dot]
  type = TimeDerivative
  variable = eta
  [../]
[]

#[AuxKernels]
#  [./epen]
#  type = EnergyPenaltyVec
#  variable = epen
#  n = nlc
#  [../]
#  [./surfpenalty]
#  type = SurfacePenaltyVec
#  variable = surfpen
#  n = nlc
#  eta = eta
#  [../]
#[]

[Materials]
  [./constants]
  type = GenericConstantMaterial
#ul is the free energy density in the liquid phase
  prop_names  = 'kappa_op  M0    S1   S2   CL     L        Ls        B      es      el     zz    ul         us     dv'
  prop_values = '0.33    319.7  1e7  1.19  14.4   6.318   0.001899  2.64  -13.64   2.598    0   0.06854   13.64  5.306'
# Ls value changed after ZJH suggested 100 times the value for limiting current i0
#  M0 M1 Normalized diffusion coefficient for liquid and solid B normalized constant for nF
#  M0=10^12*D    B=zF/(1000*R*T)   length 1 um, time 1s, energy is normalized by RT
# t transference number D+=D/(n*(1-t)) D-=D/(n*t)
  [../]
  [./lc_const]
  type = GenericConstantMaterial
  prop_names = 'RT        Fconst         K     Liplusm3permol'
  prop_values = '0.987 0.9649   0.0000003957  0.05022'
  [../]
#  [./constfactor_lc]
#  type = ParsedMaterial
#  f_name = constfactor
#  function = '1/2 * K * Liplusm3permol / RT'
#  material_property_names = 'K Liplusm3permol RT'
#  [../]
  [./liquid_GrandPotential]
  type = DerivativeParsedMaterial
  function = 'ul - RT * log(1 + exp((w - el)/RT))'
  args = 'w'
  f_name = f1
  material_property_names = 'ul el RT'
    #outputs = exodus
  [../]
  [./solid_GrandPotential]
  type = DerivativeParsedMaterial
  function = 'us - RT * log(1 + exp((w - es) / RT))'
  args = 'w'
  f_name = f2
  material_property_names = 'us es RT'
   # outputs = exodus
  [../]
  [./switching_function]
  type = SwitchingFunctionMaterial
  eta ='eta'
  h_order = HIGH
  # outputs = exodus
  [../]
  [./barrier_function]
  type = BarrierFunctionMaterial
  eta = eta
  [../]
  [./total_GrandPotential]
  type = DerivativeTwoPhaseMaterial
  args = 'w'
  eta = eta
  fa_name = f1
  fb_name = f2
  derivative_order = 2
  W = 2.64
   # outputs = exodus
  [../]
  [./coupled_eta_function]
  type = DerivativeParsedMaterial
  function = '-(cs * dv - cl) * dh'  # in this code cs=-cs h=eta dh=1
  args = 'w eta'
  f_name = ft
  material_property_names = 'dh:=D[h,eta] h dv cs:=D[f2,w] cl:=D[f1,w]'
  derivative_order = 1
  [../]
  [./coupled_eta_function2]
  type = DerivativeParsedMaterial
  function = 'Fconst * CL * dv'  # in this code cs=-cs h=eta dh=1
#  args = 'w eta'
  f_name = ft2
  material_property_names = 'dv CL Fconst'
#dh:=D[h,eta] cs:=D[f2,w] cl:=D[f1,w] c0:=D[F,w]
  derivative_order = 1
  [../]
  [./susceptibility]
  type = DerivativeParsedMaterial
  function = '-d2F1 * (1-h) - d2F2 * h * dv'
  args = 'w'
  f_name = chi
  derivative_order = 1
#outputs = exodus
  material_property_names = 'h dv d2F1:=D[f1,w,w] d2F2:=D[f2,w,w]'
  [../]
  [./Diffusion_coefficient]
  type = DerivativeParsedMaterial     #cn=h*(1-cs)+(1-h)*(1-2*cl)
  function = '-M0 * cl * (1 - h)'  #c is -c, removed 1-h extra factor which was there for convergence
  f_name = D
  args = 'eta w'
  derivative_order = 1
  material_property_names = 'M0 cl:=D[f1,w] h'
  #  outputs = exodus
  [../]
  [./Free]
  type = DerivativeParsedMaterial
  f_name = FF
  material_property_names = 'B'
  args = 'eta'
  function = 'B * eta * eta * (1 - eta) * (1 - eta)'
  derivative_order = 1
  #outputs = exodus
  [../]
  [./Convection_coefficient]
  type = DerivativeParsedMaterial     #cn=h*(1-cs)+(1-h)*(1-2*cl)
  function = '-cl * (1-h) * Fconst / RT * M0 * (1-h)'  #c is -c
  args = 'eta w'
  f_name = DN
  derivative_order = 1
  material_property_names = 'M0 cl:=D[f1,w] h RT Fconst'
  #   outputs = exodus
  [../]
  [./ButlerVolmer]
  type = DerivativeParsedMaterial
  function = 'Ls * (exp(pot * Fconst/RT /2.) + cl * (1-h) * exp(-pot * Fconst/RT /2.)) * dh'
  #    function = 'L * (op)'
  args = 'pot eta w'
  f_name = G
  derivative_order = 1
  material_property_names = 'Ls dh:=D[h,eta] h cl:=D[f1,w] RT Fconst'
  outputs = other
  [../]
  [./eta]
  type = ParsedMaterial
  f_name = etao
  args='eta'
  function = 'eta'
  # outputs = exodus
  [../]
  [./concentration]
  type = ParsedMaterial
  f_name = c
  args='eta w'
  material_property_names = 'h dFl:=D[f1,w]'
  function = '-dFl * (1-h)'
   outputs = other
  [../]
  [./Le1]
  type = DerivativeParsedMaterial
  f_name = Le1
  args = 'eta'
  material_property_names = 'S1 S2 h'
  function = 'S1 * h + S2 * (1-h)'
  #outputs = exodus
  derivative_order = 1
  [../]
[]


[Postprocessors]
  [./ETA]
  type = ElementIntegralMaterialProperty
  mat_prop = etao
  execute_on = 'initial timestep_end'
  [../]
[]

[Preconditioning]
  [./SMP]
  type = SMP
  full = true
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      121                  preonly       lu           4'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  verbose = True
  solve_type = 'PJFNK'
  l_max_its = 50
  l_tol = 1e-5
  nl_max_its = 50
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-8
  petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      31                  preonly      ilu          1'
#  [./TimeStepper]
#    type = FunctionDT
#    time_t = '0        0.001    1    20'
#    time_dt ='1e-5    1.e-3   1.e-2 2.e-2 '
#     [../]
#  type = IterationAdaptiveDT
  dt=0.01
#  growth_factor = 1.5
#  optimal_iterations = 7
#  cutback_factor = 0.8
  end_time = 800
[]

[Outputs]
  exodus = false
  csv = true
  execute_on = 'TIMESTEP_END'
  [./other]        # creates input_other.e
  type = Exodus
  interval = 1
  [../]
  [./checkpt]
  type = Checkpoint
  num_files = 2
  interval = 20
  [../]
[]
