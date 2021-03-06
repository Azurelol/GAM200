/*****************************************************************************/
/*!
@file      Action.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/7/2016
@brief     An action...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Action.h"

#include "Engine.h"

namespace DCEngine {

  /*=======================*
  *     ActionSet          *
  *========================*/
  /**************************************************************************/
  /*!
  @brief  Clears all inactive actions.
  */
  /**************************************************************************/
  void ActionSet::Clear()
  {
    if (InactiveActions.empty())
      return;

    // Remove the action from the active actions!
    ActiveActions.erase(
      std::remove_if(ActiveActions.begin(), ActiveActions.end(),
        [&](ActionPtr& a) {
      return std::find(
        InactiveActions.cbegin(), 
        InactiveActions.cend(), 
        a)
        != InactiveActions.end(); }), 
      ActiveActions.end());
    // Now clear!
    if (DCE_TRACE_ACTIONS_REMOVE) {
      DCTrace << Type << "::Clear - Removed '" << InactiveActions.size() << "' actions: \n";
      for (auto& action : InactiveActions) {
        DCTrace << " - " << action->Type << "\n";
        delete action; 
      }
    }
      
    InactiveActions.clear();
  }

  /**************************************************************************/
  /*!
  @brief  Adds an ActionSet to this set, as a child set.
  @param set A reference to the child set.
  */
  /**************************************************************************/
 void ActionSet::Add(ActionSetPtr set)
 {
   ActiveActions.push_back(set);
 }
 
 /**************************************************************************/
 /*!
 @brief  Adds an Action to this set, as one of its active actions.
 @param action A pointer to the other action.
 */
 /**************************************************************************/
 void ActionSet::Add(ActionPtr action)
 {
   ActiveActions.push_back(action);
 }

  /**************************************************************************/
  /*!
  @brief  Checks whether there are any remaining actions in the ActionSet.
          If there's no more remaining, the set is done.
  */
  /**************************************************************************/
  bool ActionSet::Validate()
  {
    if (ActiveActions.empty())
      IsFinished = true;
    return IsFinished;
  }

  /*=======================*
  *     ActionSequence     *
  *========================*/
  /**************************************************************************/
  /*!
  @brief Updates an ActionSequence, by updating the actions in the sequence
         sequentially.
  @param dt The time to be updated.
  @return How much time was consumed while updating.      
  */
  /**************************************************************************/
  float ActionSequence::Update(float dt)
  {
    auto timeLeft = dt;
    for (auto& action : ActiveActions) {
      // Every action consume time in the time slice given.
      timeLeft -= action->Update(timeLeft);      
      // If the action was completed (Meaning there is time remaining after it was updated)
      // , then mark the action to be cleared.
      if (timeLeft >= 0) {
        InactiveActions.push_back(action);
      }
      // If we have run out of time, break out of the loop!
      if (timeLeft <= 0)
        break;
    }
    // Sweep all inactive actions
    Clear();
    // Validate for remaining actions
    Validate();
    // The time was consumed while updating...
    return dt - timeLeft;
  }

  /*=======================*
  *     ActionGroup     *
  *========================*/
  /**************************************************************************/
  /*!
  @brief Updates an ActionGroup, by updating the actions in the group in
         parallel.
  @param dt The time to be updated.
  @return How much time was consumed while updating.
  */
  /**************************************************************************/
  float ActionGroup::Update(float dt)
  {
    float mostTimeElapsed = 0;
    // In an ActionGroup, every action is updated in parallel, given the same 
    // time slice.
    for (auto& action : ActiveActions) {
      auto timeElapsed = action->Update(dt);
      // If this action took longer than the previous action, it is the new maximum
      if (timeElapsed > mostTimeElapsed)
        mostTimeElapsed = timeElapsed;
      // If the action was completed (Meaning that it was completed in less time
      // than the time slice given)
      if (timeElapsed <= dt) {
        // Mark the action to be cleared
        InactiveActions.push_back(action);
      }
    }

    // Sweep all inactive actions
    Clear();    
    // Validate for remaining actions
    Validate();
    // The time consumed while updating was the maximum time it took
    return mostTimeElapsed;
  }








}