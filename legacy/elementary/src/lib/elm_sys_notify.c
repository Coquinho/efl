#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>

#include "elm_priv.h"

#include "elm_sys_notify_dbus.eo.h"
#include "elm_sys_notify_dbus.eo.legacy.h"

#define MY_CLASS ELM_SYS_NOTIFY_CLASS

#define MY_CLASS_NAME        "Elm_Sys_Notify"
#define MY_CLASS_NAME_LEGACY "elm_sys_notify"

EAPI int ELM_EVENT_SYS_NOTIFY_NOTIFICATION_CLOSED = 0;
EAPI int ELM_EVENT_SYS_NOTIFY_ACTION_INVOKED      = 0;

typedef const Eo_Class *(*Class_Get_Func)(void);

static Elm_Sys_Notify *_singleton = NULL;

/*
 * Registration of notification servers is done UNIQUELY
 * in the two structures below.
 *   1) ALWAYS add a SRV_XXX before __SRV_LAST
 *   2) copy the #if ... #else ... #endif with the appropriate class
 *      getter (generated by Eolian) or NULL when unsupported
 *
 * The rest of the code relies on the Srv enum and _class_getters
 * to register/unregister notification servers.
 */

typedef enum
{
   SRV_DBUS = 0,
   __SRV_LAST /* Sentinel */
} Srv;

static Class_Get_Func _class_getters[__SRV_LAST] =
{
#ifdef ELM_SYS_NOTIFY_DBUS_CLASS
   [SRV_DBUS] = elm_sys_notify_dbus_class_get
#else
   [SRV_DBUS] = NULL
#endif
};



typedef struct
{
   Eo *servers[__SRV_LAST];
} Elm_Sys_Notify_Data;

/*============================================================================*
 *                  Constructor/Destructor - Singleton setup                  *
 *============================================================================*/

EOLIAN static Eo_Base *
_elm_sys_notify_eo_base_constructor(Eo                  *obj,
                                    Elm_Sys_Notify_Data *sd  EINA_UNUSED)
{
   if (_singleton != NULL)
     {
        ERR("Attempted to create another system notification manager");
        return NULL;
     }

   obj = eo_do_super_ret(obj, MY_CLASS, obj, eo_constructor());
   _singleton = obj;

   return obj;
}

EOLIAN static void
_elm_sys_notify_eo_base_destructor(Eo                  *obj,
                                   Elm_Sys_Notify_Data *sd  EINA_UNUSED)
{
   eo_do_super(obj, MY_CLASS, eo_destructor());
   _singleton = NULL;
}


/*============================================================================*
 *                           Notification Interface                           *
 *============================================================================*/

EOLIAN static void
_elm_sys_notify_elm_sys_notify_interface_send(const Eo *obj EINA_UNUSED,
                                              Elm_Sys_Notify_Data *sd,
                                              unsigned int replaces_id,
                                              const char *icon,
                                              const char *summary,
                                              const char *body,
                                              Elm_Sys_Notify_Urgency urgency,
                                              int timeout,
                                              Elm_Sys_Notify_Send_Cb cb,
                                              const void *cb_data)
{
   Srv i;

   /* Propagate to all registered servers */
   for (i = SRV_DBUS; i < __SRV_LAST; ++i)
     if (sd->servers[i])
       eo_do(sd->servers[i],
             elm_obj_sys_notify_interface_send(replaces_id,
                                               icon, summary, body,
                                               urgency, timeout,
                                               cb, cb_data));
}

EOLIAN static void
_elm_sys_notify_elm_sys_notify_interface_simple_send(const Eo *obj EINA_UNUSED,
                                                     Elm_Sys_Notify_Data *sd,
                                                     const char *icon,
                                                     const char *summary,
                                                     const char *body)
{
   Srv i;

   /* Propagate to all registered servers */
   for (i = SRV_DBUS; i < __SRV_LAST; ++i)
     if (sd->servers[i])
       eo_do(sd->servers[i],
             elm_obj_sys_notify_interface_simple_send(icon, summary, body));
}

EOLIAN static void
_elm_sys_notify_elm_sys_notify_interface_close(const Eo *obj EINA_UNUSED,
                                               Elm_Sys_Notify_Data *sd,
                                               unsigned int id)
{
   Srv i;

   /* Propagate to all registered servers */
   for (i = SRV_DBUS; i < __SRV_LAST; ++i)
     if (sd->servers[i])
       eo_do(sd->servers[i],
             elm_obj_sys_notify_interface_close(id));
}


/*============================================================================*
 *                                   Methods                                  *
 *============================================================================*/

EOLIAN static Eina_Bool
_elm_sys_notify_servers_set(Eo                     *obj  EINA_UNUSED,
                            Elm_Sys_Notify_Data    *sd,
                            Elm_Sys_Notify_Server   servers)
{
   Srv i;
   Class_Get_Func class_get;

   /* Update the notification servers */
   for (i = SRV_DBUS; i < __SRV_LAST; ++i)
     {
        if (sd->servers[i])
          {
             /* Delete if server type is not provided */
             if (!(servers & (1 << i)))
               eo_del(sd->servers[i]);
          }
        else
          {
             /* If server is required, create when nonexistant */
             if (servers & (1 << i))
               {
                  class_get = _class_getters[i];
                  if (!class_get)
                    {
                       CRI("Unsupported notification server");
                       return EINA_FALSE;
                    }

                  sd->servers[i] = eo_add(class_get(), NULL);
                  if (EINA_UNLIKELY(!(sd->servers[i])))
                    {
                       CRI("Failed to create notification server");
                       return EINA_FALSE;
                    }
               }
          }
     }

   return EINA_TRUE;
}

EOLIAN static Elm_Sys_Notify_Server
_elm_sys_notify_servers_get(Eo                  *obj EINA_UNUSED,
                            Elm_Sys_Notify_Data *sd)
{
   Elm_Sys_Notify_Server servers = ELM_SYS_NOTIFY_SERVER_NONE;
   Srv i;

   for (i = SRV_DBUS; i < __SRV_LAST; ++i)
     if (sd->servers[i])
       servers |= (1 << i);

   return servers;
}

EOLIAN static Elm_Sys_Notify *
_elm_sys_notify_singleton_get(Eo   *obj EINA_UNUSED,
                              void *sd  EINA_UNUSED)
{
   if (!_singleton)
     _singleton = eo_add(MY_CLASS, NULL);
   return _singleton;
}

EOLIAN static void
_elm_sys_notify_class_constructor(Eo_Class *klass EINA_UNUSED)
{
   ELM_EVENT_SYS_NOTIFY_NOTIFICATION_CLOSED = ecore_event_type_new();
   ELM_EVENT_SYS_NOTIFY_ACTION_INVOKED = ecore_event_type_new();
}

/*============================================================================*
 *                                 Legacy API                                 *
 *============================================================================*/

void
_elm_unneed_sys_notify(void)
{
   Elm_Sys_Notify *manager = elm_sys_notify_singleton_get();
   if (manager)
     {
        eo_do(manager,
              elm_obj_sys_notify_servers_set(ELM_SYS_NOTIFY_SERVER_NONE));
        eo_del(manager);
     }
}

EAPI Eina_Bool
elm_need_sys_notify(void)
{
   Elm_Sys_Notify_Server servers = ELM_SYS_NOTIFY_SERVER_NONE;
   Elm_Sys_Notify *manager;
   Srv i;

   /* In theory, there can be N notification managers, but
    * in the implementation there will be only one: the
    * singleton which is initialized here. */
   manager = elm_sys_notify_singleton_get();
   if (EINA_UNLIKELY(!manager))
     {
        CRI("Failed to get notification manager");
        return EINA_FALSE;
     }

   /* If servers have alread been configured, this is not the right
    * function to be called! */
   if (elm_sys_notify_servers_get(manager) != ELM_SYS_NOTIFY_SERVER_NONE)
     {
        ERR("Notification servers have already been configured. "
            "Use elm_sys_notify_servers_set() instead.");
        return EINA_FALSE;
     }

   /* Register available notification servers */
   for (i = SRV_DBUS; i < __SRV_LAST; ++i)
     if (_class_getters[i])
       servers |= (1 << i);

   /* If no server are available, don't even bother... */
   if (servers == ELM_SYS_NOTIFY_SERVER_NONE)
     return EINA_FALSE;

   return elm_sys_notify_servers_set(manager, servers);
}

EAPI void
elm_sys_notify_send(unsigned int            replaces_id,
                    const char             *icon,
                    const char             *summary,
                    const char             *body,
                    Elm_Sys_Notify_Urgency  urgency,
                    int                     timeout,
                    Elm_Sys_Notify_Send_Cb  cb,
                    const void             *cb_data)
{
   eo_do(_singleton,
         elm_obj_sys_notify_interface_send(replaces_id,
                                           icon, summary, body,
                                           urgency, timeout,
                                           cb, cb_data));
}

EAPI void
elm_sys_notify_close(unsigned int id)
{
   eo_do(_singleton,
         elm_obj_sys_notify_interface_close(id));
}

#include "elm_sys_notify.eo.c"

