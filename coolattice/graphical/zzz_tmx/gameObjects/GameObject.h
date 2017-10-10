#pragma once
#include "ObjectWithID.h"
#include "SlotMap.h"

namespace tmx { namespace gameObjects {

	// Basic class for a game object with two components
	// The two components are referenced by an ID through a SlotMap 
	// inside the class TwoComponentObjectVector
	class TwoComponentsGameObject : public SelectableObject
	{
	public:
		TwoComponentsGameObject(objectID ID1, objectID ID2) : SelectableObject(false), m_ID1(ID1), m_ID2(ID2) {}
	
		inline objectID getID1()	 { return m_ID1; }
		inline objectID getID2()	 { return m_ID2; }
	
	private:
		objectID m_ID1;
		objectID m_ID2;
	
	};
	


	// Container for an (unordered) ensamble of game objects, composed of only two components.
	// It uses a slot map for each component, so that it is fast (cache friendly enough) to iterate through them,
	// during integration of the physics or rendering for example.
	// It has a std::vector of game objects which reference to the elements in the slotmaps. The game objects inside
	// this vector are not ordered, because the elements are deleted using swap-pop idiom.
	// Possible improvement: object pools (more cache friendly). Problem: fixed amount of objects allocated.
	// For now this data structure should be a good compromise.
	template <class Physics, class Graphics>
	class TwoComponentsGameObjectVector
	{
		SlotMap<Physics>    mapPart;
		SlotMap<Graphics>   mapRend;

		std::vector<TwoComponentsGameObject> gos;

	public:
		TwoComponentsGameObjectVector(size_t chunkSize) : mapPart(chunkSize), mapRend(chunkSize) {}


		size_t size()
		{
			return gos.size();
		}

		void createGameObject()
		{
			gos.push_back(TwoComponentsGameObject(mapPart.createObject(), mapRend.createObject()));
		}


		Physics* getPhys(size_t i)
		{
			return mapPart.getObject(gos.at(i).getID1());
		}

		Graphics* getRend(size_t i)
		{
			return mapRend.getObject(gos.at(i).getID2());
		}



		void selectObject(size_t i)
		{
			gos.at(i).select();
			Physics* p = mapPart.getObject(gos.at(i).getID1());
			p->select();
			Graphics* g = mapRend.getObject(gos.at(i).getID2());
			g->select();
		}

		void unselectObject(size_t i)
		{
			gos.at(i).select(false);
		}

		void destroySelectedObjects()
		{
			for (size_t i = 0; i < gos.size(); i++)
			{
				if (gos.at(i).isSelected())
				{
					destroyObject(i);
					i--;
				}
			}
		}

	private:
		objectID getPhysID(size_t i)
		{
			return gos.at(i).getID1();
		}

		objectID getRendID(size_t i)
		{
			return gos.at(i).getID2();
		}

		void destroyObject(size_t i)
		{
			mapPart.destroyObject(gos.at(i).getID1());
			mapRend.destroyObject(gos.at(i).getID2());
			std::swap(gos.at(i), gos.back());
			gos.pop_back();
		}

	};







} }
