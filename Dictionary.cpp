// Implementation file for the Dictionary class
// Stores data in a basic binary search tree
// By Ben Goering

#include "Dictionary.h"
#include <iostream>
#include <algorithm>

using namespace std;

const int Dictionary::prime[14] = {101,211,431,863,1733,3469,6947,13901,27803,55609,111227,222461,444929,889871};
const int Dictionary::secPrime[14] = {97,199,421,859,1723,3467,6917,13883,27799,55603,111217,222437,444901,889829};
    
Dictionary::Dictionary()
{
    arraySize = this->prime[hashCount];
    dictArray = new string*[arraySize]{};
    dataNum = 0;
}
Dictionary& Dictionary::operator=(const Dictionary& rhs)
{
    if (this != &rhs) 
    {
        clear();   // delete the table;
        copy(rhs); // copy rhs's table into this
    }
    return *this;
}

//checks if array is empty
bool Dictionary::isEmpty() const
{
    if (dataNum == 0)
        return true;
    return false;
}

//gets values are data
unsigned int Dictionary::getValue(const string & key) const
{
    unsigned int hashVal = 0;

    for(char ch : key)
        hashVal = 37 * hashVal + ch;


    return hashVal;
}
unsigned int Dictionary::getHashValue(const string & key) const
{
    unsigned int hashVal = getValue(key);
    return hashVal % arraySize;
}
unsigned int Dictionary::getIncrement(const string & key) const
{
    int newSize = this->secPrime[hashCount];
    unsigned int hashVal = getValue(key);
    return (hashVal % newSize) + 1;
}
int Dictionary::secHashVal(const string& data, int index) const
{
    int start = index;
    int increment = getIncrement(data);
    while (dictArray[index])
    {
        index = (index + increment) % arraySize;

        if (index == start)
            return -1;
    }
    return index;
}

//public adding
void Dictionary::addEntry(string* anEntry)
{
    //ensure theres room
    if (dataNum+1 > (arraySize/2))
        rehash();
    //proceed to add
    probe(anEntry);
}
//adds data; private
void Dictionary::probe(string* data)
{
    //initial index
    int hashValue = getHashValue(*(data));

    //gets secondary index
    if (dictArray[hashValue])
        hashValue = secHashVal(*(data),hashValue);
    //adds
    dictArray[hashValue] = data;
    dataNum++;
    
}
//reallocate size of array and rehash elements in order
void Dictionary::rehash()
{   
    //manage variables
    hashCount++;
    int oldSize = arraySize;
    arraySize = prime[hashCount];
    string** oldData = dictArray;
    dictArray = new string*[arraySize]{};
    dataNum = 0;

    //probe old data in new array
    for (int x = 0; x < oldSize; x++)
    {
        if (oldData[x] != nullptr)
            {
                probe(oldData[x]);
                oldData[x] = nullptr;
            }
    }

    delete[] oldData;
}

//hashes for data
bool Dictionary::findEntry(const string& data) const
{
    //checks first index
    int index = getHashValue(data);
    
    if (dictArray[index] != nullptr)
    {
        if (*(dictArray[index]) == data)
            return true;
    }

    //check secondarys
    int start = index;
    int offset = getValue(data)%secPrime[hashCount]+1;
    index = (index + offset) % arraySize; //begins with first secondary index
    
    while (index != start)
    {
        if (dictArray[index] != nullptr) //checks data
        {
            if (*(dictArray[index]) == data)
                return true;
        }

        index = (index + offset) % arraySize; //increment

    }
    return false;
}

//puts data in vector and sorts using algorithm sort function
vector<string> Dictionary::getInOrder() const
{
    vector<string> inOrder;

    for (int x = 0; x < this->arraySize; x++)
    {
        if (dictArray[x] != nullptr)
            inOrder.push_back(*dictArray[x]);
    }

    sort(inOrder.begin(), inOrder.end());
    return inOrder;
}
//print functions:
void Dictionary::printDictionaryInOrder(ostream& outputStream) const
{
    string endOfLine = "\n";
    vector<string> data = getInOrder();
    for (int x = 0; x < data.size(); x++)
    {
        outputStream<<data[x]<<endOfLine;
    }
}
void Dictionary::printDictionaryKeys(ostream& outputStream) const
{
    string endOfLine = "\n";
    for (int x = 0; x < this->arraySize; x++)
    {
        if (dictArray[x] != nullptr)
            outputStream<<x<<": "<<*(this->dictArray[x])<<endOfLine;
        else
            outputStream<<x<<": "<<endOfLine;

    }
}

//basic operations helpers
//helper function for copy
void Dictionary::copy(const Dictionary& orig)
{
    dataNum = orig.dataNum;
    arraySize = orig.arraySize;
    hashCount = orig.hashCount;
    dictArray = new string*[arraySize]{};

    for (int x = 0; x < this->arraySize; x++)
    {
        if (orig.dictArray[x] != nullptr)
            dictArray[x] = new string(*(orig.dictArray[x]));
    }
}
//emptys hash table
void Dictionary::clear()
{
    for (int x = 0; x < this->arraySize; x++)
    {
        if (dictArray[x] != nullptr)
        {
            delete dictArray[x];
            dictArray[x] = nullptr;
        }
    }
    delete[] dictArray;
    dictArray = nullptr;
    dataNum = 0;
}
