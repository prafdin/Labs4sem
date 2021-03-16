#pragma once 
#include <algorithm>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <stack>
#include <utility>

template<typename Key, typename Compare = std::less<Key>>
class set {
	struct node {
		const Key key;
		uint8_t height;
		node* left;
		node* right;
		node* parent;

		node(node* parent, const int key) : key(key), height(1), parent(parent), left(nullptr), right(nullptr) { }
		node() : key(Key()), parent(nullptr), left(nullptr), right(nullptr) {}
		node(const node&) = delete;

		node& operator=(const node&) = delete;
		bool operator==(const node& rhs) {
			return key == rhs.key;
		}
		bool operator!=(const node& rhs) {
			return key != rhs.key;
		}
	};
	using pair = std::pair<node*, bool>; //if flag true , we del node in right subtree 
	node* _root;
	node* _end;
	size_t _size;

	bool less(const Key& lhs, node* rhs) {
		if (rhs == _end)
			return true;
		return Compare()(lhs, rhs->key);
	}

	bool less(node* lhs, const Key& rhs) {
		if (lhs == _end)
			return false;
		return Compare()(lhs->key, rhs);
	}

	static void _clear(node* const root) {
		if (root == nullptr) {
			return;
		}
		_clear(root->left);
		_clear(root->right);
		delete root;
	}

	static void debug(const node* const root, const size_t level = 0) {
		static const size_t OFFSET_SIZE = 2;
		const std::string offset(level * OFFSET_SIZE, ' ');
		if (root == nullptr) {
			printf("%sNULL\n", offset.c_str());
			return;
		}
		printf("%s%d\n", offset.c_str(), root->key);
		if (root->left || root->right) {
			debug(root->left, level + 1);
			debug(root->right, level + 1);
		}
	}

	uint8_t height(node* p) {
		if (p == nullptr) {
			return 0;
		}
		return p->height;
	}

	void update_height(node* const p) {
		p->height = std::max(height(p->left), height(p->right)) + 1;
	}

	node* rotate_right(node* const p) {
		auto q = p->left;
		q->parent = p->parent;
		p->parent = q;
		p->left = q->right;
		if (q->right != nullptr)
			q->right->parent = p;
		q->right = p;
		update_height(p);
		update_height(q);
		return q;
	}

	node* rotate_left(node* const q) {
		auto p = q->right;
		p->parent = q->parent;
		q->parent = p;
		q->right = p->left;
		if (p->left != nullptr)
			p->left->parent = q;
		p->left = q;
		update_height(q);
		update_height(p);
		return p;
	}

	node* balance(node* p) {
		update_height(p);
		auto dh = height(p->left) - height(p->right);
		if (dh == 2) {
			// if h(s) > h(A)
			if (height(p->left->right) > height(p->left->left)) {
				p->left = rotate_left(p->left);
			}
			p = rotate_right(p);
		}
		else if (dh == -2) {
			// !!!
			if (height(p->right->left) > height(p->right->right)) {
				p->right = rotate_right(p->right);
			}
			p = rotate_left(p);
		}
		return p;
	}

	node* _find(const Key& key) {
		auto item = _root;
		while (item.key != key) {
			if (less(key, item))
				item = item->left;
			else
				item = item->right;
		}
		if (item == _root && item.key != key)
			return nullptr;
		else
			return item;
	}

	node* delete_min_in_right_subtree(node** root) {
		std::stack<pair> s;
		auto place = *root;
		if (place->left == nullptr && place->right == nullptr) {
			place->parent->right = nullptr;
			return place;
		}
		while (place->left != nullptr) {
			s.push({ place ,false });
			place = place->left;
		}
		auto right_branch = place->right;
		if (right_branch != nullptr) {
			right_branch->parent = place->parent;
			place->parent->left = right_branch;
		}
		else
			place->parent->left = nullptr;
		while (s.size() != 0) {
			auto balanced = s.top();
			s.pop();
			if (s.size() == 0) { //balanced == root
				*root = balance(balanced);
				return place;
			}
			auto parent_balanced = s.top();
			parent_balanced->left = balance(balanced);
		}
		return place;
	}

	void balance_stack(std::stack<pair> s) {
		while (s.size() != 0) {
			auto balanced = s.top();
			s.pop();
			if (s.size() == 0) { //balanced == root
				_root = balance(balanced.first);
				break;
			}
			auto parent_balanced = s.top();
			if (parent_balanced.second == true) //if balanced right child
				parent_balanced.first->right = balance(balanced.first);
			else
				parent_balanced.first->left = balance(balanced.first);
		}
	}
public:
	class iterator {
		node* item;
		iterator(node* iter) : item(iter) {}
	public:
		friend class set<Key, Compare>;
		iterator& operator++() {
			if (item->right == nullptr) {
				while (item == item->parent->right)
					item = item->parent;
				item = item->parent;
			}
			else {
				item = item->right;
				while (item->left != nullptr)
					item = item->left;

			}
			return *this;
		}
		iterator& operator--() {
			if (item->left == nullptr) {
				while (item == item->parent->left)
					item = item->parent;
				item = item->parent;
			}
			else {
				item = item->left;
				while (item->right != nullptr)
					item = item->right;

			}
			return *this;
		}
		bool operator==(const iterator& rhs) {
			return item == rhs.item;
		}
		bool operator!=(const iterator& rhs) {
			return !(*this == rhs);
		}
		const Key& operator*() {
			return item->key;
		}
	};
	set() : _root(nullptr), _size(0), _end(new node()) {
		_root = _end;
	}
	set(const set& rhs):_root(nullptr), _size(0), _end(new node()) {
		_root = _end;
		for ( auto i : rhs)
			insert(i);
	}
	set& operator=(const set& rhs) {
		_size = 0;
		_clear(_root);
		_root = _end = new node();
		for (auto& i : rhs)
			this->insert(i);
		return *this;
	}
	void swap(set& rhs) {
		std::swap(_root, rhs._root);
		std::swap(_end, rhs._end);
		std::swap(_size, rhs._size)
	}
	iterator insert(const Key& key) {
		std::stack<pair> s;
		auto place = _root;
		while (place != nullptr) {
			if (less(key, place)) {
				s.push(pair(place, false));
				place = place->left;
			}
			else if (less(place, key)) {
				s.push(pair(place, true));
				place = place->right;
			}
			else
				return iterator(place);
		}
		if (_root == nullptr)
			_root = new node(nullptr, key);
		else {
			auto parent = s.top();
			if (parent.second == true)  // if inserted item right child
				place = parent.first->right = new node(parent.first, key);

			else // if inserted item left child
				place = parent.first->left = new node(parent.first, key);
		}
		balance_stack(s);
		++_size;
		return iterator(place);
	}
	iterator erase(const Key& key) {
		std::stack<pair> s;
		auto place = _root;
		node* next_item = nullptr;
		while (place->key != key) {
			if (less(key, place)) {
				s.push(pair(place, false));
				place = place->left;
			}
			else if (less(place, key)) {
				s.push(pair(place, true));
				place = place->right;
			}
		}

		if (place == _root && place->key != key)
			throw std::logic_error("Key isn't found");

		if (place->right != nullptr) {
			auto min = delete_min_in_right_subtree(&(place->right));
			min->left = place->left;
			min->right = place->right;
			if (place->right != nullptr)
				place->right->parent = min;
			min->parent = place->parent;
			place->left->parent = min;
			next_item = min;
		}
		else {
			if (place->left != nullptr)
				place->left->parent = place->parent;
			place->parent->left = place->left;
			next_item = place->parent;
		}
		delete place;
		s.push({ next_item,false });
		balance_stack(s);
		--_size;
		return iterator(next_item);
	}
	iterator begin () const {
		auto tmp = _root;
		while (tmp->left != nullptr) {
			tmp = tmp->left;
		}
		return iterator(tmp);
	}
	iterator end () const {
		return iterator(_end);
	}
	size_t size() {

		return _size;
	}
	void clear() {
		_clear(_root);
	}
	iterator find(const Key& key) {
		auto item = _find(key);
		if (item == nullptr)
			return end();
		else
			return iterator(item);
	}
	~set() {
		clear();
	}
};
