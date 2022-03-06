
#include "../common.h"

void RectSetAABB(Seal_RawCollider2d *c) {
	c->aabb = c->collider.rect.rect;
}