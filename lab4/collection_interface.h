#pragma once
#include <vector>
#include <list>

template <typename T>
class ICollection {
public:
	class iterator {
		friend class  ICollection<T>;
	public:
		virtual T& operator*() = 0;
		virtual const T& operator*() const = 0;
		virtual iterator& operator++() = 0;
		virtual bool operator==(const iterator& rhs) const = 0;
		virtual bool operator!=(const iterator& rhs) const = 0;
	};;
	virtual iterator& insert(iterator& it, const T& value) = 0;
	virtual iterator& begin() = 0;
	virtual iterator& end() = 0;
	virtual ~ICollection() = default;
};

template <typename T>
class vector : public ICollection<T> {
	std::vector<T> v;
	


public:
	class VectorIterator : public  ICollection<T>::iterator{
		using raw_iterator = typename std::vector<T>::iterator;
		raw_iterator  it;
		VectorIterator( raw_iterator& it) : it(it) {}

		friend class vector<T>;
		raw_iterator get_row_it() {
			return it;
		}
	public:
		T& operator*() {
			return *it;
		}
		const T& operator*() const {
			return *it;
		}
		VectorIterator& operator++() {
			++it;
			return *this;
		}
		bool operator==(const iterator& rhs) const {
		//	auto un_c_rhs = const_cast<iterator&>(rhs);
			auto vector_it = dynamic_cast<VectorIterator&>(const_cast<iterator&>(rhs));
			return it == vector_it.get_row_it();
		}
		bool operator!=(const iterator& rhs) const  {
			//auto un_c_rhs = ;
			auto vector_it =dynamic_cast<VectorIterator&>(const_cast<iterator&>(rhs));
			return it != vector_it.get_row_it();
		}
	};
	vector() : v(std::vector<T>()) {}
	iterator& insert(iterator& it, const T& value) {
		auto vector_it = dynamic_cast<VectorIterator&>(it);
		return VectorIterator(v.insert(vector_it.get_row_it() , value));
	}
	iterator& begin() {
		return VectorIterator(v.begin());
	}
	iterator& end() {
		return VectorIterator(v.end());
	}
};

template <typename T>
class list : public ICollection<T> {
	std::list<T> l;
public:
	list() {}
	class ListIterator : public ICollection::iterator {
		using raw_iterator = typename std::list<T>::iterator;
		raw_iterator it;
		friend class list<T>;
		ListIterator(const raw_iterator& it) :it(it) {}
		raw_iterator get_row_it() {
			return it;
		}
	public:
		T& operator*() {
			return *it;
		}
		const T& operator*() const {
			return *it;
		}
		ListIterator& operator++() {
			++it;
			return *this;
		}
		bool operator==(const iterator& rhs)const {
			return it == rhs;
		}
		bool operator!=(const iterator& rhs)const {
			return it != rhs;
		}
	};
	iterator& insert(iterator& it, const T& value) {
		auto list_it = dynamic_cast<ListIterator>(it);
		return l.insert(list_it.get_row_it(), value);
	}
	iterator& begin() {
		return ListIterator(v.begin());
	}
	iterator& end() {
		return ListIterator(v.end());
	}
};

