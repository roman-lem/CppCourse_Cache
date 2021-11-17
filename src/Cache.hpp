#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <list>
#include <cmath>
#include <gtest/gtest.h>

enum{
	MAIN_QUEUE,
	IN_QUEUE,
	OUT_QUEUE,
};

template <typename value_T>
struct Node{
	int key;
	value_T value;
	int affiliation;

	int KeyGen(value_T value){
		return (int) value;
	}
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

	size_t GetCapacity() const { return capacity;}
	bool IsQueueFull() const { return size == capacity;}

	int add(const node_T &node);
	ListIt get() const{
		return list.end();
	}

	int MoveUpfront(ListIt node);

	void Dump() const;
};

template <typename cache_T>
struct Cache{
private:

	int c_capacity;
	int queueRatio[3] = {60, 20, 20};

public:

	using List_Iter = typename std::list<Node<cache_T>>::iterator;
	using Map_Iter = typename std::unordered_map<cache_T, List_Iter>::iterator;

	Queue<Node<cache_T>> In;
	Queue<Node<cache_T>> Out;
	Queue<Node<cache_T>> Hot;

	std::unordered_map<cache_T, List_Iter> Map;

	Cache(size_t cache_size) :
		c_capacity(cache_size), 
		In(((size_t) (trunc(cache_size * queueRatio[1]/100) >= 1)? trunc(cache_size * queueRatio[1]/100) : 1)),
		Out(((size_t) (trunc(cache_size * queueRatio[2]/100) >= 1)? trunc(cache_size * queueRatio[2]/100) : 1)),
		Hot((size_t) (c_capacity - In.GetCapacity() - Out.GetCapacity() >= 1)? c_capacity - In.GetCapacity() - Out.GetCapacity() : 1), 
		Map() {};

	int RmPage(Queue<Node<cache_T>> &queue, Map_Iter mpit);
	int MovePage(Queue<Node<cache_T>> &q_src, Queue<Node<cache_T>> &q_dst, List_Iter lstit);

	Map_Iter CacheHit(cache_T page){
		return Map.find(page);
	}

	int add(cache_T *page);

	void Dump() const;

};


//====/ Queue functioins /====
template <typename node_T>
void Queue<node_T>::Dump() const{

	int i = 0;
	for(ListIt iter = list.begin(); iter != list.end(); ++iter){
		std::cout << '\t' << i << ") " << iter->value << std::endl;
		++i;
	}

	return;
}

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
int Queue<node_T>::MoveUpfront(ListIt node){

	list.splice(list.begin(), list, node);
	return 0;
}

//====/ Cache functions /====

template <typename cache_T>
void Cache<cache_T>::Dump() const{

	std::cout << "==============================================" << std::endl;

	std::cout << "In_queue" << std::endl;
	In.Dump();

	std::cout << "Out_queue" << std::endl;
	Out.Dump();

	std::cout << "Hot_queue" << std::endl;
	Hot.Dump();

	std::cout << "==============================================" << std::endl;

	return;
}

template <typename cache_T>
int Cache<cache_T>::RmPage(Queue<Node<cache_T>> &queue, Map_Iter mpit){

	queue.list.pop_back();
	queue.size--;
	Map.erase(mpit);
	return 0;
}

template <typename cache_T>
int Cache<cache_T>::MovePage(Queue<Node<cache_T>> &q_src, Queue<Node<cache_T>> &q_dst, List_Iter lstit){

	q_src.size--;
	q_dst.size++;
	q_dst.list.splice(q_dst.list.begin(), q_src.list, lstit);
	return 0;
}

template <typename cache_T>
int Cache<cache_T>::add(cache_T *page){
	auto hit = CacheHit(*page);

	if(hit == Map.end()){//not found

		if(In.IsQueueFull()){

			if(Out.IsQueueFull()){
				
				RmPage(Out, Map.find(Out.list.back().key));
			}

			In.list.back().affiliation = OUT_QUEUE;

			List_Iter lstit = --In.list.end();
			MovePage(In, Out, lstit);
		}

		Node<cache_T> tmp_node = {tmp_node.KeyGen(*page), *page, IN_QUEUE};
		In.list.push_front(tmp_node);
		In.size++;

		Map.insert({tmp_node.key, In.list.begin()});
		return 0;
	}

	switch (hit->second->affiliation){

		case MAIN_QUEUE:
			Hot.MoveUpfront(hit->second);
			return 1;
			break;

		case IN_QUEUE:
			return 1;
			break;

		case OUT_QUEUE:
			if (Hot.IsQueueFull()) {
				RmPage(Hot, Map.find(Hot.list.back().key));
			}
			hit->second->affiliation = MAIN_QUEUE;
			MovePage(Out, Hot, hit->second);
			return 1;
			break;
		default:
			return -1;
			break;
	}
}
