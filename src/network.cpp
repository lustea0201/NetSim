#include "network.h"
#include <algorithm>
#include <iostream>
#include "random.h"
#include <tclap/CmdLine.h>

void Network::resize(const size_t &n)  {
	values.resize(n);
	RNG.normal(values);
}


bool Network::add_link(const size_t& m, const size_t& n) {
	if (n==m or is_link(n,m) or n >= values.size() or m >= values.size()) return false; 
	links.insert(std::pair<size_t, size_t>(n,m));
	links.insert(std::pair<size_t, size_t>(m,n));
	return true;
}

bool Network::is_link(const size_t& m, const size_t& n) {
	std::pair <std::multimap<size_t,size_t>::iterator, std::multimap<size_t,size_t>::iterator> eqRange = links.equal_range(m);
	for (std::multimap<size_t,size_t>::iterator it=eqRange.first; it!=eqRange.second; ++it) {
		if (it->second==n) return true;
	}
	return false;
}
	
// Version 1) proposée dans le mail qui nous a été envoyé
size_t Network::random_connect(const double& mean) {
	
	links.clear();
	size_t totalCount(0), netSize(size());
	std::vector<size_t> nodes;
	for (size_t i(0); i < netSize; ++i) nodes.push_back(i);
	
	for (size_t i(0); i < netSize; ++i) {
		size_t nVal = RNG.poisson(mean); 
		RNG.shuffle(nodes);
		if (nVal >= netSize) {
			nVal = netSize - 1 ;
		}
		size_t index(0), count(0);
		while (count < nVal and index < netSize) {
			if(add_link(i, index)) ++count; 
			++index;
		}
		totalCount += count;
	}
	return totalCount;
}

size_t Network::set_values(const std::vector<double>& v) {
	size_t nSet=0;
	for (size_t i(0); i < size() and i < v.size(); ++i) {
	  values[i] = v[i]; 
	  ++nSet;
	}
	return nSet; 
}

size_t Network::size() const {
	return values.size();
}
	
size_t Network::degree(const size_t & _n) const {
	return links.count(_n);
}


double Network::value(const size_t & _n) const {
	return values[_n]; 
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> temp = values;
	std::sort(temp.begin(), temp.end(), std::greater<double>());
	return temp; 
}	  

std::vector<size_t> Network::neighbors(const size_t& n) const {
	std::vector<size_t> neighbors;
	neighbors.clear();
	for (auto i : links) {
		if (i.first == n) neighbors.push_back(i.second);
	}
	return neighbors;
}

