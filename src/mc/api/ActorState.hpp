/* Copyright (c) 2007-2023. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SIMGRID_MC_PATTERN_H
#define SIMGRID_MC_PATTERN_H

#include "src/kernel/activity/CommImpl.hpp"
#include "src/mc/remote/RemotePtr.hpp"

#include <algorithm>
#include <exception>
#include <vector>

namespace simgrid::mc {

/* On every state, each actor has an entry of the following type.
 * This usually represents both the actor and its transition because
 * most of the time an actor cannot have more than one enabled transition
 * at a given time. However, certain transitions have multiple "paths"
 * that can be followed, which means that a given actor may be able
 * to do more than one thing at a time.
 *
 * Formally, at this state multiple transitions would exist all of
 * which happened to be executed by the same actor. This distinction
 * is important in cases
 */
class ActorState {
  /**
   * @brief The transitions that the actor is allowed to execute from this
   * state, viz. those that are enabled for this actor
   *
   * Most actors can take only a single action from any given state.
   * However, when an actor executes a transition with multiple
   * possible variations (e.g. an MC_Random() [see class: RandomTransition]
   * for more details]), multiple enabled actions are defined
   *
   * @invariant The transitions are arranged such that an actor
   * with multiple possible paths of execution will contain all
   * such transitions such that `pending_transitions_[i]` represents
   * the variation of the transition with `times_considered = i`.
   *
   * @note: If only a subset of transitions of an actor that can
   * take multiple transitions in some state are truly enabled,
   * we would instead need to map `times_considered` to a transition,
   * as the map is currently implicit in the ordering of the transitions
   * in the vector
   *
   * TODO: If a single transition is taken at a time in a concurrent system,
   * then nearly all of the transitions from in a state `s'` after taking
   * an action `t` from state `s`  (i.e. s -- t --> s') are the same
   * sans for the new transition of the actor which just executed t.
   * This means there may be a way to store the list once and apply differences
   * rather than repeating elements frequently.
   */
  std::vector<std::shared_ptr<Transition>> pending_transitions_;

  /* Possible exploration status of an actor transition in a state.
   * Either the checker did not consider the transition, or it was considered and still to do, or considered and
   * done.
   */
  enum class InterleavingType {
    /** This actor transition is not considered by the checker (yet?) */
    disabled = 0,
    /** The checker algorithm decided that this actor transitions should be done at some point */
    todo,
    /** The checker algorithm decided that this should be done, but it was done in the meanwhile */
    done,
  };

  /** Exploration control information */
  InterleavingType state_ = InterleavingType::disabled;

  /** The ID of that actor */
  const aid_t aid_;

  /** Number of times that the actor was considered to be executed in previous explorations of the state space */
  unsigned int times_considered_ = 0;
  /** Maximal amount of times that the actor can be considered for execution in this state.
   * If times_considered==max_consider, we fully explored that part of the state space */
  unsigned int max_consider_ = 0;

  /** Whether that actor is initially enabled in this state */
  bool enabled_;

public:
  ActorState(aid_t aid, bool enabled, unsigned int max_consider) : ActorState(aid, enabled, max_consider, {}) {}

  ActorState(aid_t aid, bool enabled, unsigned int max_consider, std::vector<std::shared_ptr<Transition>> transitions)
      : pending_transitions_(std::move(transitions)), aid_(aid), max_consider_(max_consider), enabled_(enabled)
  {
  }

  unsigned int do_consider()
  {
    if (max_consider_ <= times_considered_ + 1)
      mark_done();
    return times_considered_++;
  }
  unsigned int get_max_considered() const { return max_consider_; }
  unsigned int get_times_considered() const { return times_considered_; }
  unsigned int get_times_not_considered() const { return max_consider_ - times_considered_; }
  bool has_more_to_consider() const { return get_times_not_considered() > 0; }
  aid_t get_aid() const { return aid_; }

  /* returns whether the actor is marked as enabled in the application side */
  bool is_enabled() const { return enabled_; }
  /* returns whether the actor is marked as disabled by the exploration algorithm */
  bool is_disabled() const { return this->state_ == InterleavingType::disabled; }
  bool is_done() const { return this->state_ == InterleavingType::done; }
  bool is_todo() const { return this->state_ == InterleavingType::todo; }
  /** Mark that we should try executing this process at some point in the future of the checker algorithm */
  void mark_todo()
  {
    this->state_            = InterleavingType::todo;
    this->times_considered_ = 0;
  }
  void mark_done() { this->state_ = InterleavingType::done; }

  /**
   * @brief Retrieves the transition that we should consider for execution by
   * this actor from the State instance with respect to which this ActorState object
   * is considered
   */
  std::shared_ptr<Transition> get_transition() const
  {
    // The rationale for this selection is as follows:
    //
    // 1. For transitions with only one possibility of execution,
    // we always wish to select action `0` even if we've
    // marked the transition already as considered (which
    // we'll do if we explore a trace following that transition).
    //
    // 2. For transitions that can be considered multiple
    // times, we want to be sure to select the most up-to-date
    // action. In general, this means selecting that which is
    // now being considered at this state. If, however, we've
    // executed the
    //
    // The formula satisfies both of the above conditions:
    //
    // > std::clamp(times_considered_, 0u, max_consider_ - 1)
    return get_transition(std::clamp(times_considered_, 0u, max_consider_ - 1));
  }

  std::shared_ptr<Transition> get_transition(unsigned times_considered) const
  {
    xbt_assert(times_considered < this->pending_transitions_.size(),
               "Actor %ld does not have a state available transition with `times_considered = %u`,\n"
               "yet one was asked for",
               aid_, times_considered);
    return this->pending_transitions_[times_considered];
  }

  void set_transition(std::shared_ptr<Transition> t, unsigned times_considered)
  {
    xbt_assert(times_considered < this->pending_transitions_.size(),
               "Actor %ld does not have a state available transition with `times_considered = %u`, "
               "yet one was attempted to be set",
               aid_, times_considered);
    this->pending_transitions_[times_considered] = std::move(t);
  }

  const std::vector<std::shared_ptr<Transition>>& get_enabled_transitions() const
  {
    return this->pending_transitions_;
  };
};

} // namespace simgrid::mc

#endif
