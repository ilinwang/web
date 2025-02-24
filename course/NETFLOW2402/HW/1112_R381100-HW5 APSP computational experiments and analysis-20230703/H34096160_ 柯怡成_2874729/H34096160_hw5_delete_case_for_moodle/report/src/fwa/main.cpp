/*
Purpose: Find the shortest path (all to all)
Method: directly execute the exe file or compile main.cpp in UNIX environment, and execute the compiled execution file.
        Or use the batch file
Input: SP file path, output file path
Output: total times of comparison and execute time.
Compilation: Compile main.cpp directly, such as: g++ -o fwa.exe main.cpp
Pseudocode: as shown below
the code can run
Mason Ke Finished 2323/06/10 22:35
*/
/*psuedocode
Read file as Matrix
Initialize
for k in n
    for i in n
        for j in n
            if (Matrix[i][k] + Matrix[k][j] < Matrix[i][j]){
                Matrix[i][j] =Matrix[i][k] + Matrix[k][j]
            }
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <sys/types.h>
#include <sys/times.h>

using namespace std;

const int INF = numeric_limits<int>::max(); // Used to represent an infinite distance

float timer(){
    struct tms hold;
    times(&hold);
    return  (float)(hold.tms_utime) / 60.0;
}

int floydWarshall(int **&Graph_Adj_Matrix, int num_nodes) {

    // Initialize the distance matrix
    int **Distance_Matrix = new int*[num_nodes + 1];
    for (int i = 1; i <= num_nodes; i++){
        Distance_Matrix[i] = new int[num_nodes + 1];
        for (int j = 1; j <= num_nodes; j++){
            if (Graph_Adj_Matrix[i][j] != -1)
                Distance_Matrix[i][j] = Graph_Adj_Matrix[i][j]; // Update the distance matrix according to the edges of the graph
            else
                Distance_Matrix[i][j] = INF;
        }
        Distance_Matrix[i][i] = 0; // Initialize the distance on the diagonal to 0
    }

    // Perform the main operations of the Floyd-Warshall algorithm
    long Counter = 0; // Calculate the effective number of comparisons
    for (int k = 1; k <= num_nodes; ++k) {
        for (int i = 1; i <= num_nodes; ++i) {
            for (int j = 1; j <= num_nodes; ++j) {
                if (Distance_Matrix[i][k] != INF && Distance_Matrix[k][j] != INF){
                    Counter++;
                    if (Distance_Matrix[i][k] + Distance_Matrix[k][j] < Distance_Matrix[i][j]){
                        Distance_Matrix[i][j] = Distance_Matrix[i][k] + Distance_Matrix[k][j];
                    }
                }
            }
        }
    }

    return Counter;
}

int main(int argc, char* argv[]) {
    if (argc < 3) { // Ensure the number of parameters
        cout << "please input input/output file path as arguement" << endl;
        return 1;
    }

    //declare variable
    int **Graph_Adj_Matrix;
    int num_nodes;
    string input_file_path = argv[1]; // read input path
    string output_file_path = argv[2]; // read output path

    ifstream input_file(input_file_path); // read file
    string problem_name, problem_type, nodes_str, arcs_str;
    if (input_file.is_open()) {
        string line, c;
        while (getline(input_file, line)) {
            if (!line.empty()){// ignore empty lines
                stringstream lineSplitIss(line);
                lineSplitIss >> c;
                switch (c[0]){
                case 'c':
                    continue;// ignore comment lines
                    break;
                case 't':
                    problem_name = line.substr(2);// read question name (will not be used)
                    break;
                case 'p':
                    lineSplitIss >> problem_type >> nodes_str >> arcs_str;// Read question type, #node, #arc (will not be used)
                    break;
                case 'n':
                    // read #node
                    lineSplitIss >> num_nodes;
                    // Initialize the adj matrix of the graph
                    Graph_Adj_Matrix = new int*[num_nodes + 1];
                    for (int i = 1; i <= num_nodes; i++){
                        Graph_Adj_Matrix[i] = new int[num_nodes + 1];
                        for (int j = 1; j <= num_nodes; j++){
                            Graph_Adj_Matrix[i][j] = -1;
                        }
                    }
                    break;
                case 'a':
                    // Read the start, end and weight of an edge
                    int source, sink, cost;
                    lineSplitIss >> source >> sink >> cost;
                    // add edges to the adjacency matrix representation of the graph
                    Graph_Adj_Matrix[source][sink] = cost;
                    break;
                default:
                    break;
                }
            }
        }
        input_file.close();
    }
    else {
        cout << "input error" << endl;
        return 1;
    }

    // write the result to file
    ofstream output_file(output_file_path);
    if (output_file.is_open()) {
        float start = timer();
        output_file << "Number of scans: " << floydWarshall(Graph_Adj_Matrix, num_nodes) << "\n"; // °õ¦æFloyd-Warshallºâªk
        output_file << "Running time of SP computation: " << timer()-start;
        output_file.close();
        cout << "\noutput success" << endl;
    } else {
        cout << "\noutput error" << endl;
    }

    // free memory
    for (int i = 1; i <= num_nodes; i++) {
        delete[] Graph_Adj_Matrix[i];
    }
    delete[] Graph_Adj_Matrix;

    return 0;
}
