#include "electrodepApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<electrodepApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

electrodepApp::electrodepApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  electrodepApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  electrodepApp::associateSyntax(_syntax, _action_factory);

  Moose::registerExecFlags(_factory);
  ModulesApp::registerExecFlags(_factory);
  electrodepApp::registerExecFlags(_factory);
}

electrodepApp::~electrodepApp() {}

void
electrodepApp::registerApps()
{
  registerApp(electrodepApp);
}

void
electrodepApp::registerObjects(Factory & factory)
{
    Registry::registerObjectsTo(factory, {"electrodepApp"});
}

void
electrodepApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & action_factory)
{
  Registry::registerActionsTo(action_factory, {"electrodepApp"});

  /* Uncomment Syntax parameter and register your new production objects here! */
}

void
electrodepApp::registerObjectDepends(Factory & /*factory*/)
{
}

void
electrodepApp::associateSyntaxDepends(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}

void
electrodepApp::registerExecFlags(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new execution flags here! */
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
electrodepApp__registerApps()
{
  electrodepApp::registerApps();
}

extern "C" void
electrodepApp__registerObjects(Factory & factory)
{
  electrodepApp::registerObjects(factory);
}

extern "C" void
electrodepApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  electrodepApp::associateSyntax(syntax, action_factory);
}

extern "C" void
electrodepApp__registerExecFlags(Factory & factory)
{
  electrodepApp::registerExecFlags(factory);
}
