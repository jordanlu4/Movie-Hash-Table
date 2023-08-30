#include <iostream>
#include <vector>
#include "DirectorSkipList.hpp"
using namespace std;

// Constructor for DirectorSkipList with default capacity and levels
DirectorSkipList::DirectorSkipList()
{
    capacity = 2000;
    levels = 10;
    head = new DirectorSLNode("noname", levels);
    size = 0;
}

// Constructor for DirectorSkipList with given capacity and levels
DirectorSkipList::DirectorSkipList(int _cap, int _levels)
{
    capacity = _cap;
    levels = _levels;
    head = new DirectorSLNode("noname", levels);
    size = 0;
}

// Destructor for DirectorSkipList that deletes all nodes in the skip list (but not the MovieNodes, which are shared with other data structures)
DirectorSkipList::~DirectorSkipList()
{
    DirectorSLNode *curr = head;
    while (curr != nullptr)
    {
        DirectorSLNode *temp = curr;
        curr = curr->next[0];
        delete temp;
    }
}

// Inserts a movie node into the skip list with the specified director
void DirectorSkipList::insert(string director, MovieNode *_movie)
{
    DirectorSLNode *existingNode = search(director); // check if it already exists
    if (existingNode != nullptr)
    {
        existingNode->addMovie(_movie);
        return;
    }

    // create a new node with the specified director and a random number of levels
    int newLevels = randomLevel();
    DirectorSLNode *newNode = new DirectorSLNode(director, 10);
    newNode->addMovie(_movie);

    // traverse the skip list from the top down, keeping track of the last node visited on each level
    vector<DirectorSLNode *> prev(levels, nullptr); // vector to keep track of previoos nodes
    DirectorSLNode *curr = head;
    for (int i = levels - 1; i >= 0; i--)
    {
        while (curr->next[i] != nullptr && curr->next[i]->director < director)
        {
            curr = curr->next[i];
        }
        prev[i] = curr;
    }

    // update the next pointers for the new node and the previous node on each level
    for (int i = 0; i < newLevels; i++)
    {
        if (i < levels)
        {
            newNode->next[i] = prev[i]->next[i];
            prev[i]->next[i] = newNode;
        }
        else
        {
            head->next.push_back(newNode);
            newNode->next.push_back(nullptr);
            levels++;
            // } else {
            //     // if the new node has more levels than any previous node, update the head node
            //     head->next.push_back(newNode);
            //     newNode->next.push_back(nullptr);
            //     levels++;
            // }
        }
        if (prev[i] != nullptr)
        {
            prev[i]->next[i] = newNode;
        }
    }

    size++;

    //     DirectorSLNode* curr = head;
    //     vector <DirectorSLNode*> prev;

    //     int n_levels = 1;

    //     while (n_levels < levels && rand()%2 == 0)
    //     {
    //         n_levels++;
    //     }

    //    for (int i = levels-1; i >= 0; i--)
    //     {
    //         while(curr->next[i] != nullptr && curr->next[i]->director < director)
    //         {
    //             curr = curr->next[i];
    //         }
    //         prev[i] = curr;
    //     }
    //     // cout << "curr: " << curr << endl;
    //     cout << "test" << endl;
    //     curr = curr->next[0];
    //     cout << "test 2" << endl;
    //     if (curr != nullptr && curr->director == director)
    //     {
    //         curr->addMovie(_movie);
    //     }

    //     else
    //     {
    //         DirectorSLNode* temp = new DirectorSLNode;
    //         for (int i = n_levels - 1; i >= 0; i--)
    //         {
    //             temp->next[i] = prev[i]->next[i];
    //             prev[i]->next[i] = temp;
    //         }
    //         temp->addMovie(_movie);
    //         size++;
    //     }
}

// Searches for a node in the skip list with the specified director
DirectorSLNode *DirectorSkipList::search(string director)
{
    DirectorSLNode *curr = head;
    for (int i = levels - 1; i >= 0; i--)
    {
        while (curr->next[i] != nullptr && curr->next[i]->director < director)
        {
            curr = curr->next[i];
        }
        if (curr->next[i] != nullptr && curr->next[i]->director == director)
        {
            return curr->next[i];
        }
    }
    return nullptr;
    // DirectorSLNode* curr = head;
    // for (int i = levels - 1; i >= 0; i--)
    // {
    //     while (curr->next[i] != nullptr && curr->next[i]->director < director)
    //     {
    //         curr = curr->next[i];
    //     }
    // }
    // curr = curr->next[0];

    // if (curr != nullptr && curr->director == director)
    // {
    //     return curr;
    // }
    // else
    // {
    //     return nullptr;
    // }
}

// Pretty-prints the skip list
void DirectorSkipList::prettyPrint()
{
    // Traverse the skip list from top to bottom
    // cout << "hi " << endl;
    for (int i = levels - 1; i >= 0; i--)
    {
        DirectorSLNode *curr = head->next[i];
        cout << "Level " << i << ": ";
        while (curr != nullptr)
        {
            cout << curr->director << " -> ";
            curr = curr->next[i];
        }
        cout << "NULL" << endl;
    }
}

// Generates a random level for a new node
int DirectorSkipList::randomLevel()
{
    int level = 1;
    while (rand() % 2 == 0 && level < levels)
    {
        level++;
    }
    return level;
}