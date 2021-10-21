#include "PerfCache.hpp"

int main() {

	int cacheSz  = 0;
	int numOfElements = 0;

	std::cin >> cacheSz >> numOfElements;

	int* future = (int*) calloc(numOfElements, sizeof(*future));

	for(int i = 0; i < numOfElements; i++){
		std::cin >> future[i];
	}

	Cache<int> cache{cacheSz};

	int numOfHits = 0;
	int result = 0;
	for(int i = 0; i < numOfElements; i++){
		
		result = cache.add(future[i], &future[i], numOfElements - (i+1));
		numOfHits += result;
	}

	std::cout << numOfHits << std::endl;

	free(future);

	return 0;
}