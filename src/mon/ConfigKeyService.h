// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2013 Inktank, Inc
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */
#ifndef CEPH_MON_CONFIG_KEY_SERVICE_H
#define CEPH_MON_CONFIG_KEY_SERVICE_H

#include "include/Context.h"
#include "mon/MonOpRequest.h"
#include "mon/MonitorDBStore.h"

class Paxos;
class Monitor;

class ConfigKeyService
{
public:
  enum {
    SERVICE_HEALTH     = 0x01,
    SERVICE_TIMECHECK  = 0x02,
    SERVICE_CONFIG_KEY = 0x03,
  };
  ConfigKeyService(Monitor *m, Paxos *p);
  ~ConfigKeyService() {}

  void start(epoch_t new_epoch);
  void finish();
  void shutdown();

  void init() { }
  bool dispatch(MonOpRequestRef op);
  bool service_dispatch(MonOpRequestRef op);

  void start_epoch() { }
  void finish_epoch() { }
  epoch_t get_epoch() const;
  void cleanup() { }
  void service_tick() { }

  int validate_osd_destroy(const int32_t id, const uuid_d& uuid);
  void do_osd_destroy(int32_t id, uuid_d& uuid);
  int validate_osd_new(
      const uuid_d& uuid,
      const std::string& dmcrypt_key,
      std::stringstream& ss);
  void do_osd_new(const uuid_d& uuid, const std::string& dmcrypt_key);

  int get_type() {
    return SERVICE_CONFIG_KEY;
  }

  std::string get_name() const {
    return "config_key";
  }
  void get_store_prefixes(std::set<std::string>& s) const;

protected:
  void service_shutdown() { }

private:
  Monitor *mon;
  Paxos *paxos;
  epoch_t epoch = 0;

  bool in_quorum() const;

  void start_tick();
  void cancel_tick();
  void tick();
  void set_update_period(double t);

  Context *tick_event = nullptr;
  double tick_period;

  int store_get(const std::string &key, ceph::buffer::list &bl);
  void store_put(const std::string &key, ceph::buffer::list &bl, Context *cb = NULL);
  void store_delete(MonitorDBStore::TransactionRef t, const std::string &key);
  void store_delete(const std::string &key, Context *cb = NULL);
  void store_delete_prefix(
      MonitorDBStore::TransactionRef t,
      const std::string &prefix);
  void store_list(std::stringstream &ss);
  void store_dump(std::stringstream &ss, const std::string& prefix);
  bool store_exists(const std::string &key);
  bool store_has_prefix(const std::string &prefix);

  static const std::string STORE_PREFIX;
};

#endif // CEPH_MON_CONFIG_KEY_SERVICE_H
