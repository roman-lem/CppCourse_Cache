struct twoQCache {

	std::list<int> mainLRU;
	std::list<int> AinQueue;
	std::list<int> AoutQueue;
};

int SimpleHash (int key) {

	return key%2;
}