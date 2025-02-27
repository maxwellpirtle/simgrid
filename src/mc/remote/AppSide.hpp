/* mc::remote::AppSide: the Application-side of the channel                 */

/* Copyright (c) 2015-2023. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SIMGRID_MC_CLIENT_H
#define SIMGRID_MC_CLIENT_H

#include "src/mc/remote/Channel.hpp"

#include <memory>

namespace simgrid::mc {

/** Model-checked-side of the communication protocol
 *
 *  Send messages to the model-checker and handles message from it.
 */
class XBT_PUBLIC AppSide {
private:
  Channel channel_;
  static std::unique_ptr<AppSide> instance_;
  bool need_memory_info_ = false; /* by default we don't send memory info, unless we got a NEED_MEMINFO */
  std::unordered_map<int, int> child_statuses_;

public:
  AppSide();
  explicit AppSide(int fd) : channel_(fd) {}
  void handle_messages();

private:
  void handle_deadlock_check(const s_mc_message_t* msg) const;
  void handle_simcall_execute(const s_mc_message_simcall_execute_t* message) const;
  void handle_finalize(const s_mc_message_int_t* msg) const;
  void handle_fork(const s_mc_message_fork_t* msg);
  void handle_wait_child(const s_mc_message_int_t* msg);
  void handle_need_meminfo();
  void handle_actors_status() const;
  void handle_actors_maxpid() const;

public:
  Channel const& get_channel() const { return channel_; }
  Channel& get_channel() { return channel_; }
  XBT_ATTRIB_NORETURN void main_loop();
  void report_assertion_failure();
  void ignore_memory(void* addr, std::size_t size) const;
  void ignore_heap(void* addr, std::size_t size) const;
  void unignore_heap(void* addr, std::size_t size) const;
  void declare_symbol(const char* name, int* value) const;
#if HAVE_UCONTEXT_H
  void declare_stack(void* stack, size_t size, ucontext_t* context) const;
#endif

  // TODO, remove the singleton antipattern.
  static AppSide* get();
};
} // namespace simgrid::mc

#endif
