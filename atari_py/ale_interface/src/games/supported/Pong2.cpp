/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "Pong2.hpp"

#include "../RomUtils.hpp"


Pong2Settings::Pong2Settings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* Pong2Settings::clone() const { 
    
    RomSettings* rval = new Pong2Settings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void Pong2Settings::step(const System& system) {

    // update the reward
    int x = readRam(&system, 13); // player L (B) score
    int y = readRam(&system, 14); // player R (A) score
    reward_t score_l = x - y;
    reward_t score_r = y - x;
    m_reward_l = score_l - m_score_l;
    m_reward_r = score_r - m_score_r;
    m_score_l = score_l;
    m_score_r = score_r;

    // update terminal status
    // (game over when a player reaches 21)
    m_terminal = x == 21 || y == 21;
}


/* is end of game */
bool Pong2Settings::isTerminal() const {

    return m_terminal;
}


/* get the most recently observed reward */
reward_t Pong2Settings::getReward() const { 

    return m_reward_r;
}


/* get the most recently observed reward */
reward_t Pong2Settings::getRewardB() const { 

    return m_reward_l;
}


/* is an action part of the minimal set? */
bool Pong2Settings::isMinimal(const Action &a) const {

    switch (a) {
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        case PLAYER_A_RIGHTFIRE:
        case PLAYER_A_LEFTFIRE:
            return true;
        default:
            return false;
    }   
}


/* is an action part of the minimal set? */
bool Pong2Settings::isMinimalB(const Action &a) const {

    switch (a) {
        case PLAYER_B_NOOP:
        case PLAYER_B_FIRE:
        case PLAYER_B_RIGHT:
        case PLAYER_B_LEFT:
        case PLAYER_B_RIGHTFIRE:
        case PLAYER_B_LEFTFIRE:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void Pong2Settings::reset() {
    
    m_reward_l = 0;
    m_reward_r = 0;
    m_score_l  = 0;
    m_score_r  = 0;
    m_terminal = false;
}

        
/* saves the state of the rom settings */
void Pong2Settings::saveState(Serializer & ser) {
  ser.putInt(m_reward_l);
  ser.putInt(m_reward_r);
  ser.putInt(m_score_l);
  ser.putInt(m_score_r);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void Pong2Settings::loadState(Deserializer & ser) {
  m_reward_l = ser.getInt();
  m_reward_r = ser.getInt();
  m_score_l = ser.getInt();
  m_score_r = ser.getInt();
  m_terminal = ser.getBool();
}

ActionVect Pong2Settings::getStartingActions() {
  ActionVect startingActions;
  startingActions.push_back(SELECT);
  startingActions.push_back(PLAYER_A_NOOP);
  startingActions.push_back(SELECT);
  startingActions.push_back(RESET);
  return startingActions;
}
