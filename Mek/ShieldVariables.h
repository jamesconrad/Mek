#pragma once
#ifndef SHIELDVARIABLES_H
#define SHIELDVARIABLES_H

float shieldHealth = 20.f;
float maxShieldHealth = 100.f;
float shieldRechargeTimer = 0.f;
float shieldMaxTimer = 3.5f;
float shieldRechargeAmount = 5.f;

bool  playerIsHit = false;
float hitInvulnTimer = 0.0f;
float maxInvulnTime = 0.5f;

#endif // !SHIELDVARIABLES_H