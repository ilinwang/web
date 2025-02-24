/*
Purpose: Find the shortest path (all to all)
Method: directly execute the exe file or compile main.cpp in UNIX environment, and execute the compiled execution file.
        Or use the batch file
Input: SP file path, output file path
Output: total times of comparison and execute time.
Compilation: Compile main.cpp directly, such as: g++ -o fwg.exe main.cpp
Pseudocode: as shown below
the code can run
Mason Ke Finished 2323/06/10 22:38
*/
/*psuedocode
Read file as Matrix
Construct List
Initialize
for k in n
    for i in n
        for j in n
            if (Matrix[k][temp_sink] != INF && Matrix[temp_src][k] != INF){
                Matrix[temp_src][temp_sink] = Matrix[temp_src][k] + Matrix[k][temp_sink];
                From_List[temp_src][From_End[temp_src]] = temp_sink;
                To_List[temp_sink][To_End[temp_sink]] = temp_src;
                From_End[temp_src]++;
                To_End[temp_sink]++;
            }
            else if (Matrix[i][k] + Matrix[k][j] < Matrix[i][j]){
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

int main(int argc, char* argv[]) {
    if (argc < 3) { // Ensure the number of parameters
        cout << "please input input/output file path as arguement" << endl;
        return 1;
    }

    //declare variable
    int **Graph_Adj_Matrix;
    int **From_List;
    int **To_List;
    int *From_End;
    int *To_End;
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
                    // Initialize the adj matrix and adj list of the graph
                    Graph_Adj_Matrix = new int*[num_nodes + 1];
                    From_List = new int*[num_nodes + 1];
                    To_List = new int*[num_nodes + 1];
                    From_End = new int[num_nodes + 1];
                    To_End = new int[num_nodes + 1];
                    for (int i = 1; i <= num_nodes; i++){
                        Graph_Adj_Matrix[i] = new int[num_nodes + 1];
                        From_List[i] = new int[num_nodes + 1];
                        To_List[i] = new int[num_nodes + 1];
                        From_End[i] = 1;
                        To_End[i] = 1;
                        for (int j = 1; j <= num_nodes; j++){
                            Graph_Adj_Matrix[i][j] = INF;
                            From_List[i][j] = -1;
                            To_List[i][j] = -1;
                        }
                        Graph_Adj_Matrix[i][i] = 0; // Initialize the distance on the diagonal to 0
                    }
                    break;
                case 'a':
                    // Read the start, end and weight of an edge
                    int source, sink, cost;
                    lineSplitIss >> source >> sink >> cost;
                    // add edges to the adjacency list representation of the graph
                    Graph_Adj_Matrix[source][sink] = cost; // Update the distance matrix according to the edges of the graph
                    From_List[source][From_End[source]] = sink; // from adj list
                    To_List[sink][To_End[sink]] = source; // to adj list
                    From_End[source]++;
                    To_End[sink]++;
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

    float start = timer();
    // 進行Floyd-Warshall算法的主要運算
    long Counter = 0; // 計算有效比較次數
    int temp_src, temp_sink;
    for (int k = 1; k <= num_nodes; k++){
        for (int i = 1; i < min(From_End[k], num_nodes); i++){
            for (int j = 1; j < min(To_End[k], num_nodes); j++){
                if (Graph_Adj_Matrix[k][temp_sink] != INF && Graph_Adj_Matrix[temp_src][k] != INF){
                    Counter++; // Calculate the effective number of comparisons
                    temp_src = To_List[k][j];
                    temp_sink = From_List[k][i];
                    if (Graph_Adj_Matrix[temp_src][temp_sink] == INF){ // If there is no value (INF), create a new node in the list
                        Graph_Adj_Matrix[temp_src][temp_sink] = Graph_Adj_Matrix[temp_src][k] + Graph_Adj_Matrix[k][temp_sink];
                        From_List[temp_src][From_End[temp_src]] = temp_sink;
                        To_List[temp_sink][To_End[temp_sink]] = temp_src;
                        From_End[temp_src]++;
                        To_End[temp_sink]++;
                    }
                    else if (Graph_Adj_Matrix[k][temp_sink] + Graph_Adj_Matrix[temp_src][k] < Graph_Adj_Matrix[temp_src][temp_sink]){ // comparison
                        Graph_Adj_Matrix[temp_src][temp_sink] = Graph_Adj_Matrix[k][temp_sink] + Graph_Adj_Matrix[temp_src][k];
                    }
                }
            }
        }
    }

    // write the result to file
    ofstream output_file(output_file_path);
    if (output_file.is_open()) {
        output_file << "Number of scans: " << Counter << "\n";
        output_file << "Running time of SP computation: " << timer()-start;
        output_file.close();
        cout << "\noutput success" << endl;
    } else {
        cout << "\noutput error" << endl;
    }

    // free memory
    for (int i = 1; i <= num_nodes; i++) {
        delete[] Graph_Adj_Matrix[i];
        delete[] From_List[i];
        delete[] To_List[i];
    }
    delete[] Graph_Adj_Matrix;
    delete[] From_List;
    delete[] To_List;
    delete[] From_End;
    delete[] To_End;

    return 0;
}
