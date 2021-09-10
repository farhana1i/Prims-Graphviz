#pragma once
#include <assert.h>
#include <algorithm>
#include<iostream>

template <typename k, typename v>
class MinHeap{
	struct HeapItem{
		k key;
		v value;
	};
private:
	HeapItem* arr;
	int* idPos;		// to store index of each id i.e idPos[id] = index
	int capacity;
	int totalItems;
	void doubleCapacity(){
		if (this->arr == nullptr){
			this->arr = new HeapItem[1];
			this->idPos = new int[1];
			this->capacity = 1;
			return;
		}
		int newCapacity = capacity* 2;
		HeapItem* newArr = new HeapItem[newCapacity];
		
		if (this->idPos) delete[] this->idPos;
		this->idPos = new int[newCapacity];
		for (int i = 0; i < this->totalItems; i++){
			newArr[i] = this->arr[i];
			idPos[arr[i].value] = i;		// store index of id in heap
		}
		if (this->arr != nullptr)
			delete this->arr;
		this->capacity = newCapacity;
		this->arr = newArr;
	}
	// upward heapification
	void shiftUp(int index){
		if (index < 1)
			return;
		int parent = (index - 1) / 2;
		if (this->arr[index].key < this->arr[parent].key) {
			std::swap(this->arr[index], this->arr[parent]);
			std::swap(idPos[arr[index].value], idPos[arr[parent].value]);
		}
		shiftUp(parent);
	}
	// downward heapification
	void shiftDown(int index){
		int minIndex = -1;
		int lChildIndex = index * 2 + 1;
		int rChildIndex = (index * 2) + 2;

		if (lChildIndex < totalItems){
			if (arr[index].key > arr[lChildIndex].key){
				minIndex = lChildIndex;
			}
		}
		if (rChildIndex < totalItems){
			if (arr[(minIndex == -1 ? index : minIndex)].key > arr[rChildIndex].key){
				minIndex = rChildIndex;
			}
		}
		if (minIndex == -1) return;
		std::swap(arr[index], arr[minIndex]);
		std::swap(idPos[arr[index].value], idPos[arr[minIndex].value]);
		shiftDown(minIndex);
	}

public:
	MinHeap()
	{
		this->arr = nullptr;
		this->capacity = 0;
		this->totalItems = 0;
		this->idPos = nullptr;
	}
	MinHeap(int _capacity)
	{
		assert(_capacity >= 1);

		this->arr = new HeapItem[_capacity];
		this->idPos = new int[_capacity];
		this->capacity = _capacity;
		this->totalItems = 0;
	}
	// insert new element
	void insert(k const key, v const value){
		if (this->totalItems == this->capacity){
			doubleCapacity();
		}
		this->arr[totalItems].key = key;
		this->arr[totalItems].value = value;
		this->idPos[value] = totalItems;	// to keep record of index of each vertex in heap

		shiftUp(totalItems);
		this->totalItems++;
	}
	// to get and delete minimum
	void getMinimum(v& value){
		assert(totalItems != 0);
		value = this->arr[0].value;
	}
	// to decrease key
	void decreaseKey(int index, k const key) {
		assert(index >= 0 && index < this->capacity);
		if (key < this->arr[index].key) {
			this->arr[index].key = key;
			shiftUp(index);
		}
	}
	// to delete minimum element
	void deleteMin(){
		assert(totalItems != 0);
		std::swap(arr[0], arr[this->totalItems - 1]);
		std::swap(this->idPos[arr[0].value], this->idPos[arr[this->totalItems - 1].value]);
		totalItems--;
		//shift down
		shiftDown(0);
	}
	// return true, if empty
	bool isEmpty() const{
		return (totalItems == 0);
	}

	int getValueInd(const v& value) {
		assert(value >= 0 && value < this->capacity);
		return this->idPos[value];
	}

	// to delete all nodes
	void deleteAll()
	{
		if (this->arr != nullptr)
		{
			delete[] this->arr;
			arr = nullptr;
			delete[] this->idPos;
			this->capacity = 0;
			this->totalItems = 0;
		}
	}
	~MinHeap(){
		deleteAll();
	}
};