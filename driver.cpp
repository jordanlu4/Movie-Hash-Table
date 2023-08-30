#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MovieHashTable.hpp"
#include "DirectorSkipList.hpp"

using namespace std;

// Function prototypes
MovieNode *parseMovieLine(string line);
void readMovieCSV(string filename, MovieHashTable &movieTable, DirectorSkipList &directorList);
void display_menu();

// Main function
int main(int argc, char *argv[])
{
    // Check command line arguments
    if (argc != 4)
    {
        cout << "Usage: " << argv[0] << " <csv_file> <hashtable_size> <skiplist_size>" << endl;
        return 1;
    }

    // Parse command line arguments
    string csv_file = argv[1];
    int hashtable_size = stoi(argv[2]);
    int skiplist_size = stoi(argv[3]);

    // Create MovieHashTable and DirectorSkipList objects
    MovieHashTable movieTable(hashtable_size);
    DirectorSkipList directorList(skiplist_size, 10);
    // cout << "HI" << endl;
    // Read CSV file and populate data structures
    readMovieCSV(csv_file, movieTable, directorList);

    // Test if data was successfully inserted into the data structures
    // DirectorSLNode* movie = directorList.search("Eli Roth");
    // cout << movie << endl;
    // directorList.prettyPrint();
    // if (movie != nullptr) {
    //     cout << "Found director: " << movie->director << endl;
    //         } else {
    //     cout << "Director not found" << endl;
    // }
    // cout << movieTable.getCollisions();
    // cout << movieTable.hash("Avengers") << endl;
    // cout << movieTable.getCollisions() << endl;
    while (true){
    int choice;
    cout << "Number of collisions: " << movieTable.getCollisions() << endl;
    display_menu();
    cout << "Enter an option: ";
    cin >> choice;
    string movie_name = "";
    string director_name = "";
    MovieNode *search;
    DirectorSLNode *director_node;
    string name2 = "";
    MovieNode* movie;
    int counter = 0;
    switch (choice)
    {
    case 1:

        cout << "Enter movie name: ";
        cin.ignore(); // Ignore the newline character from previous input
        getline(cin, movie_name);
        search = movieTable.search(movie_name);
        if (search != nullptr)
        {
            cout << "The director of " << search->title << " is " << search->director << endl;
        }
        else
        {
            cout << "Movie not found" << endl;
        }
        break;
    case 2:

        cout << "Enter director name: ";
        cin.ignore(); // Ignore the newline character from previous input
        getline(cin, director_name);
        director_node = directorList.search(director_name);
        // cout << director_node << endl;
        if (director_node == nullptr)
        {
            cout << "Director not found." << endl;
            break;
        }
        for (auto movie : director_node->movies)
        {
            counter++;
        }
        cout << director_node->director << " has directed directed " << counter << " movies" << endl;

        break;
    case 3:
        // If the movie is found, print its details
 
    cout << "Enter movie name: ";
    cin.ignore();
    getline(cin, name2);

    // Search for the movie in the hash table
    movie = movieTable.search(name2);
    if (movie != nullptr) {
        cout << "Summary: " << movie->title << " is a " << movie->year << " (" << movie->genre << ") film featuring \"" << movie->actors << "\"." << endl;
        cout << "Plot: \"" << movie->description << "\"" << endl;
    } else {
        cout << "Movie not found" << endl;
    }
        break;
    case 4:

        cout << "Enter director name: ";
        cin.ignore(); // Ignore the newline character from previous input
        getline(cin, director_name);
        director_node = directorList.search(director_name);
        if (director_node == nullptr)
        {
            cout << "Director not found." << endl;
            break;
        }

        cout << director_node->director << " directed " << director_node->movies.size() << " movies:" << endl;
        for (auto movie : director_node->movies)
        {
            cout << movie->title << " (" << movie->year << ")" << endl;
        }
        break;
    case 5:
        cout << "Goodbye!" << endl;
        return 0;
    default:
        cout << "Invalid option. Please try again." << endl;
    }
}
    return 0;
}

// Function to parse a CSV line into a MovieNode object
MovieNode *parseMovieLine(string line)
{
    stringstream ss(line);
    vector<string> fields;
    string field;

    // Loop through the characters in the line
    bool in_quotes = false;
    for (size_t i = 0; i < line.length(); ++i)
    {
        char c = line[i];
        if (c == '\"')
        {
            in_quotes = !in_quotes;
        }
        else if (c == ',' && !in_quotes)
        {
            // add field to vector and reset for next field
            fields.push_back(field);
            field.clear();
        }
        else
        {
            field += c;
            // handle double quotes inside a quoted field
            if (in_quotes && c == '\"' && i < line.length() - 1 && line[i + 1] == '\"')
            {
                field += '\"'; // add the second double quote and skip it
                ++i;
            }
        }
    }
    fields.push_back(field);

    if (fields.size() != 12)
    {
        cerr << "Error: Invalid movie line format" << line << endl;
        return nullptr;
    }

    int rank = stoi(fields[0]);
    string title = fields[1];
    string genre = fields[2];
    string description = fields[3];
    string director = fields[4];
    string actors = fields[5];
    int year = stoi(fields[6]);
    int runtime = stoi(fields[7]);
    float rating = stof(fields[8]);
    int votes = stoi(fields[9]);
    float revenue = stof(fields[10]);
    int metascore = stoi(fields[11]);

    // Create a new MovieNode object with the parsed fields
    MovieNode *movie = new MovieNode(rank, title, genre, description, director, actors, year, runtime, rating, votes, revenue, metascore);
    return movie;
}

// Function to read a CSV file into a vector of MovieNode objects
void readMovieCSV(string filename, MovieHashTable &movieTable, DirectorSkipList &directorList)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Failed to open file " << filename << endl;
        return;
    }

    // Vector to store parsed MovieNode objects

    // Read file line by line
    string line;
    getline(file, line); // skip first line
    while (getline(file, line) && !file.eof())
    {
        // cout << line << endl;
        // Parse line into MovieNode object
        //   cout << "test 1" << endl;
        MovieNode *movie = parseMovieLine(line);
        //   cout << "test 2" << endl;
        if (movie != nullptr)
        {
            // Insert movie into MovieHashTable
            movieTable.insert(movie->title, movie);
            //   cout << "test 3" << endl;

            // Insert director into DirectorSkipList
            // cout << movie->director << endl;
            directorList.insert(movie->director, movie);
            // cout << "hi" << endl;
        }
    }

    // Close the file
    file.close();
}
// Function to display the menu options

void display_menu()
{
    MovieHashTable movie = MovieHashTable();
    DirectorSkipList SL = DirectorSkipList();
    cout << "Please select an option:" << endl;
    cout << "1. Find the director of a movie" << endl;
    cout << "2. Find the number of movies by a director" << endl;
    cout << "3. Find the description of a movie" << endl;
    cout << "4. List the movies by a director" << endl;
    cout << "5. Quit" << endl;
    // cout << "Enter an option: ";
    // string movie_name = "";
    // MovieNode* search;
    // switch (option) {
    //     case 1:
    //         // TODO: implement "Find the director of a movie"
    //         cout << "Enter movie name: ";
    //         cin.ignore(); // Ignore the newline character from previous input
    //         getline(cin, movie_name);
    //         search = movie.search(movie_name);
    //         if (search != nullptr) {
    //             cout << "The director of " << search->title << " is " << search->director << endl;
    //         } else {
    //             cout << "Movie not found" << endl;
    //         }
    //         break;
    //     case 2:
    //         // TODO: implement "Find the number of movies by a director"
    //         cout << "You chose option 2" << endl;
    //         break;
    //     case 3:
    //         // TODO: implement "Find the description of a movie"
    //         cout << "You chose option 3" << endl;
    //         break;
    //     case 4:
    //         // TODO: implement "List the movies by a director"
    //         cout << "You chose option 4" << endl;
    //         break;
    //     case 5:
    //         cout << "Goodbye!" << endl;
    //         return;
    //     default:
    //         cout << "Invalid option. Please try again." << endl;
    // }
}
