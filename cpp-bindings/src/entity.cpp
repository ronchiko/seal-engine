#include "seal/cpp/component.hpp"

using namespace seal;

entity::entity(Seal_Entity id)
	: m_id(id) 
{
}

entity entity::make_new() {
	return entity(Seal_CreateEntity());
}

void entity::destroy() {
	if (m_id == SEAL_ID_INVALID) {
		throw seal::error("Cannot destroy an invalid entity!");
	}
	
	Seal_DestroyEntity(m_id);
	m_id = SEAL_INVALID_ENTITY;
}

void entity::purge() {
	if(m_id == SEAL_ID_INVALID) {
		throw seal::error("Cannot purge an invalid entity!");
	}

	Seal_PurgeEntity(m_id);
	m_id = SEAL_INVALID_ENTITY;
}