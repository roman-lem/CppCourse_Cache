#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unordered_map>

template <typename cache_T>
struct Cache{
private:
	size_t capacity;
public:
	size_t size = 0;

	cache_T* list;
	std::unordered_map<cache_T, cache_T*> Map;

	Cache(int cacheCpct): capacity(cacheCpct){list = new cache_T[capacity];}
	~Cache(){delete[] list;}

	bool isCacheFull(){
		return size == capacity;
	}

	int add(cache_T page, cache_T* future, int elementsLeft);
};

template <typename cache_T>
int Cache<cache_T>::add(cache_T page, cache_T* future, int elementsLeft){

	auto hit = Map.find(page);

	if(hit == Map.end()){
		if(isCacheFull()){
			int farMatchPos = 0;
			int farElement = 0;
			int iMatchPos = 0;
			for(int i = 0; i < size; i++){
				iMatchPos = 0;

				for(int j = 0; j < elementsLeft; j++){
					iMatchPos++;
					if(list[i] == future[j+1]){
						break;
					}
				}

				if(iMatchPos > farMatchPos){
					farMatchPos = iMatchPos;
					farElement = i;
				}
			}
			Map.erase(list[farElement]);
			list[farElement] = page;
			Map.insert({page, &list[farElement]});
			return 0;
		}

		size++;
		list[size-1] = page;
		Map.insert({page, &list[size-1]});
		return 0;
	}

	return 1;
}