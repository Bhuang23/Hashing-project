/*ILplates.cpp*/

//
// Hashing functions to store (license plate, amount) pairs
// using linear probing.
//
// << Brian Huang >>
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <string>
#include <math.h>


#include "ILplates.h"

using namespace std;


//
// Hash:
//
// Given a specialized Illinois license plate, returns an index into
// the underyling hash table.  If the given plate does not follow the
// formatting rules given below, -1 is returned.
//
// Personalized:
//   letters and numbers, with a space between the letters 
//   and numbers.  Format: 1-5 letters plus 1..99 *OR* 
//   6 letters plus 1..9
//
//   Examples: A 1, B 99, ZZZZZ 1, ABCDEF 3
//
// Vanity:
//   Format: 1-3 numbers *OR* 1-7 letters
// 
//   Examples: 007, 1, 42, X, AAA, ZZZEFGH
//
int ILplates::Hash(string plate)
{
  long long index = -1;
  //
  // TODO:
  //
  int numbers = 0;
  int letters = 0;
  int spaces = 0;
  for(int i = 0; i < plate.size(); i++)//find number of numbers and letters
  {
	  if(isdigit(plate[i]))
	  {
		  numbers++;
	  }
	  else if((plate[i]-'A' >= 0) && (plate[i]-'A' <= 26))
	  {
		  letters++;
	  }
	  else if(plate[i] == ' ')
	  {
		  spaces++;
	  }
	  else
	  {
		  return index;
	  }
  }
  int consecutive = 0;
  for(int i = 0; i < plate.size(); i++)//find number of consecutive letters
  {
		  if(isdigit(plate[i]))
		  {
			  break;
		  }
		  if((plate[i]-'A' >= 0) && (plate[i]-'A' <= 26))
		  {
			  consecutive++;
		  }
		  if(plate[i] == ' ')
		  {
			  break;
		  }
	  }
  if(plate.size() > 8)
  {
	  return index;
  }
  else if(spaces == 1 && (plate.size() >= 3 && (((numbers >=1) && (numbers <= 2))&& letters >=1)))
  {
	  //personalized
	  if(plate.size() <=8 && (consecutive >= 1 && consecutive <= 5) && (isdigit(plate[consecutive+1])&&consecutive+2==plate.size()) || (isdigit(plate[consecutive+1]) && isdigit(plate[consecutive+2])))
	  {
		  //1-5 letters plus 1...99
		  for(int i =0; i < consecutive; i++)//1-5 letters
		  {
			  index = index + (plate[i]-'A'+11)*pow(36,(plate.size()-i));
		  }
		  if(consecutive+1 < plate.size() && (int)(plate[consecutive+1]-'0') > 0 && consecutive+2 == plate.size())//1-10
		  {
			index = index + (int)(plate[consecutive+1]-'0')*pow(36,plate.size()-(consecutive +1));
			return index%HT.Size();
		  }
		  else if((consecutive+2 < plate.size()) && (consecutive+3 == plate.size()) && (isdigit(plate[consecutive+1]) && isdigit(plate[consecutive+2])))//10-99
		  {
		  	index = index + (int)(plate[consecutive +1]-'0')*pow(36,plate.size()-consecutive +1);
		  	index = index + (int)(plate[consecutive +2]-'0')*pow(36,plate.size()-consecutive +2);
			return index%HT.Size();
		  }
		  else
		  {
			  return -1;
		  }
	
	  }
	  else if((spaces == 1) && ((plate.size() == 8) && (consecutive == 6) && (isdigit(plate[7]))))
	  {
			//6 letters plus 1...9
			for(int i = 0; i < 6; i++)//6 letters
			{
				index = index + (plate[i]-'A'+11)*pow(36, (plate.size()-i));
			}
		    index = index + (int)(plate[7]-'0')*pow(36, (plate.size()-7));//1...9
		    return index%HT.Size();
	  }
	  else
	  {
		  return index;
	  }
	  
  }
  else if((spaces == 0) && (((numbers >=1 && numbers <=3) && letters == 0) || (numbers == 0 && (letters >=1 && letters <= 7)) && consecutive==plate.size())){
	  //vanity
	  for(int i = 0; i < plate.size(); i++)
	  {
		  if(isdigit(plate[i]))//1-3 numbers
		  {
			  index = index + (int)(plate[i]-'0')*pow(36, plate.size()-i);
		  }
		  else if(plate[i]-'A' >= 0 && plate[i]-'A' <= 26)//1-7 letters
		  {
			  index = index + (plate[i]-'A'+11)*pow(36, plate.size()-i);
		  }
	  }
	  return index%HT.Size();
  }
  else{
	 //invalid license plate
	 return index;
  }
}


//
// Search
// 
// Hashes and searches for the given license plate; returns the 
// associated value for this plate if found, or -1 if not found.
//
int ILplates::Search(string plate)
{
  //
  // TODO:
  //
  int bucket = Hash(plate);
	int N = HT.Size();
  int bucketsProbed = 0;
	bool empty;
	string key;
	int value;
   HT.Get(bucket, empty, key, value);//retrieve empty, key and value
   while ((empty != true) && (bucketsProbed < N)) {

      if (key == plate) {//check if the key at the index bucket is the plate we are looking for
        return value;
		  //return value if given license plate is found
     }
     // Increment bucket index
     bucket = (bucket + 1) % N;
      // Increment number of buckets probed
     ++bucketsProbed;
	HT.Get(bucket, empty, key, value);
   }
   return -1;
}


//
// Insert
//
// Inserts the given (plate, newValue) into the hash table,
// overwriting an existing value if there.
//
void ILplates::Insert(string plate, int newValue)
{
  //
  // TODO:
  //
  int N = HT.Size();
  int bucket = Hash(plate);
  int bucketsprobed = 0;
  while(bucketsprobed < N)
  {
	  bool empty;
	  string key1;
	  int value1;
	 HT.Get(bucket, empty, key1, value1);
     if(empty == true)//insert new value because there is no value there
     {
        HT.Set(bucket, plate, newValue);
        return;
     }
	 else if(key1 == plate)//overwrite existing value 
	 {
		HT.Set(bucket, plate, newValue);
        return;
	 }
     bucket = (bucket+1)%N;//keep on probing until you reach empty or you find the key you are looking for
     bucketsprobed++;
  }
}
