/* Copyright (c) 2017-2023. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

/* This example demonstrates how to dynamically modify a graph of tasks.
 *
 * Assuming we have two instances of a service placed on different hosts,
 * we want to send data alternatively to thoses instances.
 *
 * We consider the following graph:
 *
 * comm0 -> exec1 -> comm1
 *     ↳-> exec2 ->comm2
 *
 * With exec1 and exec2 on different hosts.
 */

#include "simgrid/plugins/task.hpp"
#include "simgrid/s4u.hpp"

XBT_LOG_NEW_DEFAULT_CATEGORY(task_switch_host, "Messages specific for this task example");

int main(int argc, char* argv[])
{
  simgrid::s4u::Engine e(&argc, argv);
  e.load_platform(argv[1]);
  simgrid::plugins::Task::init();

  // Retrieve hosts
  auto* tremblay = e.host_by_name("Tremblay");
  auto* jupiter  = e.host_by_name("Jupiter");
  auto* fafard   = e.host_by_name("Fafard");

  // Create tasks
  auto comm0 = simgrid::plugins::CommTask::init("comm0");
  comm0->set_bytes(1e7);
  comm0->set_source(tremblay);
  auto exec1 = simgrid::plugins::ExecTask::init("exec1", 1e9, jupiter);
  auto exec2 = simgrid::plugins::ExecTask::init("exec2", 1e9, fafard);
  auto comm1 = simgrid::plugins::CommTask::init("comm1", 1e7, jupiter, tremblay);
  auto comm2 = simgrid::plugins::CommTask::init("comm2", 1e7, fafard, tremblay);

  // Create the initial graph by defining dependencies between tasks
  comm0->add_successor(exec2);
  exec1->add_successor(comm1);
  exec2->add_successor(comm2);

  // Add a function to be called when tasks end for log purpose
  simgrid::plugins::Task::on_end_cb([](const simgrid::plugins::Task* t) {
    XBT_INFO("Task %s finished (%d)", t->get_name().c_str(), t->get_count());
  });

  // Add a function to be called before each executions of comm0
  // This function modifies the graph of tasks by adding or removing
  // successors to comm0
  comm0->on_this_start([exec1, exec2, jupiter, fafard](simgrid::plugins::Task* t) {
    auto* comm0      = dynamic_cast<simgrid::plugins::CommTask*>(t);
    static int count = 0;
    if (count % 2 == 0) {
      comm0->set_destination(jupiter);
      comm0->add_successor(exec1);
      comm0->remove_successor(exec2);
    } else {
      comm0->set_destination(fafard);
      comm0->add_successor(exec2);
      comm0->remove_successor(exec1);
    }
    count++;
  });

  // Enqueue four executions for task comm0
  comm0->enqueue_execs(4);

  // Start the simulation
  e.run();
  return 0;
}
