/* Copyright (c) 2015-2023. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SIMGRID_MC_TRANSITION_SYNCHRO_HPP
#define SIMGRID_MC_TRANSITION_SYNCHRO_HPP

#include "src/mc/transition/Transition.hpp"

#include <cstdint>

namespace simgrid::mc {

class BarrierTransition : public Transition {
  unsigned bar_;

public:
  std::string to_string(bool verbose) const override;
  BarrierTransition(aid_t issuer, int times_considered, Type type, std::stringstream& stream);
  bool depends(const Transition* other) const override;
};

class MutexTransition : public Transition {
  uintptr_t mutex_;
  aid_t owner_;

public:
  std::string to_string(bool verbose) const override;
  MutexTransition(aid_t issuer, int times_considered, Type type, std::stringstream& stream);
  bool depends(const Transition* other) const override;
};

class SemaphoreTransition : public Transition {
  uintptr_t sem_;
  bool granted_;

public:
  std::string to_string(bool verbose) const override;
  SemaphoreTransition(aid_t issuer, int times_considered, Type type, std::stringstream& stream);
  bool depends(const Transition* other) const override;
};

} // namespace simgrid::mc

#endif
