#ifndef HSPARSEIDLIST_H
#define HSPARSEIDLIST_H

#include <vector>
#include <functional>

#include "HGeneral.h"

namespace holodec {

	class HIdGenerator {
	private:
		HId val = 1;
	public:
		HId next() {
			return val++;
		}
		void clear() {
			val = 1;
		}
	};

/**
 * Needs id member and relabel function
 * 
 */
	template<typename T>
	struct HIdList {
		HIdGenerator gen;
		HList<T> list;
		
		typedef typename HList<T>::iterator iterator;

		HIdList() {}
		HIdList (std::initializer_list<T> list) : list (list) {
			relabel();
		}
		HIdList (HList<T> list) : list (list) {
			relabel();
		}
		
		HId push_back (T& ele) {
			ele.id = gen.next();
			list.push_back (ele);
			return ele.id;
		}
		void relabel (std::function<void (HId, HId) > replacer = nullptr) {
			for (T& ele : list) {
				ele.relabel(&gen,replacer);
			}
		}
		auto begin() -> decltype(list.begin()){
			return list.begin();
		}
		auto end() -> decltype(list.end()){
			return list.end();
		}
		auto size() -> decltype(list.size()){
			return list.size();
		}
		auto back() -> decltype(list.back()){
			return list.back();
		}
		void erase(iterator it){
			list.erase(it);
		}
		T* get (HId id) {
			if(!id)
				return nullptr;
			size_t lowerbound = 0, upperbound = list.size();
			if (upperbound >= id) {//optimization for quick access
				T& val = list[id - 1];
				if (val.id == id) //quick return
					return &list[id - 1];
			}
			
			while(lowerbound <= upperbound) {// binary seach
				int middle = lowerbound + ((upperbound - lowerbound) / 2);
				HId middleId = list[middle].id;
				if(middleId == id)
					return &(list[middle]);
				if(middleId < id)
					lowerbound = middle + 1;
				else
					upperbound = middle - 1;
			}
			return nullptr;
		}
		T& operator[] (HId id) {
			return *get(id);
		}
		void clear() {
			gen.clear();
			list.clear();
		}

	};
	
	
	template<typename T>
	class HSparseIdList {
		typedef typename std::vector<T>::iterator iterator;
		typedef typename std::vector<T>::const_iterator const_iterator;
		
		HList<T> list;
		
		void label(){
			for(size_t i = 0; i < list.size();++i){
				list[i].id = i + 1;
			}
		}
	public:
		HSparseIdList(){}
		~HSparseIdList(){}
		HSparseIdList (const std::initializer_list<T>& list) : list (list) {
			label();
			shrink();
		}
		HSparseIdList (const HSparseIdList<T>& list) : list (list.list) {
			label();
			shrink();
		}
		HSparseIdList (const HList<T>& list) : list (list) {
			label();
			shrink();
		}

		HId push_back (T& ele) {
			ele.id = list.size() + 1;
			list.push_back (ele);
			return ele.id;
		}
		void shrink (std::function<void (HId, HId)> replacer = nullptr) {
			auto fit = list.begin();
			auto rit = list.end() - 1;
			int erased = 0;
			while(fit != rit){
				while(fit->id && fit < rit) ++fit;
				while(!rit->id && fit < rit) --rit;
				if(fit == rit) break;
				
				HId id = std::distance(list.begin(), rit) + 1;
				HId newId = std::distance(list.begin(), fit) + 1;
				if(replacer)
					replacer(id, newId);
				*fit = *rit;
				fit->id = newId;
				rit->id = 0;
			}
			if(fit->id)
				++fit;
			list.erase(fit, list.end());
		}
		auto begin() -> decltype(list.begin()){
			return list.begin();
		}
		auto end() -> decltype(list.end()){
			return list.end();
		}
		auto size() -> decltype(list.size()){
			return list.size();
		}
		auto back() -> decltype(list.back()){
			return list.back();
		}
		void erase(iterator it){
			it->id = 0;
			++it;
		}
		T* get (HId id) {
			return &list[id-1];
		}
		T& operator[] (HId id) {
			return list[id-1];
		}
		void clear() {
			list.clear();
		}
	};

}

#endif // HSPARSEIDLIST_H
