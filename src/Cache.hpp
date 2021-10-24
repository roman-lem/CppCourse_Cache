#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <list>
#include <cmath>
#include <gtest/gtest.h>

template <typename key_T>
struct Node{
	key_T key;
	int affiliation;
};

template <typename node_T>
struct Queue{
private:
	int capacity;
public:
	int size;
	std::list<node_T> list;
	using ListIt = typename std::list<node_T>::iterator;

	Queue(size_t q_capacity) : capacity(q_capacity) {size = 0;}
	~Queue() {}

	size_t GetCapacity() { return capacity;}
	bool IsQueueFull() { return size == capacity;}

	int add(const node_T &node);
	node_T* get();

	int MoveUpfront(ListIt node);
};

template <typename cache_T>
struct Cache{
private:

	int c_capacity;

public:

	using List_Iter = typename std::list<Node<cache_T>>::iterator;
	using Map_Iter = typename std::unordered_map<cache_T, List_Iter>::iterator;

	Queue<Node<cache_T>> In;
	Queue<Node<cache_T>> Out;
	Queue<Node<cache_T>> Hot;

	std::unordered_map<cache_T, List_Iter> Map;

	Cache(size_t cache_size) :
		c_capacity(cache_size), 
		In(((size_t) (trunc(cache_size * 0.2) >= 1)? trunc(cache_size * 0.2) : 1)),
		Out(((size_t) (trunc(cache_size * 0.2) >= 1)? trunc(cache_size * 0.2) : 1)),
		Hot((size_t) (c_capacity - In.GetCapacity() - Out.GetCapacity() >= 1)? c_capacity - In.GetCapacity() - Out.GetCapacity() : 1), 
		Map() {};
	
	~Cache() {}

	int RmPage(Queue<Node<cache_T>> *queue, Map_Iter mpit);
	int MovePage(Queue<Node<cache_T>> *q_src, Queue<Node<cache_T>> *q_dst, List_Iter lstit);

	Map_Iter CacheHit(cache_T page) {
		return Map.find(page);
	}

	int add(cache_T page);

};


//====/ Queue functioins /====
template <typename node_T>
int Queue<node_T>::add(const node_T &node){
	if(size < capacity){
		list.push_front(node);
		size++;
		return 0;
	}
	else if (size == capacity){
		list.pop_back();
		list.push_front(node);
		return 0;
	}
	else{
		return -1;
	}
}

template <typename node_T>
node_T* Queue<node_T>::get(){
	return list.end();
}

template <typename node_T>
int Queue<node_T>::MoveUpfront(ListIt node){

	list.splice(list.begin(), list, node);
	return 0;
}

//====/ Cache functions /====

template <typename cache_T>
int Cache<cache_T>::RmPage(Queue<Node<cache_T>> *queue, Map_Iter mpit){

	queue->list.pop_back();
	queue->size--;
	Map.erase(mpit);
	return 0;
}

template <typename cache_T>
int Cache<cache_T>::MovePage(Queue<Node<cache_T>> *q_src, Queue<Node<cache_T>> *q_dst, List_Iter lstit){

	q_src->size--;
	q_dst->size++;
	q_dst->list.splice(q_dst->list.begin(), q_src->list, lstit);
	return 0;
}

template <typename cache_T>
int Cache<cache_T>::add(cache_T page){
	auto hit = CacheHit(page);

	if(hit == Map.end()){//not found

		if(In.IsQueueFull()){

			if(Out.IsQueueFull()){
				
				RmPage(&Out, Map.find(Out.list.back().key));
			}

			In.list.back().affiliation = 2;

			List_Iter lstit = In.list.begin();
			std::advance(lstit, In.list.size() - 1);
			MovePage(&In, &Out, lstit);
		}

		Node<cache_T> tmp_node = {page, 1};
		In.list.push_front(tmp_node);
		In.size++;

		Map.insert({page, In.list.begin()});
		return 0;
	}

	switch (hit->second->affiliation){

		case 0:
			Hot.MoveUpfront(hit->second);
			return 1;
			break;

		case 1:
			return 1;
			break;

		case 2:
			if (Hot.IsQueueFull()) {
				RmPage(&Hot, Map.find(Hot.list.back().key));
			}
			hit->second->affiliation = 0;
			MovePage(&Out, &Hot, hit->second);
			return 1;
			break;
		default:
			return -1;
			break;
	}
}
