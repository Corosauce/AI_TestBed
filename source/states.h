#ifndef STATES_H
#define STATES_H
namespace states {
//void Idle();
const int AI_PRI1_STATE_COMA = 0;
//void Listen();
const int AI_PRI1_STATE_SELF = 1;
//void Protect();
const int AI_PRI1_STATE_PROTECT = 2;
//void Protect();
const int AI_PRI1_STATE_KAMIKAZE = 3;
//void Dazed();
const int AI_PRI1_STATE_DAZED = 4;

//void Wait();
const int AI_JOB_STATE_IDLE = 0;
//void Follow();
const int AI_JOB_STATE_FOLLOW = 1;
//void Waypoints();
const int AI_JOB_STATE_WAYPOINTS = 2;
//void Patrol();
const int AI_JOB_STATE_PATROL = 3;
//void Hunt();
const int AI_JOB_STATE_HUNT = 4;
//void Fight(); like dogfighting!
const int AI_JOB_STATE_FIGHT = 5;
//void Evade();
const int AI_JOB_STATE_EVADE = 6;
//void Pathfind();
const int AI_JOB_STATE_PATHFIND = 7;
//void StartAI();
const int AI_JOB_STATE_BIRTH = 8;

const int AI_JOB_DEFEND_AREA = 9;

//void Idle();
const int AI_MM_IDLE = 0;
//void MoveTo();
const int AI_MM_MOVETO = 1;
//void Chase();
const int AI_MM_CHASE = 2;
//void Attack();
const int AI_MM_ATTACK = 3;
//void Flee();
const int AI_MM_FLEE = 4; //avoid target
//void Dodge();
const int AI_MM_DODGE = 5; //avoid prj for a sec

const int AI_MM_DEAD = 0;

//hmmmmmmm
const int AI_MOOD_HOLD = 0;
const int AI_MOOD_DEFENCE = 1;
const int AI_MOOD_OFFENCE = 2;


const int PHYS_STATE_NORMAL = 0;
const int PHYS_STATE_COLLIDE_PRJ = 7;
const int PHYS_STATE_COLLIDE_UNIT = 8;
const int PHYS_STATE_COLLIDE_MAP = 9;
const int PHYS_STATE_DEAD = -1;

const int ANIM_STATE_IDLE = 0;
const int ANIM_STATE_MOVE = 1;
const int ANIM_STATE_ATTACK = 2;
const int ANIM_STATE_ATTACK2 = 3;
const int ANIM_STATE_ATTACK3 = 4;
const int ANIM_STATE_ATTACK4 = 5;
//const int ANIM_STATE_BURROW = 6;
//const int ANIM_STATE_BURROWED = 7;

const int ANIM_STATE_HIT = 9;
const int ANIM_STATE_DEATH = 10;
const int COUNT_ANIM_STATE = 11;
}
#endif