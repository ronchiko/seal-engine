#pragma once

#include <seal.h>

#define AABB2D	(Seal_AABBCollider2d *)	

typedef void (*Seal_AABBTriggerHandler)(Seal_Entity other);

typedef enum {
	SEAL_AABB_TRIGGER = Seal_Bit(0),
} Seal_AABBColliderFlags;

typedef struct {
	Seal_Component base;
	
	Seal_UByte flags;
	Seal_Rect aabb;

	Seal_AABBTriggerHandler triggerHandler;
} Seal_AABBCollider2d;

void Seal_InitAABBCollider2dBuffer(void);
Seal_ComponentBuffer Seal_GetAABBCollider2dBuffer(void);

/* 	
	Adds an AABB collider component and initialezes it to a 
	value using other components on the object if possible (Non trigger collider)
*/
Seal_AABBCollider2d *Seal_AddInitializedAABBCollider2d(Seal_Entity entity);
/* Adds an uninitialized AABB component to the entity (The AABB rect is 0,0,1,1)*/
Seal_AABBCollider2d *Seal_AddAABBCollider2d(Seal_Entity entity);

/* Sets an AABB collider2d to be a trigger collider */
Seal_Bool Seal_MakeAABBCollider2dTrigger(Seal_AABBCollider2d *collider, Seal_AABBTriggerHandler handler);

void Seal_RemoveAABBCollider2d(Seal_Entity entity);
