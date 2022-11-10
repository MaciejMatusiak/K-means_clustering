#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

struct Point
{
    double x, y;    // coordinates
    int cluster;    // no default cluster
    double minDist; // default infinite distance to nearest cluster

    Point() : x(0.0), y(0.0), cluster(-1), minDist(__DBL_MAX__) {} // initialisation list
    Point(double x, double y) : x(x), y(y), cluster(-1), minDist(__DBL_MAX__) {}

    /*
         Computes the (square) euclidean distance between this point and another
    */
    double distance(Point p)
    {
        return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
    }
};

vector<Point> readcsv()
{
    /*
     * Reads in the data.csv file into a vector of points
     * @return vector of points
     *
     */
    vector<Point> points;
    string line;
    ifstream file("input.csv");

    if (file.good() == true) // checking access to file "input.csv"
    {
        cout << "File accessed!" << endl;
    }
    else
        cout << "No file named 'input.csv' was detected. Please try again" << endl;

    while (getline(file, line)) // load data to vector "Point"
    {
        stringstream lineStream(line);
        string bit;
        double x, y;
        getline(lineStream, bit, ',');
        x = stof(bit); // convert string into float (string (stof) into double)
        getline(lineStream, bit, '\n');
        y = stof(bit);

        points.push_back(Point(x, y));
    }
    return points;
}

void kMeansClustering(vector<Point> *points, int epochs, int k)
{
    int n = points->size();

    /*
    points - pointer to vector of points
        epochs - number of k means iterations
    k - the number of initial centroids
        Randomly initialise centroids
    The index of the centroid within the centroids vector represents the cluster label.
    */

    vector<Point> centroids;
    srand(time(0));
    for (int i = 0; i < k; ++i)
    {
        centroids.push_back(points->at(rand() % n)); // change elements in Vector centroids from point
    }

    for (int i = 0; i < epochs; ++i)
    {
        // For each centroid, compute distance from centroid to each point
        // and update point's cluster if necessary
        for (vector<Point>::iterator c = begin(centroids); c != end(centroids); ++c) // declarate iterator
        {
            int clusterId = c - begin(centroids);

            for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it)
            {
                Point p = *it;
                double dist = c->distance(p);
                if (dist < p.minDist)
                {
                    p.minDist = dist;
                    p.cluster = clusterId;
                }
                *it = p;
            }
        }

        // Create vectors to keep track of data needed to compute means
        vector<int> nPoints;
        vector<double> sumX, sumY;
        for (int j = 0; j < k; ++j)
        {
            nPoints.push_back(0);
            sumX.push_back(0.0);
            sumY.push_back(0.0);
        }

        // Iterate over points to append data to centroids
        for (vector<Point>::iterator it = points->begin(); it != points->end();
             ++it)
        {
            int clusterId = it->cluster;
            nPoints[clusterId] += 1;
            sumX[clusterId] += it->x;
            sumY[clusterId] += it->y;

            it->minDist = __DBL_MAX__; // reset distance
        }
        // Compute the new centroids
        for (vector<Point>::iterator c = begin(centroids); c != end(centroids);
             ++c)
        {
            int clusterId = c - begin(centroids);
            c->x = sumX[clusterId] / nPoints[clusterId];
            c->y = sumY[clusterId] / nPoints[clusterId];
        }
    }

    // Write to output.csv file
    ofstream myfile;
    myfile.open("output.csv");
    myfile << "x,y  cluster" << endl;

    for (vector<Point>::iterator it = points->begin(); it != points->end();
         ++it)
    {
        myfile << it->x << "," << it->y << "    " << it->cluster << endl;
    }
    myfile.close();
}

void Display_menu()
{
    cout << "             Welcome to the program calculating centroids.              " << endl;
    cout << "This program can assign centroids for points in the Cartesian system." << endl;
    cout << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "1. Load data from file include in program." << endl;
    cout << "2. Exit from program." << endl;
    cout << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "Make your choice  =>";
}

int main()
{
    vector<Point> points = readcsv();

    Display_menu();
    for (int i = 0; i < 4; i++)
    {
        char Option = ' ';
        cin >> Option;
        uint16_t end = 0;

        switch (Option)

        {
        case 49: // load database
            system("cls");
            kMeansClustering(&points, 40, 5); // kMeansClustering(vector<Point>* points, int epochs, int k)
            cout << "Centroids and numbers of clusters to which they belong are in the file 'output.csv'." << endl;
            end = 58;
            break;

        case 50:
            cout << "\nProgram Closed" << endl;
            end = 58;
            exit(0);

        default:
            cout << "An error has occurred." << endl;
            cout << "Please, select another option" << endl;
            break;
        }

        if (end)
            break;
    }
    return 0;
}
