#pragma once 
#include <utility>
#include <stdexcept>
#include <initializer_list>

template<typename Key, typename Hash = std::hash<Key>, typename EqualComparer = std::equal_to<Key>>
class unordered_set {
	struct node {
		Key _data;
		node* _next;
		node(Key data) : _data(std::move(data)), _next(nullptr) {}
	};
	node** table;
	size_t size_table;

public:
	class iterator {
		node* _item;
		node** _table;
		size_t _size_table;
		size_t _bucket;
		iterator(node** table, size_t tsize, node* item, size_t bucket) : _table(table), _item(item), _size_table(tsize), _bucket(bucket) {}
	public:
		friend class unordered_set<Key, Hash, EqualComparer>;
		Key operator*() {
			return _item->_data;
		}
		iterator& operator++() {
			if (_bucket > _size_table)
				throw std::logic_error("You can't increment past the end");
			if (_item->_next != nullptr)
				_item = _item->_next;
			else {
				while (++_bucket <= _size_table) {
					if (_table[_bucket] != nullptr) {
						_item = _table[_bucket];
						break;
					}
				}
				_item = _table[_bucket];
			}
			return *this;
		}
		iterator& operator++(int) {
			auto copy = *this;
			*this++;
			return copy;
		}
		iterator& operator--() {
			if (_bucket == 0)
				throw std::logic_error("You can't decrement begin");
			if (_table[_bucket] == _item) {
				--_bucket;
				while (_table[_bucket] == nullptr)
					--_bucket;
				_item = _table[_bucket];
				while (_item->_next != nullptr)
					_item = _item->_next;
			}
			else {
				auto temp = _item;
				_item = _table[_bucket];
				while (_item->_next != temp)
					_item = _item->_next;
			}
			return *this;
		}
		iterator& operator--(int) {
			auto copy = *this;
			--this;
			return copy;
		}
		bool operator==(const iterator& rhs) {
			if (_item == rhs._item && _table == rhs._table && _size_table == rhs._size_table && _bucket == rhs._bucket)
				return true;
			else
				return false;
		}
		bool operator!=(const iterator& rhs) {
			return !(*this == rhs);
		}
	};
	unordered_set(size_t tsize = 0) : size_table(tsize), table(new node* [tsize + 1]) {
		for (size_t i = 0; i <= size_table; ++i) {
			table[i] = nullptr;
		}
	}
	unordered_set(std::initializer_list<Key> v):size_table(v.size()), table(new node* [v.size() + 1]) {
		for (size_t i = 0; i <= size_table; ++i) {
			table[i] = nullptr;
		}
		for (auto i : v)
			insert(i);
		
	}
	unordered_set(const unordered_set& rhs) {
		unordered_set tmp(rhs.size_table);
		for (auto i : rhs) {
			tmp.insert(i);
		}
		std::swap(table, tmp.table);
		std::swap(size_table, tmp.size_table);
	}
	iterator insert(const Key& value) {
		auto hash = Hash()(value) % (size_table);
		auto insert_place = &table[hash];
		if (*insert_place == nullptr) {
			*insert_place = new node(value);
		}
		else {
			if ((*insert_place)->_data == value)
				return iterator(table, size_table, *insert_place, hash);
			while ((*insert_place)->_next != nullptr) {
				if ((*insert_place)->_next->_data == value)
					return iterator(table, size_table, (*insert_place)->_next, hash);
				insert_place = &((*insert_place)->_next);
			}
			(*insert_place)->_next = new node(value);
			insert_place = &((*insert_place)->_next);

		}
		return iterator(table, size_table, *insert_place, hash);

	}
	iterator erase(iterator pos) {
		auto bucket = pos._bucket;
		auto item = pos._item;
		auto pos_after = pos;
		++pos_after;
		//if (item->_next = nullptr) {
		//	table[bucket] = nullptr;
		//	delete item;
		//	//return first find elem
		//}
		//else {
		if (item == table[bucket]) {
			table[bucket] = item->_next;
		}
		else {
			auto prev_item = table[bucket];
			while (prev_item->_next != item)
				prev_item = prev_item->_next;
			prev_item->_next = item->_next;
		}
		delete item;
		//}
		return pos_after;
	}
	iterator find(const Key& value) {
		auto hash = Hash()(value) % size_table;
		auto item = table[hash];
		while (item != nullptr) {
			if (EqualComparer()(item->_data, value))
				return iterator(table, size_table, item, hash);
			item = item->_next;
		}
		return iterator(table, size_table, table[size_table + 1], size_table + 1);
	}
	iterator begin() {
		for (auto i = 0; i < size_table; ++i) {
			if (table[i] != nullptr)
				return iterator(table, size_table, table[i], i);
		}
		return iterator(table, size_table, table[size_table + 1], size_table + 1);
	}
	iterator end() {
		return iterator(table, size_table, table[size_table + 1], size_table + 1);
	}
	void swap(unordered_set& rhs) {
		std::swap(table, rhs.table);
		std::swap(size_table, rhs.size_table);
	}
	void clear() {
		for (size_t i = 0; i <= size_table; ++i) {
			while (table[i] != nullptr) {
				auto tmp = table[i];
				table[i] = table[i]->_next;
				delete tmp;
			}
		}
		delete[] table;
		size_table = 0;
		table = new node*;
	}
	~unordered_set() {
		clear();
		delete table;
	} 

};