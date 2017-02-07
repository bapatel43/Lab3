#include "Password.h"
using CSC2110::ListArrayIterator;

#include <iostream>
using namespace std;

#include "Text.h"

//constructor
Password::Password()
{
   len = 0;
   viable_words = new ListArray<String>();
   all_words = new ListArray<String>();
}

//destructor (there is work to do here, delete the individual words)
Password::~Password()
{
   
   
   for(int i = 0; i < viable_words->size();i++){
	   viable_words->remove(i);
   }
   delete viable_words;
   
    for(int j = 0; j < all_words->size();j++){
	   all_words->remove(j);
   }
   delete all_words;
   
}


//a private helper method to report the number of character matches between two Strings
int Password::getNumMatches(String* curr_word, String* word_guess)
{
   int hits = 0;

   for(int i = 0; i <= len; i++)
   {
      if(curr_word->charAt(i) == word_guess->charAt(i))
      {
         hits++;
      }   
   } 

   return hits++;    
}




//add a word to the list of possible passwords
void Password::addWord(String* word)
{
   if (word->length() > 0)   
   {
      viable_words->add(word);
      all_words->add(word);      
   }

} 

//index of guessed word in the list of all words (1-based)
//number of matches reported by fallout 3, update viable passwords list
/*
void Password::guess(int try_password, int num_matches)
{
   String* word;
   int match;
   ListArray<String>* hold;
   ListArrayIterator<String>* iter = viable_words->iterator();
   while(iter->hasNext())
   {
      word = getOriginalWord(try_password);
      String* pass = iter->next();
      match = getNumMatches(pass, word);
      if(match == num_matches)
      {
         hold->add(word);
      }   
   }

	viable_words = hold;
	delete viable_words;
} 
*/

	void Password::guess(int try_password,int num_matches)
	{
		
		ListArray<String>* newwords = new ListArray<String>();// creat a new list array
		ListArrayIterator<String>* iterator = viable_words->iterator();//sets up it
		String* password = getOriginalWord(try_password);
		while(iterator->hasNext())
		{
			String* word = iterator->next();//store the next words
			int word_matches = getNumMatches(word,password);// see if the word has the right number of matches
			if(num_matches == word_matches)
			{
				newwords->add(word);// add words to a new list
			}
			
		}
		delete viable_words;// del old list pointer.
		viable_words = newwords;// reassign
		
	}

//returns the number of possible passwords remaining
int Password::getNumberOfPasswordsLeft()
{
   return viable_words->size();
} 

//display the current list of possible passwords
void Password::displayViableWords()
{
   ListArrayIterator<String>* ite = viable_words->iterator();
   while(ite->hasNext())
   {
      String* displayPass = ite->next();
     
      displayPass->displayString();
	  
	  cout << endl;
   }
} 

//get a word from the original list of all passwords, 1-based
String* Password::getOriginalWord(int index)
{
   return all_words->get(index);
}


 //the best word in the original list to guess next (done for you)
int Password::bestGuess()
{
   int best_guess_index = -1;
   int best_num_eliminated = -1;
   int num_viable_passwords = getNumberOfPasswordsLeft();

   //loop over ALL words, even if they have been eliminated as the password
   int count = 1;
   ListArrayIterator<String>* all_iter = all_words->iterator();
   while(all_iter->hasNext())
   {
      String* original_word = all_iter->next();

      //loop over only those words that could still be the password
      //count up the number of matches between a possible password and a word in the original list
      int* count_num_matches = new int[len + 1];

      for (int i = 0; i < len; i++) 
      {
         count_num_matches[i] = 0;
      }

      ListArrayIterator<String>* viable_iter = viable_words->iterator();
      while(viable_iter->hasNext())
      {
         String* viable_word = viable_iter->next();
         int num_matches = getNumMatches(viable_word, original_word);
         count_num_matches[num_matches]++;
      }
      delete viable_iter;

      //find the largest number in the count_num_matches array
      //the largest number indicates the guess that will generate the most eliminations
      int most_num_matches = 0;
      for (int j = 0; j < len; j++) 
      {
         int curr_num_matches = count_num_matches[j];
         if (curr_num_matches > most_num_matches)
         {
            most_num_matches = curr_num_matches;
         }
      }

      //compute the fewest that can possibly be eliminated by guessing the current word (original list)
      int num_eliminated = num_viable_passwords - most_num_matches;

      //select the word to guess that maximizes the minimum number of eliminations (minimax)
      if (num_eliminated > best_num_eliminated)
      {
         best_num_eliminated = num_eliminated;
         best_guess_index = count;
      }
      
      count++;
      delete[] count_num_matches;
   }

   delete all_iter;
   return best_guess_index;  //return a 1-based index into the all_words list of words (careful)
}
