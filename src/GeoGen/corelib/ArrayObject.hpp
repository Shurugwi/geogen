/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <vector>
#include <list>

#include "../runtime/ManagedObject.hpp"
#include "../random/RandomSeed.hpp"

namespace geogen
{
	namespace corelib
	{
		/// Implementation of a reference to Array item script object (a hybrid data structure combining a sorted list and an unsorted associative array).
		class ArrayObject : public runtime::ManagedObject
		{
		public:
			typedef std::map<ManagedObject*, ManagedObject*, geogen::runtime::CompareObjects> HashMap;
			typedef std::list<ManagedObject*> List;
			typedef List::iterator iterator;
			typedef List::const_iterator const_iterator;
			typedef List::reverse_iterator reverse_iterator;
			typedef List::const_reverse_iterator const_reverse_iterator;
		private:
			HashMap hashmap;
			List list;

			int maxIntegerKey;
		public:
			ArrayObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type) : ManagedObject(vm, type), maxIntegerKey(-1)
			{
			};

			virtual ~ArrayObject();

			inline const_iterator Begin() const { std::list<ManagedObject*>::const_iterator it = this->list.begin(); return (const_iterator&)(it); }
			inline const_iterator End() const { std::list<ManagedObject*>::const_iterator it = this->list.end(); return (const_iterator&)(it); }

			inline iterator Begin() { return this->list.begin(); }
			inline iterator End() { return this->list.end(); }

			inline const_reverse_iterator RBegin() const { std::list<ManagedObject*>::const_reverse_iterator it = this->list.rbegin(); return (const_reverse_iterator&)(it); }
			inline const_reverse_iterator REnd() const { std::list<ManagedObject*>::const_reverse_iterator it = this->list.rend(); return (const_reverse_iterator&)(it); }

			inline reverse_iterator RBegin() { return this->list.rbegin(); }
			inline reverse_iterator REnd() { return this->list.rend(); }

			inline HashMap::iterator MBegin() { return this->hashmap.begin(); }
			inline HashMap::iterator MEnd() { return this->hashmap.end(); }

			void Set(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key, ManagedObject* value);
			ManagedObject* Get(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key);
			ManagedObject* GetActualKey(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key);

			ManagedObject* Front(runtime::VirtualMachine* vm, CodeLocation location);
			ManagedObject* Back(runtime::VirtualMachine* vm, CodeLocation location);

			//ManagedObject* PushFront(runtime::VirtualMachine* vm, ManagedObject* object);
			void PushBack(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* object);

			void PopFront(runtime::VirtualMachine* vm, CodeLocation location);
			void PopBack(runtime::VirtualMachine* vm, CodeLocation location);

			bool ContainsKey(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key);
			bool ContainsValue(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* value);

			//void Insert(runtime::VirtualMachine* vm, unsigned index, ManagedObject* object);
			//void RemoveAt(runtime::VirtualMachine* vm, CodeLocation location, unsigned index);
			void RemoveKey(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key);
			void RemoveValue(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* value);

			inline unsigned Count() const { return this->hashmap.size(); };

			ManagedObject* GetKeyByIndex(runtime::VirtualMachine* vm, CodeLocation location, int index);
			ManagedObject* GetValueByIndex(runtime::VirtualMachine* vm, CodeLocation location, int index);

			void SortByKeys(runtime::VirtualMachine* vm, CodeLocation location);
			void SortByValues(runtime::VirtualMachine* vm, CodeLocation location);

			void Shuffle(runtime::VirtualMachine* vm, CodeLocation location, random::RandomSeed randomSeed);

			virtual String GetStringValue() const;
		};
	}
}