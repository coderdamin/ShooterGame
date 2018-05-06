// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"

#define FLOAT_EQUL(val1, val2)		\
		(((val1) - (val2) < 1e-6) && ((val1) - (val2) > -1e-6))

#define WEAPON_MAX_COUNT 2

#define COLLISION_WEAPON ECC_EngineTraceChannel1
#define COLLISION_PROJECTILE ECC_EngineTraceChannel2

