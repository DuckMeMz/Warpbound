#include "Counter.h"
#include "Spaceship.h"

template<typename T>
SpaceShip* ResourceCounter<T>::m_player; 

int AmmoCounter::GetCustomCurrent() { return m_player->GetCurrentAmmo(); }
int AmmoCounter::GetCustomMax() { return m_player->GetMaxAmmo(); }

Vector2D AmmoCounter::GetCurrentOffset() {return CURRENT_OFFSET;}
Vector2D AmmoCounter::GetSplitterOffset() {return SPLITTER_OFFSET;}
Vector2D AmmoCounter::GetMaxOffset() {return MAX_OFFSET;}
Vector2D AmmoCounter::GetIconOffset() {return ICON_OFFSET;}

const Vector2D AmmoCounter::CURRENT_OFFSET = Vector2D(-70, 0);
const Vector2D AmmoCounter::SPLITTER_OFFSET = Vector2D(0,0);
const Vector2D AmmoCounter::MAX_OFFSET = Vector2D(65, 0);
const Vector2D AmmoCounter::ICON_OFFSET = Vector2D(-200 , 0);

int MissileCounter::GetCustomCurrent() { return m_player->GetCurrentMissile(); }
int MissileCounter::GetCustomMax() { return m_player->GetMaxMissile(); }

Vector2D MissileCounter::GetCurrentOffset() {return CURRENT_OFFSET;}
Vector2D MissileCounter::GetSplitterOffset() {return SPLITTER_OFFSET;}
Vector2D MissileCounter::GetMaxOffset() {return MAX_OFFSET;}
Vector2D MissileCounter::GetIconOffset() {return ICON_OFFSET;}

const Vector2D MissileCounter::CURRENT_OFFSET = Vector2D(-55, 0);
const Vector2D MissileCounter::SPLITTER_OFFSET = Vector2D(0, 0);
const Vector2D MissileCounter::MAX_OFFSET = Vector2D(40, 0);
const Vector2D MissileCounter::ICON_OFFSET = Vector2D(-200, 0);