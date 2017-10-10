#pragma once

namespace tmx { namespace gameObjects {

	class SelectableObject
	{
	public:
		SelectableObject() : selected(false) {}
		SelectableObject(bool sel) : selected(sel) {}

		void select() { selected = true; }
		void unselect() { selected = false; }
		inline bool isSelected() const { return selected; }

	private:
		bool selected;
	};


	typedef long long objectID;

	class SelectableObjectWithID : public SelectableObject
	{
	public:
		SelectableObjectWithID() : SelectableObject(), id(0) {}
		SelectableObjectWithID(bool sel, objectID n) : SelectableObject(sel), id(n) {}
		inline objectID getID() { return id; }
		void setID(objectID newID) { id = newID; }

	private:
		objectID id;
	};



} }

