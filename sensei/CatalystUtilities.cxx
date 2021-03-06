#include "CatalystUtilities.h"

#include <vtkNew.h>
#include <vtkSMParaViewPipelineController.h>
#include <vtkSMPropertyHelper.h>
#include <vtkSMProxy.h>
#include <vtkSMProxyManager.h>
#include <vtkSMSessionProxyManager.h>
#include <vtkSMSourceProxy.h>

#include <vtkSMParaViewPipelineControllerWithRendering.h>
#include <vtkSMRenderViewProxy.h>
#include <vtkSMRepresentationProxy.h>
#include <vtkSMPVRepresentationProxy.h>
#include <vtkDataObject.h>

namespace sensei
{
namespace catalyst
{

// --------------------------------------------------------------------------
vtkSMSourceProxy* CreatePipelineProxy(const char* group,
  const char* name, vtkSMProxy* input)
{
  vtkSMSessionProxyManager* pxm =
    vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager();
  vtkSmartPointer<vtkSMProxy> proxy;
  proxy.TakeReference(pxm->NewProxy(group, name));
  if (!proxy || !vtkSMSourceProxy::SafeDownCast(proxy))
    {
    return NULL;
    }
  vtkNew<vtkSMParaViewPipelineController> controller;
  controller->PreInitializeProxy(proxy);
  if (input)
    {
    vtkSMPropertyHelper(proxy, "Input").Set(input);
    }
  controller->PostInitializeProxy(proxy);
  controller->RegisterPipelineProxy(proxy);
  return vtkSMSourceProxy::SafeDownCast(proxy);
}

// --------------------------------------------------------------------------
void DeletePipelineProxy(vtkSMProxy* proxy)
{
  if (proxy)
    {
    vtkNew<vtkSMParaViewPipelineController> controller;
    controller->UnRegisterProxy(proxy);
    }
}

// --------------------------------------------------------------------------
vtkSMViewProxy* CreateViewProxy(const char* group, const char* name)
{
  vtkSMSessionProxyManager* pxm =
    vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager();
  vtkSmartPointer<vtkSMProxy> proxy;
  proxy.TakeReference(pxm->NewProxy(group, name));
  if (!proxy || !vtkSMViewProxy::SafeDownCast(proxy))
    {
    return NULL;
    }
  vtkNew<vtkSMParaViewPipelineController> controller;
  controller->InitializeProxy(proxy);
  controller->RegisterViewProxy(proxy);
  return vtkSMViewProxy::SafeDownCast(proxy);
}

vtkSMRepresentationProxy* Show(vtkSMSourceProxy* producer, vtkSMViewProxy* view)
{
  vtkNew<vtkSMParaViewPipelineControllerWithRendering> controller;
  return vtkSMRepresentationProxy::SafeDownCast(
    controller->Show(producer, 0, view));
}

}
}
