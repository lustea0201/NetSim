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
	
/* 
Version 1) proposée dans le mail qui nous a été envoyé.
Avec n nodes, le nombre maximum de liens qui peuvent être créés est
donné par n/2*(n - 1). Donc, plus n*d est proche de n/2*(n - 1), plus 
le nombre de liens créés s'éloignera de n*d.
*/

size_t Network::random_connect(const double& mean) {
	if (mean < 0) return 0;
	links.clear();
	size_t totalCount(0), netSize(size());
	std::vector<size_t> nodes;
	for (size_t i(0); i < netSize; ++i) nodes.push_back(i);
	
	for (size_t i(0); i < netSize; ++i) {
		size_t nLinks = RNG.poisson(mean); 
		RNG.shuffle(nodes);
		if (nLinks >= netSize) {
			nLinks = netSize - 1;
		}
		size_t index(0), count(0);
		while (count < nLinks and index < netSize) {
			if(add_link(i, index)) ++count; 
			++index;
		}
		totalCount += count;
	}
	return totalCount;
}

size_t Network::set_values(const std::vector<double>& v) {
	size_t nSet=std::min(size(), v.size());
	for (size_t i(0); i < nSet; ++i) {
	  values[i] = v[i]; 
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
	std::pair <std::multimap<size_t,size_t>::const_iterator, std::multimap<size_t,size_t>::const_iterator> eqRange = links.equal_range(n);
	for (std::multimap<size_t,size_t>::const_iterator it=eqRange.first; it!=eqRange.second; ++it) {
		neighbors.push_back(it->second);
	}
	return neighbors;
}

