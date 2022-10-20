#pragma once

#ifdef __cplusplus

#include <seal/ecs/component.h>

#include <seal/cpp/error.hpp>

#include <algorithm>
#include <type_traits>
#include <cstring>

namespace seal {

struct component {
public:
	Seal_Component base;
};

template<typename T, typename = std::enable_if_t<std::is_default_constructible_v<T>>>
class component_buffer final {
public:
	class iterator : public std::iterator<std::forward_iterator_tag, T> {
	public:
		explicit iterator(size_t index) 
			: m_index(index)
			, m_current()
		{}

		iterator& operator++() {
			Seal_GetComponent(m_buffer_index, m_index++, &(&m_current));
			return *this;
		}

		iterator operator++(int) {
			iterator copy = *this;
			Seal_GetComponent(m_buffer_index, m_index++, &(&m_current));
			return copy;
		}

		bool operator==(iterator& other) const {
			return m_index == other.m_index;
		}

		bool operator!=(iterator& other) const {
			return m_index != other.m_index;
		}

		T& operator*() const {
			return m_current;
		}

	private:
		Seal_Size m_index;
		T m_current;
	};

	T& push_new() {
		char buffer[sizeof(T)];
		std::memset(buffer, 0, sizeof(T));

		T* comp = reinterpret_cast<T*>(Seal_AddComponent(m_buffer_index, reinterpret_cast<Seal_Component*>(&buffer)));
		if(nullptr == comp) {
			throw seal::error("Failed to create new component");
		}

		return *comp;
	}

	static component_buffer& instance() {
		static component_buffer* instance = nullptr;

		if(nullptr == instance) {
			instance = new component_buffer();
		}

		return *instance;
	}

	Seal_ComponentBuffer id() const {
		return m_buffer_index;
	}

	void remove(Seal_Entity entity) {
		Seal_RemoveComponent(m_buffer_index, entity);
	}

	iterator begin() const {
		return iterator{0};
	}

	iterator end() const {
		Seal_Int size = Seal_GetComponentBufferSize(m_buffer_index);
		if(size == -1) {
			throw seal::error("Invalid buffer");
		}

		return iterator{size + 1};
	}

	T& find_component_for_entity(Seal_Entity entity) {
		T* comp = reinterpret_cast<T*>(Seal_FindComponentForEntity(m_buffer_index, entity));
		if(nullptr == comp) {
			throw seal::error("Failed to find component");
		}
		return *comp;
	}

private:
	component_buffer() 
		: m_buffer_index(Seal_CreateComponentBuffer(sizeof(T)))
	{}

	Seal_ComponentBuffer m_buffer_index; 
};

template<typename T>
constexpr bool is_c_component() {	
	if constexpr(offsetof(T, base) == 0 && std::is_same<decltype(T::base), Seal_Component>::value) {
		return true;
	} else {
		return false;
	}
} 

template<typename T>
constexpr bool is_component_v = is_c_component<T>() || std::is_base_of_v<component, T>;


class entity final {
public:
	entity(Seal_Entity entity_id);

	/**
	 * @brief Makes a new entity
	 */
	static entity make_new();

	/**
	 * @brief Destroys this entity and its components
	 */
	void destroy();

	/**
	 * @brief Destroys this entity and its components immetialy
	 * 
	 */
	void purge();

	/**
	 * @brief Gets a component from this entity
	 * 
	 * @tparam T: The type of component to get 
	 */
	template<typename T, typename = std::enable_if_t<is_component_v<T>>>
	T& get() {
		if (m_id == SEAL_INVALID_ENTITY) {
			throw seal::error("Cannot get component from an invalid entity");
		}

		T& comp = component_buffer<T>::instance().find_component_for_entity(m_id);
		return comp;
	}

	/**
	 * @brief Attachs a new component to this entity
	 * 
	 * @tparam T: The type of component to attach
	 */
	template<typename T, typename = std::enable_if_t<is_component_v<T>>>
	T& attach() {
		if (m_id == SEAL_INVALID_ENTITY) {
			throw seal::error("Cannot attach component to an invalid entity");
		}

		T& comp = component_buffer<T>::instance().push_new();
		Seal_AttachComponent(m_id, reinterpret_cast<Seal_Component*>(&comp));
		return comp;
	}

	/**
	 * @brief Detaches a component to this entity
	 * 
	 * @tparam T: The type of component to dettach
	 */
	template<typename T, typename = std::enable_if_t<is_component_v<T>>>
	void detach() {
		if (m_id == SEAL_INVALID_ENTITY) {
			throw seal::error("Cannot detach component from an invalid entity");
		}

		component_buffer<T>::instance().remove(m_id);
	}


private:
	Seal_Entity m_id;
};
};

#endif