/* virtu - virtualization layer for the logging to know about the actors    */

/* Copyright (c) 2007-2023. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef XBT_VIRTU_H
#define XBT_VIRTU_H

#include <simgrid/actor.h>
#include <xbt/base.h>

// avoid deprecation warning on include (remove entire file with XBT_ATTRIB_DEPRECATED_v337)
#ifndef XBT_VIRTU_H_NO_DEPRECATED_WARNING
#warning xbt/virtu.h is deprecated and will be removed in v3.37.
#endif

SG_BEGIN_DECL

XBT_ATTRIB_DEPRECATED_v337("Please use sg_actor_self_get_name()") static const char* xbt_procname(void)
{
  return sg_actor_self_get_name();
}

XBT_ATTRIB_DEPRECATED_v337("Please use sg_actor_self_get_pid()") static int xbt_getpid(void)
{
  return sg_actor_self_get_pid();
};

SG_END_DECL

#endif /* XBT_VIRTU_H */
