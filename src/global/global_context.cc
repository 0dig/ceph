// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2011 New Dream Network
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */

#include "common/ceph_context.h"
#include "global/global_context.h"

/*
 * Global variables for use from process context.
 */
CephContext g_ceph_context __attribute__((init_priority(103))) (0);
md_config_t *g_conf(g_ceph_context._conf);
DoutStreambuf <char, std::basic_string<char>::traits_type> *_doss(g_ceph_context._doss);

CephContext *get_global_context(void)
{
  return &g_ceph_context;
}
