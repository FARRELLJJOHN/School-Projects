#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "genome.h"

using namespace std;

// Helper function to print out query matches
//
// Arguments
//	query_string: string containing the query
//	max_mismatch: number of mismatches allowed
//	index: index of the start of the kmer in the genome
// 	genome: the genome
//
// Returns
//	true: Match successfully printed
//	false: No match with the genome
bool printMatch(const string& query_string, const int& max_mismatch, const int& index,
				const string& genome) {

	int mismatch_count = 0;
	int length = query_string.size();
	// Loop to go through the query string and genome at the same time
	for(int i = 0; i < length; i++) {
		// Check the current spot in the genome
		if(query_string[i] != genome[index + i]) 
			mismatch_count++;

		if(mismatch_count > max_mismatch)
			return false;
	}
	// Print out the genome substring if the mismatches are under
	// the limit
	cout << index << " " << mismatch_count << " ";
	cout << genome.substr(index, length) << endl;
	return true;
}


int main() {

	// Suedo global variables
	float occupancy_max;
	string genome = "";
	Genome kmer_table(100);
	kmer_table.setOccupancyMax(0.5);
	int kmer_length;

	// Main loop to read input
	string command;
	while(cin >> command) {

		// Command to load a genome sequence from
		// a specified filename
		if(command == "genome") {
			// Get the 
			string filename;
			cin >> filename;

			ifstream genome_file(filename.c_str()); // Create fstream for input

			// Read genome into one string
			string line;
			while(genome_file >> line)
				genome += line;
		}
		// Command to set the initial hash table
		// size.
		// Default value: 100
		else if(command == "table_size") {
			int t_size;
			cin >> t_size;

			Genome kmer_table(t_size);
		}
		// Command to set the ceiling for occupancy.
		// If occupancy goes above the ceiling, resize
		// the hash table. 
		// Default value: 0.5
		else if(command == "occupancy") {
			cin >> occupancy_max;

			kmer_table.setOccupancyMax(occupancy_max);
		}
		// Command to set the length of kmer indexing
		// the genome and add all the kmers to the hash table
		else if(command == "kmer") {
			cin >> kmer_length;

			for( size_t i = 0; i < (genome.length() - kmer_length); i++ ) {
				string temp_kmer = genome.substr(i, kmer_length);
				kmer_table.add(temp_kmer, i);
			}
		}
		// Command to search the genome for a specified
		// string with a specified mismatch allowance
		else if(command == "query") {
			int max_mismatch;
			string query_string;
			cin >> max_mismatch >> query_string;

			cout << "Query: " << query_string << endl;

			// Find the HashNode containing the seed
			string seed = query_string.substr(0, kmer_length);
			HashNode* temp = kmer_table.find(seed);

			if(temp) {

				int printed = 0;
				for(size_t i = 0; i < temp->genome_index_.size(); i++) {
					if(printMatch(query_string, max_mismatch, temp->genome_index_[i], genome))
						printed++;
				}

				// Check to make sure the edge case of there being seed matches
				// but no query matches
				if(printed == 0) 
					cout << "No Match" << endl;

			} else
				cout << "No Match" << endl;
		}
		// Command to quit the program
		else if(command == "quit") {
			break;
		}
	}

	return 0;
}