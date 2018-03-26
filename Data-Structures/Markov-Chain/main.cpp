// -----------------------------------------------------------------
// HOMEWORK 7 WORD FREQUENCY MAPS
//
// You may use all of, some of, or none of the provided code below.
// You may edit it as you like (provided you follow the homework
// instructions).
// -----------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
#include <cassert>
#include <time.h>



// ASSIGNMENT: FILL IN YOUR OWN MAP STRUCTURE
typedef  std::map<std::string, std::map<std::string, int> > MY_MAP;



// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
  char c;
  word.clear();
  while (istr) {
    // just "peek" at the next character in the stream
    c = istr.peek();
    if (isspace(c)) {
      // skip whitespace before a word starts
      istr.get(c);
      if (word != "") {
	// break words at whitespace
	return true;
      }
    } else if (c == '"') {
      // double quotes are a delimiter and a special "word"
      if (word == "") {
	istr.get(c);
	word.push_back(c);
      }
      return true;
    } else if (isalpha(c)) {
      // this a an alphabetic word character
      istr.get(c);
      word.push_back(tolower(c));
    } else {
      // ignore this character (probably punctuation)
      istr.get(c);
    }
  }
  return false;
}


// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
  // returns a vector of strings of the different words
  std::vector<std::string> answer;
  std::string word;
  bool open_quote = false;
  while (ReadNextWord(istr,word)) {
    if (word == "\"") {
      if (open_quote == false) { open_quote=true; }
      else { break; }
    } else {
      // add each word to the vector
      answer.push_back(word);
    }
  }
  return answer;
}



// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
void LoadSampleText(MY_MAP &data, const std::string &filename, int window, const std::string &parse_method) {
  // open the file stream
  std::ifstream istr(filename.c_str());
  if (!istr) { 
    std::cerr << "ERROR cannot open file: " << filename << std::endl; 
    exit(1);
  } 
  // verify the window parameter is appropriate
  if (window < 2) {
    std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
  }
  // verify that the parse method is appropriate
  bool ignore_punctuation = false;
  if (parse_method == "ignore_punctuation") {
    ignore_punctuation = true;
  } else {
    std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
    exit(1);
  }



  //
  // ASSIGNMENT:SETUP YOUR MAP DATA AS NEEDED
  //

  std::string prev_word = "";
  std::string word = "";
  while (ReadNextWord(istr,word)) {
    // skip the quotation marks (not used for this part)
    if (word == "\"") continue;

    //
    // ASSIGNMENT: PROCESS ALL THE NON PUNCTUATION WORDS
    // INSERTING THESE WORDS INTO THE DATA STRUCTURE
    //

    if(word == "\n") {
      word = "";
    }

    // Increment the word count
    data[prev_word][word]++;

    prev_word = word;

  }
  // Error catching for last word
  data[prev_word][""]++;

}

void LoadSampleTextThree(std::map<std::string, MY_MAP> &data, const std::string &filename, int window, const std::string &parse_method) {
  // open the file stream
  std::ifstream istr(filename.c_str());
  if (!istr) { 
    std::cerr << "ERROR cannot open file: " << filename << std::endl; 
    exit(1);
  } 
  // verify the window parameter is appropriate
  if (window < 2) {
    std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
  }
  // verify that the parse method is appropriate
  bool ignore_punctuation = false;
  if (parse_method == "ignore_punctuation") {
    ignore_punctuation = true;
  } else {
    std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
    exit(1);
  }



  //
  // ASSIGNMENT:SETUP YOUR MAP DATA AS NEEDED
  //

  std::string prev_word = "";
  std::string current_word = "";
  std::string next_word = "";
  while (ReadNextWord(istr,next_word)) {
    // skip the quotation marks (not used for this part)
    if (next_word == "\"") continue;

    //
    // ASSIGNMENT: PROCESS ALL THE NON PUNCTUATION WORDS
    // INSERTING THESE WORDS INTO THE DATA STRUCTURE
    //

    if(next_word == "\n") {
      next_word = "";
    }

    data[prev_word][current_word][next_word]++;

    prev_word = current_word;
    current_word = next_word;

  }
  // data[prev_word][""]++;

}





int main () {

  // ASSIGNMENT: THE MAIN DATA STRUCTURE
  MY_MAP data;

  // Parse each command
  std::string command;    
  while (std::cin >> command) {

    // load the sample text file
    if (command == "load") {
      std::string filename;
      int window;
      std::string parse_method;
      std::cin >> filename >> window >> parse_method;      

      //
      // ASSIGNMENT: ADD YOUR COMMANDS HERE
      //

      // Calls a different function depending on the window size
      if(window == 2) {
        LoadSampleText(data, filename, window, parse_method);
      }
      if(window == 3) {
        LoadSampleText(data, filename, window, parse_method);
      }

      std::cout << "Loaded " << filename << " with window = " << window;
      std::cout << " and parse method = " << parse_method << std::endl;


    } 

    // print the portion of the map structure with the choices for the
    // next word given a particular sequence.
    else if (command == "print") {
      std::vector<std::string> sentence = ReadQuotedWords(std::cin);


      //
      // ASSIGNMENT: ADD YOUR COMMANDS HERE
      //

      // Test if only 1 word is given to print from
      std::cout << std::endl;

      int total_words = 0;
      std::map<std::string, int>::iterator itr = data[sentence[0]].begin();

      // Loop to find total number of occurances after the given word
      for( ; itr != data[sentence[0]].end(); itr++) {
        total_words += itr->second;
      }

      // Reset the iterator
      itr = data[sentence[0]].begin();

      std::cout << sentence[0] << " (";
      std::cout << total_words << ")" << std::endl;

      // Loop to print out the words that follow
      for( ; itr != data[sentence[0]].end(); itr++) {
        if(itr->first.length() == 0) {
          continue;
        }
        std::cout << sentence[0] << " " << itr->first << " (";
        std::cout << itr->second << ")" << std::endl;
      }

    }

    // generate the specified number of words 
    else if (command == "generate") {
      std::vector<std::string> sentence = ReadQuotedWords(std::cin);
      // how many additional words to generate
      int length;
      std::cin >> length;
      std::string selection_method;
      std::cin >> selection_method;
      bool random_flag;
      if (selection_method == "random") {
	       random_flag = true;
      } else {
	       assert(selection_method == "most_common");
	       random_flag = false;
      }


      //
      // ASSIGNMENT: ADD YOUR COMMANDS HERE
      //
      std::cout << std::endl;

      if(random_flag) {

        std::string current_word = sentence[0];

        // Loop to generate a word for as long as requested
        for(int i = 0; i <= length; i++) {
          std::cout << current_word << " ";

          // Gets a random int between 0 and number of words
          int num = std::rand() % data[current_word].size();

          int count = 0;
          std::string temp = "";
          std::map<std::string, int>::iterator itr = data[current_word].begin();

          // Loop to get the next random word
          for( ; itr != data[current_word].end(); itr++) {
            if(count == num) {
              temp = itr->first;
            }
            count++;
          }

          current_word = temp;
        }
        std::cout << std::endl;


      } else {


        std::string current_word = sentence[0];

        // Loop through to generate requested number of words
        for(int i = 0; i <= length; i++) {
          // Output formatting to make sure extra space is not at the end
          if(i != length)
            std::cout << current_word << " ";
          else
            std::cout << current_word;

          int max_count = 0;
          std::string temp = "";
          // Iterator beginning with the outputted words data
          std::map<std::string, int>::iterator itr = data[current_word].begin();
          // Loop to find the word with the highest occurance
          for( ; itr != data[current_word].end(); itr++) {
            if(itr->second > max_count) {
              max_count = itr->second;
              temp = itr->first;
            }
          }

          // Sets the current word as the word with the next highest occurance
          current_word = temp;
        }
        std::cout << std::endl;

      }


    } else if (command == "quit") {
      break;
    } else {
      std::cout << "WARNING: Unknown command: " << command << std::endl;
    }
  }
  std::cout << std::endl;
}
