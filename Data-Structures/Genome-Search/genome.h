#ifndef _GENOMEHASH_H_
#define _GENOMEHASH_H_

/*
For morning john,
Use the hash function given in lecture
look at piazza
Just finished HashNode, it should be good.
Everything compiles nicely, but just return warnings
Get hash function and you're almost done
*/

#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

// Basic array to store kmers and index values
// Keeps track of the position inside the node
class Genome;

class HashNode {
public:
	HashNode(const string& kmer, vector<int>& index) {
		kmer_ = kmer;
		genome_index_ = index;
	}

	// Value of the kmer
	string kmer_;
	// Vector containing all the indices that the kmer appears
	vector<int> genome_index_;
};

class Genome {

public:
	// Constructor
	Genome(unsigned int alloc_size = 100) : hash_table_(alloc_size, NULL),
											 size_(0),
											 occupancy_max_(0.5) {};

	~Genome() {
		for(size_t i = 0; i < hash_table_.size(); i++)
			delete hash_table_[i];
		hash_table_.clear();
	}

	// Add a kmer to the hashtable
	bool add(string& kmer, int pos);
	// Set the occupancy
	void setOccupancyMax(float occ) { occupancy_max_ = occ; }
	// Checks the occupancy
	bool breaksOccupancy() const;
	// Find a kmer if its in the table
	HashNode* find(string& kmer);

private:
	// Vector of HashNodes
	vector<HashNode*> hash_table_;
	// Number if unique items in the table
	unsigned int size_;
	// Maximum occupancy value from 0.0-1.0
	float occupancy_max_;

	// Hash Function
	unsigned int hash_func(string& kmer, unsigned int& size);
	// Helper function to resize the hash table
	void resize();
};

// Hash function from Lecture 22
unsigned int Genome::hash_func(string& kmer, unsigned int& size) {
	unsigned int value = 0;
	for(unsigned int i = 0; i < kmer.size(); i++)
		value = value*8 + kmer[i];

	return value % size;
}

// Function to add a kmer into the hash table
// 
// Arguments
//	kmer: String containing the kmer to be added
//	pos: Index in the genome of the added
//
// Returns
//	true: kmer is added successfully
//	false: kmer is not added
bool Genome::add(string& kmer, int pos) {
	unsigned int size = hash_table_.size();
	unsigned int index = hash_func(kmer, size);

	// Current index is empty
	if(!hash_table_[index]) {
		vector<int> indexes;
		indexes.push_back(pos);
		hash_table_[index] = new HashNode(kmer, indexes);
		size_++;
		if(breaksOccupancy()) { resize(); }
	}
	// Current index contains the provided kmer 
	else if(hash_table_[index]->kmer_ == kmer) {
		hash_table_[index]->genome_index_.push_back(pos);
		return true;
	}
	// Current index contains a different kmer,
	// need to linear probe
	else {
		bool found = false;
		int i = 1;

		while(!found) {
			unsigned int new_index = (index + i) % size;

			// New spot is empty
			if(!hash_table_[new_index]) {
				// Fill the spot with a new kmer node
				vector<int> indexes;
				indexes.push_back(pos);
				hash_table_[new_index] = new HashNode(kmer, indexes);
				size_++;
				// Check to make sure it doesn't need to be resized
				if(breaksOccupancy()) { resize(); }

				// Get out of the loop
				found = true;
				return found;
			}
			// New spot contains current kmer 
			else if(hash_table_[new_index]->kmer_ == kmer) {
				// Add the genomic index to the kmers list
				hash_table_[new_index]->genome_index_.push_back(pos);
				found = true;
				return found;
			}
			// New spot is filled with something else
			else { i++; }
		}
	}
	
	return true;
}

// Function to find a specific kmer
//
// Arguments
//	kmer: String containing the kmer to look for
//
// Returns
//	HashNode* containing the kmer if its in the table
// 	NULL if kmer is not in the table
HashNode* Genome::find(string& kmer) {
	unsigned int size = hash_table_.size();
	unsigned int index = hash_func(kmer, size);

	// Check the first hashed node
	if(hash_table_[index]->kmer_ == kmer) 
		return hash_table_[index];
	// Go into linear probing to check nodes 
	else {
		bool found = false;
		int i = 1;

		// Loop to go through the linear probe values
		while(!found) {
			unsigned int new_index = (index + i) % size;

			// New kmer node exists
			if(hash_table_[new_index]) {
				// Check if its the right kmer
				if(hash_table_[new_index]->kmer_ == kmer) {
					found = true;
					return hash_table_[new_index];
				}
				// Go to the next linear probe
				else {
					i++;
					continue;
				}
			} else
				return NULL;
		} // while(!found)
	}
	return NULL;
}

// Helper function to just check if the occupancy is broken
bool Genome::breaksOccupancy() const {
	if( (float(size_) / float(hash_table_.size()) ) > occupancy_max_ )
		return true;

	return false;
}

void Genome::resize() {
	unsigned int new_size = hash_table_.size() * 2;
	vector<HashNode*> new_table(new_size);

	// Loop through the entire current hash table
	for(size_t i = 0; i < hash_table_.size(); i++) {

		// If the item exists, get a new hash value
		// and add it to the new table
		if(hash_table_[i]) {
			string kmer = hash_table_[i]->kmer_;

			unsigned int value = hash_func(kmer, new_size);

			if(!new_table[value])
				new_table[value] = new HashNode(kmer, hash_table_[i]->genome_index_);
		}

		// Clean up the old memory
		delete hash_table_[i];
	}

	hash_table_.clear();

	// Reassign the hash table
	hash_table_ = new_table;
}


#endif