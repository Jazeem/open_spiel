// Copyright 2019 DeepMind Technologies Limited
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "open_spiel/games/kalah.h"
#include "open_spiel/spiel.h"
#include "open_spiel/tests/basic_tests.h"

namespace open_spiel {
namespace kalah {
namespace {

namespace testing = open_spiel::testing;

void BasicSerializationTest() {
  std::shared_ptr<const Game> game = LoadGame("kalah");
  std::unique_ptr<State> state = game->NewInitialState();
  std::unique_ptr<State> state2 = game->DeserializeState(state->Serialize());
  SPIEL_CHECK_EQ(state->ToString(), state2->ToString());
}

void BasicKalahTests() {
  testing::LoadGameTest("kalah");
  testing::NoChanceOutcomesTest(*LoadGame("kalah"));
  testing::RandomSimTest(*LoadGame("kalah"), 100);
}

// Board:
// -0-0-0-4-0-0-
// 0-----------0
// -0-0-1-0-0-0-
// Player 0 taking action 3 should capture the opponents 4 beans
void CaptureWhenOppositeHouseNotEmptyTest() {
  std::shared_ptr<const Game> game = LoadGame("kalah");
  std::unique_ptr<State> state = game->NewInitialState();
  KalahState* mstate = static_cast<KalahState*>(state.get());
  mstate->SetBoard({0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0});

  // Check for exactly one legal move.
  std::vector<Action> legal_actions = mstate->LegalActions();
  SPIEL_CHECK_EQ(legal_actions.size(), 1);

  // Check that it's 3
  SPIEL_CHECK_EQ(legal_actions[0], 3);
  
  mstate->ApplyAction(legal_actions[0]);
  // Check if Player 0 store has 5 beans
  SPIEL_CHECK_EQ(mstate->BoardAt(7), 5);
}

// Board:
// -0-0-0-0-4-0-
// 0-----------0
// -0-0-1-0-0-0-
// Player 0 taking action 3 should not result in any captures
void DoNotCaptureWhenOppositeHouseIsEmptyTest() {
  std::shared_ptr<const Game> game = LoadGame("kalah");
  std::unique_ptr<State> state = game->NewInitialState();
  KalahState* mstate = static_cast<KalahState*>(state.get());
  mstate->SetBoard({0, 0, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0});

  // Check for exactly one legal move.
  std::vector<Action> legal_actions = mstate->LegalActions();
  SPIEL_CHECK_EQ(legal_actions.size(), 1);

  // Check that it's 3
  SPIEL_CHECK_EQ(legal_actions[0], 3);
  
  mstate->ApplyAction(legal_actions[0]);
  // Check if no capture has taken place
  SPIEL_CHECK_EQ(mstate->BoardAt(7), 0);
  SPIEL_CHECK_EQ(mstate->BoardAt(3), 0);
  SPIEL_CHECK_EQ(mstate->BoardAt(4), 1);
  SPIEL_CHECK_EQ(mstate->BoardAt(9), 4);
}

// Board:
// -0-0-0-0-0-1-
// 0-----------0
// -1-0-0-0-0-8-
// Player 0 taking action 6 should not put seeds in opponents store
void DoNotAddSeedToOpponentsStoreTest() {
  std::shared_ptr<const Game> game = LoadGame("kalah");
  std::unique_ptr<State> state = game->NewInitialState();
  KalahState* mstate = static_cast<KalahState*>(state.get());
  mstate->SetBoard({0, 1, 0, 0, 0, 0, 8, 0, 1, 0, 0, 0, 0, 0});

  // Check for exactly two legal move.
  std::vector<Action> legal_actions = mstate->LegalActions();
  SPIEL_CHECK_EQ(legal_actions.size(), 2);

  // Check that it's 1 & 6
  SPIEL_CHECK_EQ(legal_actions[0], 1);
  SPIEL_CHECK_EQ(legal_actions[1], 6);
  
  mstate->ApplyAction(legal_actions[1]);
  // Check if no bean is put into opponents store
  SPIEL_CHECK_EQ(mstate->BoardAt(0), 0);
  SPIEL_CHECK_EQ(mstate->BoardAt(7), 1);
  SPIEL_CHECK_EQ(mstate->BoardAt(8), 2);
  SPIEL_CHECK_EQ(mstate->BoardAt(1), 2);
}

}  // namespace
}  // namespace kalah
}  // namespace open_spiel

int main(int argc, char** argv) {
  open_spiel::kalah::BasicSerializationTest();
  open_spiel::kalah::BasicKalahTests();
  open_spiel::kalah::CaptureWhenOppositeHouseNotEmptyTest();
  open_spiel::kalah::DoNotCaptureWhenOppositeHouseIsEmptyTest();
  open_spiel::kalah::DoNotAddSeedToOpponentsStoreTest();
}
