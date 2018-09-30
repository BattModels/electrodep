//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef ELECTRODEPTESTAPP_H
#define ELECTRODEPTESTAPP_H

#include "MooseApp.h"

class electrodepTestApp;

template <>
InputParameters validParams<electrodepTestApp>();

class electrodepTestApp : public MooseApp
{
public:
  electrodepTestApp(InputParameters parameters);
  virtual ~electrodepTestApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
  static void registerExecFlags(Factory & factory);
};

#endif /* ELECTRODEPTESTAPP_H */
