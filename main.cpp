/*main.cpp*/

//
// Hashing program for specialized Illinois license plates,
// which processes an input file of license plates and fines.
// The output is the total fines per license plate, in 
// sorted order.
//
// Original author: Prof. Joe Hummel
// Modified by:     << Brian Huang >>
//
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>

#include "ILplates.h"

using namespace std;


//
// hashInput:
//
void hashInput(string basename, ILplates& hashplates)
{
  string infilename = basename + ".txt";
  ifstream infile(infilename);

  if (!infile.good())
  {
    cout << endl;
    cout << "**Error: unable to open input file '" << infilename << "', exiting." << endl;
    cout << endl;
    exit(-1);
  }

  //
  // input the plates and fines:
  //
  cout << "Reading '" << infilename << "'..." << endl;

  string fine;
  string plate;

  getline(infile, fine);

  //
  // for each pair (fine, license plate), hash and store/update fine:
  //
  while (!infile.eof())
  {
    getline(infile, plate);

    //cout << fine << endl;
    //cout << plate << endl;

    // 
    // is plate valid?  Only process valid plates:
    //
    if (hashplates.Hash(plate) >= 0)  // yes:
    {
      int amount = hashplates.Search(plate);

      if (amount < 0)  // not found:
      {
        hashplates.Insert(plate, stoi(fine));
      }
      else  // we found it, so update total in hash table:
      {
        amount += stoi(fine);
        hashplates.Insert(plate, amount);
      }

    }//valid

    getline(infile, fine);
  }
}
//insertionSort to sort the plates and amounts vectors by using the natural C++ ordering as
//defined by the < operator.
void insertionSort(vector<string>& plates, vector<int> &amounts, int n)  
{  
    int i, j, key2;  
	string key;
    for (i = 1; i < n; i++) 
    {  
        key = plates[i];
		key2 = amounts[i];
        j = i - 1;  
  
        /* Move elements of plates[0..i-1] and amounts[0..i-1], that are  
        greater than key, to one position ahead  
        of their current position */
        while (j >= 0 && key < plates[j]) 
        {  
            plates[j + 1] = plates[j]; 
			amounts[j+1] = amounts[j];
            j = j - 1;  
        }  
        plates[j + 1] = key;  
		amounts[j+1] = key2;
    }  
}  

int main()
{
  int    N;
  string basename;

  cout << "Enter hashtable size> ";
  cin >> N;

  hashtable<string, int>  ht(N);
  ILplates                hashplates(ht);

  cout << "Enter base filename> ";
  cin >> basename;
  cout << endl;
  //
  // process input file of fines and license plates:
  //
  hashInput(basename, hashplates);

  //
  // debugging:
  //
  //get vector of plates and amount of fines for each plate
  vector<string> plates = ht.Keys();
  vector<int> amounts = ht.Values();
  cout << "Sorting..." << endl;
  //call insertionsort to sort vector plates and amounts using the natural C++ ordering as
  //defined by the < operator.
  insertionSort(plates,amounts, plates.size());
  //
  // done:
  //
  //create outfile using basename and -output.txt
  cout << "Writing " << "'" << basename + "-output.txt" << "'..."<< endl;
  ofstream outfile(basename + "-output.txt");
  //output to output file
  for(int i = 0; i < plates.size(); i++)
  {
		outfile <<  "\""<< plates[i] << "\"" << " $" << amounts[i] << endl;
  }
  return 0;
}