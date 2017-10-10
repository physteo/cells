#include "SlotMap.h"

namespace tmx { namespace gameObjects {

	/*
	template <class O>
	objectID SlotMap<O>::createObject() {
		if (freeList.empty()) {
			std::vector<O> chunk(chunkSize);
			for (int i = chunkSize - 1; i >= 0; --i) {
				chunk.at(i).id = objectTable.size() * chunkSize + i;
				freeList.push_back(objectTable.size() * chunkSize + i);
			}
			objectTable.push_back(chunk);
		}

		int free = freeList.back();
		freeList.pop_back();
		return objectTable.at(free / chunkSize).at(free % chunkSize).id;
	}

	template <class O>
	O* SlotMap<O>::getObject(objectID id) {
		O* obj = &(objectTable.at((id & 0xFFFFFFFF) / chunkSize).at((id & 0xFFFFFFFF) % chunkSize));
		return obj->id != id ? nullptr : obj;
	}

	template <class O>
	void SlotMap<O>::destroyObject(objectID id) {
		O* obj = getObject(id);
		obj->id = (obj->id & 0xFFFFFFFF) | (((obj->id >> 32) + 1) << 32);
		freeList.push_back(id & 0xFFFFFFFF);
	}
	*/

} }