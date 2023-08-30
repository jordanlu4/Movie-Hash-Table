#include<iostream>
#include <vector>
#include "MovieHashTable.hpp"
using namespace std;

// Constructor for MovieHashTable with default size
MovieHashTable::MovieHashTable() {
    // table_size = 1000;
    // n_collisions = 0;
    // MovieNode* hashFunc = new MovieNode[table_size];
    table_size = 1000; // or any other default value as desired
    n_collisions = 0; // Initialize the number of collisions to zero

    // Allocate memory for the hash table as an array of pointers to MovieNode objects
    table = new MovieNode*[table_size];

    // Initialize all pointers in the hash table to nullptr
    for (int i = 0; i < table_size; i++) {
        table[i] = nullptr;
    }
    
}

// Constructor for MovieHashTable with given size
MovieHashTable::MovieHashTable(int s) {
    table_size = s;
    n_collisions = 0; 
    table = new MovieNode*[table_size];
    for (int i = 0; i < table_size; i++) {
        table[i] = nullptr;
    }
}

// Destructor for MovieHashTable that deletes all nodes in the hash table
MovieHashTable::~MovieHashTable() {
    for (int i = 0; i < table_size; i++) {
        MovieNode* current = table[i];
        while (current != nullptr) {
            MovieNode* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] table;
}

// Hash function for MovieHashTable that returns an index in the hash table for a given movie title.

int MovieHashTable::hash(string title) {
    int sum = 0;
    int len = title.length();

    for (int i = 0; i < len; i++) {
        sum += static_cast<int>(title[i]);
    }

    string key = "jolu4523"; // use a random combination of letters and numbers for the key
    int len2 = key.length();

    for (int i = 0; i < len2; i++)
    {
        sum += static_cast<int>(key[i]);
    }


    // return sum;
    return sum % table_size; 
}

// Inserts a movie node into the hash table with the specified title
void MovieHashTable::insert(string title, MovieNode* movie) {
    int index = hash(title);
        
    // MovieNode* newNode = new MovieNode(movie->rank, movie->title, movie->genre, movie->description,
    //                                    movie->director, movie->actors, movie->year, movie->runtime,
    //                                    movie->rating, movie->votes, movie->revenue, movie->metascore);

    MovieNode* existingMovie = search(title);
    // cout << movie->title << endl;
   // if the movie alreadt exists
    if (existingMovie != nullptr) {
        // cout << title << " already exists in the hash table." << endl;
        return;
    }

    
    if (table[index] == nullptr) {
    table[index] = movie;
    return;
    }
        // default     // If there is already a node at the slot
    else {
        MovieNode* head = table[index];
        table[index] = movie;
        movie->next = head;
        setCollisions();
        // cout << getCollisions() << endl;
    }
    return;
}

// Searches for a node in the hash table with the specified title
MovieNode* MovieHashTable::search(string title) {
    //Compute the index by using the hash function
    int index = hash(title);

    if (table[index] == nullptr)
    {
        return nullptr;
    }
    //create node with title
    MovieNode* search = table[index]; //if it exsits, it will be in the table[index] list
    while (search != nullptr)
    {
        if (search->title == title)
        {
            return search;
        }
        search = search->next;
    }
    return nullptr;

}

// Returns the number of collisions that have occurred during insertion into the hash table
int MovieHashTable::getCollisions() {
    // TODO
    return n_collisions;
}

// Increments the number of collisions that have occurred during insertion into the hash table
void MovieHashTable::setCollisions() {
    n_collisions++;
}
