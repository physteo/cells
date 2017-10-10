#pragma once
#include <vector>
#include "ObjectWithID.h"

namespace tmx { namespace gameObjects {

	template <class O>
	class SlotMap
	{
		const size_t      chunkSize;
		std::vector< std::vector<O> >   objectTable;
		std::vector<int>  freeList;

	public:
		//std::vector<objectID> ids;

	public:
		SlotMap(size_t chunkSize) : chunkSize(chunkSize) {}

		objectID createObject() {
			if (freeList.empty()) {
				std::vector<O> chunk(chunkSize);
				for (int i = chunkSize - 1; i >= 0; --i) {
					chunk.at(i).setID( objectTable.size() * chunkSize + i );
					freeList.push_back(objectTable.size() * chunkSize + i);
				}
				objectTable.push_back(chunk);
			}

			int free = freeList.back();
			freeList.pop_back();
			return objectTable.at(free / chunkSize).at(free % chunkSize).getID();
		}


		O* getObject(objectID id) {
			O* obj = &(objectTable.at((id & 0xFFFFFFFF) / chunkSize).at((id & 0xFFFFFFFF) % chunkSize));
			return obj->getID() != id ? nullptr : obj;
		}

		void destroyObject(objectID id) {
			O* obj = getObject(id);
			obj->setID( (obj->getID() & 0xFFFFFFFF) | (((obj->getID() >> 32) + 1) << 32) );
			freeList.push_back(id & 0xFFFFFFFF);
		}

	};
	

} }

