#include "Cache.hpp"

int main(){

	int capacity = 0;
	int numOfElements = 0;
	int page;
	int numOfHits = 0;

	std::cin >> capacity >> numOfElements;

	Cache<int> Cache(capacity);

	int result = 0;
	for(int i = 0; i < numOfElements; i++){

		std::cin >> page;
		result = Cache.add(page);
		if(result == 1){
			numOfHits++;
		}
		else if(result != 0){
			printf("ERROR!\n");
			return -1;
		}
	}

	std::cout  << numOfHits << std::endl;

	return 0;
}