#include <iostream>
#include <iomanip>
#include <random>
#include <set>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>

#include "AVLTree.h"
#include "AVLTreeIterative.h"
#include "RBTree.h"

template <typename T> inline void Insert(T& tree, int value);
template <> inline void Insert<std::set<int>>(std::set<int>& tree, int value);
template <typename T> inline void Remove(T& tree, int value);
template <> inline void Remove<std::set<int>>(std::set<int>& tree, int value);

template <typename T> void TestTreeTiming(T& tree, std::vector<int>& keys, std::pair<double, double>& times);
template <typename T> void PrepareSomeTree(T& tree, std::vector<int>& keys);
template <typename T> void CheckEquality(T& tree, std::set<int>& controlSet, const char* name);

int main()
{
	const int maxValue = 10'000'000;
	const int insertSize = 1'000'000;
	const int numTests = 1;

	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> dist(0, maxValue);

	std::vector<int> insertKeys;
	insertKeys.reserve(insertSize);
	for (int i = 0; i < insertSize; i++)
	{
		int rndInt = dist(gen);
		insertKeys.push_back(rndInt);
	}

	// test insert/remove timings
	std::pair<double, double> stdTimes;
	std::pair<double, double> avlRecTimes;
	std::pair<double, double> avlIterTimes;
	std::pair<double, double> rbTimes;

	for (int n = 0; n < numTests; n++)
	{
		std::set<int> stdSet;
		AVLTree avlRec;
		AVLTreeIterative avlIter;
		RBTree rb;

		TestTreeTiming(stdSet, insertKeys, stdTimes);
		TestTreeTiming(avlRec, insertKeys, avlRecTimes);
		TestTreeTiming(avlIter, insertKeys, avlIterTimes);
		TestTreeTiming(rb, insertKeys, rbTimes);
	}

	stdTimes.first /= numTests;
	stdTimes.second /= numTests;
	avlRecTimes.first /= numTests;
	avlRecTimes.second /= numTests;
	avlIterTimes.first /= numTests;
	avlIterTimes.second /= numTests;
	rbTimes.first /= numTests;
	rbTimes.second /= numTests;

	std::cout << "Test insert/remove with " << insertSize << " elements" << '\n';
	std::cout << std::left << std::setw(10) << "tree" << std::setw(20) << "insert, ms" << std::setw(20) << "remove, ms" << '\n';
	std::cout << std::left << std::setw(10) << "std::set" << std::setw(20) << stdTimes.first << std::setw(20) << stdTimes.second << '\n';
	std::cout << std::left << std::setw(10) << "avlRec" << std::setw(20) << avlRecTimes.first << std::setw(20) << avlRecTimes.second << '\n';
	std::cout << std::left << std::setw(10) << "avlIter" << std::setw(20) << avlIterTimes.first << std::setw(20) << avlIterTimes.second << '\n';
	std::cout << std::left << std::setw(10) << "rb" << std::setw(20) << rbTimes.first << std::setw(20) << rbTimes.second << '\n';

	// test equality with std::set
	std::set<int> controlSet;
	AVLTree avlRec;
	AVLTreeIterative avlIter;
	RBTree rb;

	PrepareSomeTree(controlSet, insertKeys);
	PrepareSomeTree(avlRec, insertKeys);
	PrepareSomeTree(avlIter, insertKeys);
	PrepareSomeTree(rb, insertKeys);

	CheckEquality(avlRec, controlSet, "avlRec");
	CheckEquality(avlIter, controlSet, "avlIter");
	CheckEquality(rb, controlSet, "rb");
}

template <typename T> void TestTreeTiming(T& tree, std::vector<int>& keys, std::pair<double, double>& times)
{
	double insertTime = 0.0;
	double removeTime = 0.0;
	std::chrono::high_resolution_clock::time_point t1, t2;

	t1 = std::chrono::high_resolution_clock::now();
	for (auto& value : keys)
	{
		Insert(tree, value);
	}
	t2 = std::chrono::high_resolution_clock::now();
	insertTime += std::chrono::duration<double, std::milli>(t2 - t1).count();

	t1 = std::chrono::high_resolution_clock::now();
	for (auto& value : keys)
	{
		Remove(tree, value);
	}
	t2 = std::chrono::high_resolution_clock::now();
	removeTime += std::chrono::duration<double, std::milli>(t2 - t1).count();

	times.first += insertTime;
	times.second += removeTime;
}

template <typename T> void CheckEquality(T& tree, std::set<int>& controlSet, const char* name)
{
	std::vector<int> treeValues = tree.GetVector();
	bool isEqual = std::equal(treeValues.cbegin(), treeValues.cend(), controlSet.cbegin());
	std::cout << "Are " << name << " and std::set equal? " << (isEqual ? "yes" : "no") << '\n';
}

template <typename T> inline void Insert(T& tree, int value)
{
	tree.Insert(value);
}

template <> inline void Insert<std::set<int>>(std::set<int>& tree, int value)
{
	tree.insert(value);
}

template <typename T> inline void Remove(T& tree, int value)
{
	tree.Remove(value);
}

template <> inline void Remove<std::set<int>>(std::set<int>& tree, int value)
{
	tree.erase(value);
}

template <typename T> void PrepareSomeTree(T& tree, std::vector<int>& keys)
{
	for (int value : keys)
	{
		Insert(tree, value);
	}
	for (size_t i = 0; i < keys.size(); i += 2)
	{
		Remove(tree, keys[i]);
	}
}