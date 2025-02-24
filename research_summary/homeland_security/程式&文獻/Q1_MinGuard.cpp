 /*
	find the minimum number of guards to cover whole network(or only trains)

*/
#include "gurobi_c++.h"
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<sstream>
#include<climits>
#include<map>
#include<cmath>
#include<vector>
#include<ctime>
#include<utility>
using namespace std;
map<string, GRBVar> var;
GRBEnv *env = new GRBEnv();
GRBModel *model = new GRBModel(*env);
int trackN = 1;
class Node{
    public:
        Node(){
            stopName = " ";
            trainName = " ";
            stopIndex = 0;
            Time = 0;
            pass = false;
            add = false;
        }

        void setNode(string stopName, string trainName, int Time, bool pass, bool add){

            this->stopName = stopName;
            this->trainName = trainName;
            this->Time = Time;
            this->pass = pass;
            this->add = add;
        }

        string getstopName(){
            return stopName;
        }
        string gettrainName(){
            return trainName;
        }
        int getTime(){
            return Time;
        }
        bool getpass(){
            return pass;
        }
        bool getadd(){
            return add;
        }
    private:
        string stopName;
        string trainName;
        int stopIndex;
        int Time;
        bool pass;
        bool add;
};
class Arc{
    public:
        Arc(){
            from = make_pair(0,0);
            to = make_pair(0,0);
            weight = 0.0;
        }

        void setArc(pair<int, int> from, pair<int, int> to, double weight){
            this->from = from;
            this->to = to;
            this->weight = weight;
        }

        int getFromFirst(){
            return from.first;
        }

        int getToFirst(){
            return to.first;
        }
        int getFromSecond(){
            return from.second;
        }

        int getToSecond(){
            return to.second;
        }

        double getWeight(){
            return weight;
        }

    private:
        pair<int, int> from;
        pair<int, int> to;
        double weight;
};
struct SameNode{
    int nowIndex;
    int targetTrack;
    int targetIndex;
    int targetNum;
};
vector<vector<Node> > node;
vector<vector<Arc> > arc;
vector<Arc> crossArc;
//Node **node;
//Arc **arc;
//Arc *crossArc;
vector<string> crossNode;
int **addStopNum;
int **addStopIndex;
int *nodeNum = new int [trackN];// each track has how many nodes
int *arcNum = new int [trackN];
int crossArcNum = 0;
vector<vector<SameNode> > sameNode;
vector<vector<vector<int> > > stopNode;// this stop contains which node
vector<vector<vector<int> > > trainNode;//this train contains which node
vector<vector<vector<int> > > NodeFrom; // pair (mrt,stop)
vector<vector<vector<int> > > NodeTo;
vector<vector<vector<int> > > CrossNodeFrom; // pair (mrt,stop)
vector<vector<vector<int> > > CrossNodeTo;
void read(vector<vector<int> >&, vector<vector<string> >&, vector<vector<string> >&, vector<vector<int> >&,  vector<vector<pair<int, int> > >& , int);
void initialize(vector<vector<int> >&, vector<vector<string> >&, vector<vector<string> >&, vector<vector<int> >&,  vector<vector<pair<int, int> > >&);
void nodeDetail(vector<vector<int> >&, vector<vector<string> >&, vector<vector<string> >&, int*&, int);
void cross(vector<vector<int> >&, vector<vector<string> >&, vector<vector<string> >&, int*&, int, int&);
void arcDetail(vector<vector<string> >&, vector<vector<string> >&, int);
void gurobi();
int main(){
    vector<vector<int> > timeTable;
    vector<vector<string> > stopName;
    vector<vector<string> > trainName;
    vector<vector<int> > crossTrack;// this track have which cross stop
    vector<vector<pair<int, int> > > crossNodeDetail;//this cross stop belongs to which track and which stopindex

    fstream file;
    file.open("crossnode.txt");
    string tmp, token;
    if(file.is_open()==1){
        getline(file,tmp);
        stringstream ss(tmp);
             while(getline(ss,token,',')){
            crossNode.push_back(token);
        }

    }
    else
        cout<<"open crossnode error"<<endl;
    /*for(int i=0; i<crossNode.size(); i++){
    	cout<<crossNode[i]<<" ";
    }*/

    for(int i=0; i<trackN; i++){
        vector<int> tmp;
        vector<string> t;
        vector<SameNode> s;
        timeTable.push_back(tmp);
        stopName.push_back(t);
        trainName.push_back(t);
        crossTrack.push_back(tmp);
        sameNode.push_back(s);
    }
    for(int i=0; i<crossNode.size(); i++){
        vector<pair<int,int> > p;
        crossNodeDetail.push_back(p);
    }

    addStopNum = new int* [trackN];
    addStopIndex = new int* [trackN];
    for(int i=0; i<trackN; i++){
        addStopNum[i] = new int [crossNode.size()];
        addStopIndex[i] = new int [crossNode.size()];
        for(int j=0; j<crossNode.size(); j++){
            addStopNum[i][j] = 0;// count number
            addStopIndex[i][j] = -1;// record index, so can't use 0
        }
    }

    for(int n = 0; n<trackN; n++)
        read(timeTable, stopName, trainName, crossTrack, crossNodeDetail, n);

    /*for(int i=0; i<trackN; i++){
        cout<<"mrt: "<<i<<endl;
        for(int j=0; j<stopName[i].size();j++){
        	cout<<stopName[i][j]<<" ";
        }
        cout<<endl;
    }*/
    initialize(timeTable, stopName, trainName, crossTrack, crossNodeDetail);

   /* for(int i=0; i<trackN; i++){
    	cout<<"mrt: "<<i<<endl;
    	for(int j=0; j<crossNode.size(); j++){
    		cout<<addStopIndex[i][j]<<" ";
    	}
    	cout<<endl;
    }*/
    for(int n = 0; n<trackN; n++)
        nodeDetail(timeTable, stopName, trainName, nodeNum, n);
    //cout<<"AAAAAaa"<<endl;
    /*for(int i=0; i<trackN; i++){
    	for(int j=0; j<node[i].size(); j++){
    		cout<<"mrt: "<<i<<" "<<j<<": time: "<<node[i][j].getTime()<<endl;
    	}
    }*/

    int whichArc = 0;
    for(int n = 0; n<trackN; n++)
    	cross(timeTable, stopName, trainName, nodeNum, n, whichArc);

    /*for(int i=0; i<crossArc.size();i++){
    	cout<<"arc"<<i<<": mrt: "<<crossArc[i].getFromFirst()<<" node: "<<crossArc[i].getFromSecond()<<" -> " <<"mrt: "<<crossArc[i].getToFirst()<<" node: "<<crossArc[i].getToSecond()<<endl;
    }*/
    for(int n = 0; n<trackN; n++)
        arcDetail(stopName, trainName, n);
    /*for(int i=0; i<trackN; i++){
    	cout<<"mrt: "<<i<<endl;
    	for(int j=0; j<arc[i].size(); j++){
    		cout<<"arc: "<<j<<" "<<arc[i][j].getFromSecond()<<" -> "<<arc[i][j].getToSecond()<<endl;
    	}
    }*/
    gurobi();
	system("pause");
}
void read(vector<vector<int> >& timeTable, vector<vector<string> >& stopName, vector<vector<string> >&trainName, vector<vector<int> >& crossTrack,  vector<vector<pair<int, int> > >& crossNodeDetail, int n){
    fstream file;
	string tmp, token;
	stringstream sfile;
	sfile<<"mrt"<<n+1<<".txt";
	string name = sfile.str();
	const char *filename = name.c_str();

	file.open(filename);
	if(file.is_open()){
         int c = 0;
        while(getline(file,tmp) ){ // each row
            c++;// row
            stringstream ss(tmp);
            int flag = 0;// unit
             int stop=0;

            while(getline(ss,token,',') ){ // each unit in row

                if(c == 1 && flag != 0){
                    stopName[n].push_back(token);
                    for(int i=0; i<crossNode.size(); i++){
                        if(token == crossNode[i]){
                            pair<int, int> p;// track/stopindex
                            p =  make_pair(n,stop);
                            crossNodeDetail[i].push_back(p);
                            crossTrack[n].push_back(i);
                            addStopIndex[n][i] = stop;
                            break;
                        }
                    }
                    stop++;
                    //cout<<token<<" ";
                }
                if(c != 1){
                	//cout<<token<<" ";
                    if(flag==0){
                    	trainName[n].push_back(token);
                    	  for(int i=0; i<crossNode.size(); i++){
                            if(addStopIndex[n][i] != -1){
                                 addStopNum[n][i]++;
                            }
                    	  }
                    	//cout<<"AAA "<<token<<endl;

                    }
                    else{ // time
                        int t=0;
                        t = atoi(token.c_str());
                    	timeTable[n].push_back(t);
                    }

                }
                flag++;
            }

        }

	}
	else
        cout<<"mrt open error"<<endl;
	file.close();

/*	for(int i=0; i<trackN; i++){
		for(int j=0; j<crossNode.size();j++){
			cout<<addStopIndex[i][j]<<" ";
		}
		cout<<endl;
	}
	for(int i=0; i<crossNode.size();i++)
        cout<<crossNode[i]<<" ";
    cout<<endl;
    for(int i=0; i<crossNodeDetail.size();i++){
        cout<<crossNode[i]<<" ";
        for(int j=0; j<crossNodeDetail[i].size(); j++){
            cout<<" ("<<crossNodeDetail[i][j].first<<", "<<crossNodeDetail[i][j].second<<") ";
        }
        cout<<endl;
    }
    cout<<"track"<<endl;
    for(int i=0; i<crossTrack.size(); i++){
        for(int j=0; j<crossTrack[i].size(); j++){
            cout<<crossTrack[i][j]<<" ";
        }
        cout<<endl;
    }
    for(int i=0; i<trackN; i++){
        cout<<"track: "<<i<<endl;
        for(int j=0; j<crossNode.size(); j++){
            cout<<addStopNum[i][j]<<" ";
        }
        cout<<endl;
    }
    */
}
void initialize(vector<vector<int> >& timeTable, vector<vector<string> >& stopName, vector<vector<string> >&trainName, vector<vector<int> >& crossTrack,  vector<vector<pair<int, int> > >& crossNodeDetail){
    for(int i=0; i<trackN; i++){// now
        int samestop = 0;
        for(int j=0; j<crossNode.size(); j++){
            if(addStopNum[i][j]!=0){
                for(int k=0; k<trackN; k++){ //target
                    if(i!=k && addStopNum[k][j]!=0){
                    	SameNode tmp;
                        samestop += addStopNum[k][j];
                        crossArcNum += addStopNum[k][j];
                        tmp.nowIndex = addStopIndex[i][j];
                        tmp.targetTrack = k;
                        tmp.targetIndex = addStopIndex[k][j];
                        tmp.targetNum = addStopNum[k][j];
                       	sameNode[i].push_back(tmp);

                    }

                }

            }
        }
        nodeNum[i] = stopName[i].size()*trainName[i].size() + 2*stopName[i].size() + samestop + 2;
        int saN = (trainName[i].size()+1)*stopName[i].size() + 2*stopName[i].size() + samestop;
        int taN = (stopName[i].size()-1)*trainName[i].size();
        arcNum[i] = saN + taN; //
    }
    /*int N = 0;
    for(int n=0; n<trackN; n++){
        N+= nodeNum[n];
    }
    cout<<N<<endl;*/
    for(int i=0; i<trackN; i++){
        vector<Node> n;
        vector<Arc> a;
        vector<int> e;
        node.push_back(n);
        arc.push_back(a);
        for(int j=0; j<nodeNum[i];j++){
            Node  tmp;
            node[i].push_back(tmp);
        }

        for(int j=0; j<arcNum[i];j++){
            Arc  tmp;
            int etmp;
            arc[i].push_back(tmp);
        }
    }
    for(int i=0; i<crossArcNum; i++){
        Arc tmp;
        crossArc.push_back(tmp);
    }

    //node = new Node* [trackN];
    //arc = new Arc* [trackN];
    //crossArc = new Arc [crossArcNum];

    /*for(int i=0; i<trackN; i++){
        node[i] = new Node [nodeNum[i]];
        arc[i] = new Arc [arcNum[i]];
    }*/

    for(int i=0; i<trackN; i++){
        vector<vector<int> > t;
        stopNode.push_back(t);
        NodeFrom.push_back(t);
        NodeTo.push_back(t);
        CrossNodeFrom.push_back(t);
        CrossNodeTo.push_back(t);
        for(int j=0; j<stopName[i].size(); j++){
            vector<int> t1;
            stopNode[i].push_back(t1);
        }
        for(int j=0; j<nodeNum[i]; j++){
            vector<int> p1;
            NodeFrom[i].push_back(p1);
            NodeTo[i].push_back(p1);
            CrossNodeFrom[i].push_back(p1);
            CrossNodeTo[i].push_back(p1);
        }
    }
    for(int i=0; i<trackN; i++){
        vector<vector<int> > t;
        trainNode.push_back(t);
        for(int j=0; j<trainName[i].size(); j++){
            vector<int> t1;
            trainNode[i].push_back(t1);
        }
    }
}
void nodeDetail(vector<vector<int> >& timeTable, vector<vector<string> >& stopName, vector<vector<string> >&trainName, int *&nodeNum, int n){
    int si = 0; // stop index
    int Count = 0; //time
    int tmp; //train;
    int same = 0;// sameNode index sameNode[n][same]
    int sameN = 0;// sameNode[n][?] contains how many nodes
    int whichArc = 0;
    for(int i=0; i<nodeNum[n]; i++){

        string sN = " "; //stopName
		string tN = " "; //trainName
		int T = 0; //time
		bool p = true; //pass
		bool a = false;
		//cout<<"n: "<<n<<endl;
		//cout<<"stopNameSize: "<<stopName[n].size()<<endl;
		//cout<<"nodeNumSie: "<<nodeNum[n]<<endl;
		if(i==0 || i==nodeNum[n]-1){ //start and sink
			node[n][i].setNode(sN, tN, T, p, a);
		}
		else if(i<= stopName[n].size() || i>= (nodeNum[n]-1-stopName[n].size())){// virtual node
			int t1 = 0; //for sort time
			int t2 = INT_MAX; //for sort time
			sN = stopName[n].at(si);// si->index, sN->stopname
			if(i<= stopName[n].size()){
				node[n][i].setNode(sN, tN, t1, p, a);
				stopNode[n][si].push_back(i);
			}
			else{
				node[n][i].setNode(sN, tN, t2, p, a);
				stopNode[n][si].push_back(i);
			}
			si++;
			if(si >= stopName[n].size())
					si = 0;
			//cout<<"i 2: "<<i<<endl;

		}
		else{
			tmp = (Count/stopName[n].size()); // which train
			if(tmp < trainName[n].size()){
				sN = stopName[n].at(si);
				tN = trainName[n].at(tmp);
				T = timeTable[n].at(Count);
				if(timeTable[n].at(Count) == 0){
					p = false;
				}
				node[n][i].setNode(sN, tN, T, p, a);
				stopNode[n][si].push_back(i);
				trainNode[n][tmp].push_back(i);
				Count++;
				si++;
				if(si >= stopName[n].size())
					si = 0;
			}
			else{ // for next map
				a = true;
                int sNIndex = sameNode[n][same].nowIndex;
                int TIndex = sameNode[n][same].targetTrack;
                int nextIndex = sameNode[n][same].targetIndex;
				sN =  stopName[n].at(sNIndex);
				tN = trainName[TIndex].at(sameN);
				T = timeTable[TIndex].at(nextIndex+(stopName[TIndex].size()*sameN));
				if(T == 0){
					p = false;
				}
				node[n][i].setNode(sN, tN, T, p, a);
				stopNode[n][sNIndex].push_back(i);

				sameN++;
		        if(sameN == sameNode[n][same].targetNum){
		            same++;
		            sameN = 0;
		        }
			}
		}
		//cout<<"n: "<<n<<" i: "<<i<<endl;
	}
}
void cross(vector<vector<int> >& timeTable, vector<vector<string> >& stopName, vector<vector<string> >&trainName, int *&nodeNum, int n, int& whichArc){
	int crossN = nodeNum[n]-(stopName[n].size()*trainName[n].size() + 2*stopName[n].size() + 2);
	int original = stopName[n].size()*trainName[n].size() + 2*stopName[n].size() + 2;
	int same = 0;// sameNode index sameNode[n][same]
    int sameN = 0;// sameNode[n][?] contains how many nodes
    for(int i=0; i<crossN; i++){
    	int sNIndex = sameNode[n][same].nowIndex;
	    int TIndex = sameNode[n][same].targetTrack;
	    int nextIndex = sameNode[n][same].targetIndex;
	    int T = timeTable[TIndex].at(nextIndex+(stopName[TIndex].size()*sameN));
	    srand(time(NULL));
	    pair<int,int> s,t;
        for(int k=0; k<stopNode[TIndex][nextIndex].size(); k++){
            int nextNode = stopNode[TIndex][nextIndex][k];
            if(T == node[TIndex][nextNode].getTime() ){//&& node[TIndex][nextNode].getadd()==true
                s = make_pair( TIndex,  nextNode);
                CrossNodeTo[TIndex][nextNode].push_back(whichArc);
                break;
            }
        }
        int tNode = original-1-stopName[n].size();
        t = make_pair( n, tNode+i);
        crossArc[whichArc].setArc( s, t, rand()%7+6);
        CrossNodeFrom[n][tNode+i].push_back(whichArc);

        sameN++;
        whichArc++;
        if(sameN == sameNode[n][same].targetNum){
            same++;
            sameN = 0;
        }
    }
}
void arcDetail(vector<vector<string> >& stopName, vector<vector<string> >&trainName, int n){
    int whichArc = 0;
    //srand(time(NULL));
	int Count = 0;
	int Count1 = 1;
	int Count2 = stopName[n].size();
	//=========== virtual arc
   //cout<<"nodeNum: "<<nodeNum[n]<<endl;
	while(Count < 2*stopName[n].size()){
		//cout<<"whichArc: "<<whichArc<<endl;
		if(Count < stopName[n].size()){
            pair<int,int> s,t;
            s = make_pair( n, 0 );
            t = make_pair( n, Count1);
            bool density=0;
            for(int i=0; i<crossNode.size(); i++){
            	if(node[n][Count1].getstopName() == crossNode[i]){
            		arc[n][whichArc].setArc(s, t, 0);
            		density = 1;
            		break;
            	}
            }
            if(density==0)
				arc[n][whichArc].setArc(s, t, 0);
			
			NodeTo[n][0].push_back(whichArc);
			NodeFrom[n][Count1].push_back(whichArc);
			Count1++;
			whichArc++;
			Count++;
		}

		else{
            pair<int,int> s,t;
            s = make_pair( n, nodeNum[n]-1-Count2 );
            t = make_pair( n, nodeNum[n]-1);
			bool density=0;
            for(int i=0; i<crossNode.size(); i++){
            	if(node[n][s.second].getstopName() == crossNode[i]){
            		arc[n][whichArc].setArc(s, t, 0);
            		density = 1;
            		break;
            	}
            }
            if(density==0)
				arc[n][whichArc].setArc(s, t, 0);
			NodeTo[n][nodeNum[n]-1-Count2].push_back(whichArc);
			NodeFrom[n][nodeNum[n]-1].push_back(whichArc);
			whichArc++;
			Count2--;
			Count++;
		}
	}

    // stop arc
	for(int i=0; i<stopNode[n].size();i++){ // for each stop
		multimap< int,int > TimeCheck;
		vector<int> vec;
		for(int j=0; j<stopNode[n][i].size(); j++){
			int nodetime = node[n][stopNode[n][i][j]].getTime();
			TimeCheck.insert( pair<int,int> (nodetime , stopNode[n][i][j]));
		}

		// after sorting time
		for(multimap<int,int>::iterator it=TimeCheck.begin(); it != TimeCheck.end(); it++){
			vec.push_back(it->second); // save node index
		}

		for(int j=0; j<vec.size()-1; j++){
			//cout<<whichArc<<endl;
			pair<int,int> s,t;
            s = make_pair( n, vec.at(j) );
            t = make_pair( n, vec.at(j+1));
			int gap = node[n][t.second].getTime()-node[n][s.second].getTime();
            bool density=0;
            for(int i=0; i<crossNode.size(); i++){
            	if(node[n][s.second].getstopName() == crossNode[i]){
            		arc[n][whichArc].setArc(s, t, gap*(rand()%6+7));
            		density = 1;
            		break;
            	}
            }
            if(density==0)
				arc[n][whichArc].setArc(s, t, gap*(rand()%6+2));
			NodeTo[n][vec.at(j)].push_back(whichArc);
			NodeFrom[n][vec.at(j+1)].push_back(whichArc);
			whichArc++;
		}
	}

	// train arc
	for(int i=0; i<trainNode[n].size();i++){ // for each stop
		multimap< int,int > TimeCheck;
		vector<int> vec;
		for(int j=0; j<trainNode[n][i].size(); j++){
			int nodetime = node[n][trainNode[n][i][j]].getTime();
			TimeCheck.insert( pair<int,int> (nodetime , trainNode[n][i][j]));
		}
		// after sorting time
		for(multimap<int,int>::iterator it=TimeCheck.begin(); it != TimeCheck.end(); it++){
			vec.push_back(it->second); // save node index
		}
		for(int j=0; j<vec.size()-1; j++){
			//cout<<whichArc<<endl;
			pair<int,int> s,t;
            s = make_pair( n, vec.at(j) );
            t = make_pair( n, vec.at(j+1));
			int gap = node[n][t.second].getTime()-node[n][s.second].getTime();
			bool density=0;
            for(int i=0; i<crossNode.size(); i++){
            	if(node[n][s.second].getstopName() == crossNode[i]){
            		arc[n][whichArc].setArc(s, t, gap*(rand()%6+5));
            		density = 1;
            		break;
            	}
            }
            if(density==0)
				arc[n][whichArc].setArc(s, t, gap*(rand()%5+1));
			NodeTo[n][vec.at(j)].push_back(whichArc);
			NodeFrom[n][vec.at(j+1)].push_back(whichArc);
			whichArc++;
		}
	}
}
void gurobi(){
	var["guard"] = model->addVar(0, INT_MAX, 1, GRB_INTEGER, "guard");
	
	for(int i=0; i<trackN; i++){
		for(int j=0; j<arc[i].size(); j++){
			stringstream name,v;
			name<<"Arc"<<i<<"_"<<j;
			var[name.str()] = model->addVar(1, INT_MAX, 0, GRB_INTEGER, name.str());
			name.clear();
		}


	}
	for(int i=0; i<crossArc.size(); i++){
			stringstream name,v;
			name<<"crossArc"<<i;
			var[name.str()] = model->addVar(1, INT_MAX, 0, GRB_INTEGER, name.str());
			
	}
	model -> update();
	// flow balance
	GRBLinExpr start, end;
	for(int i=0; i<trackN; i++){
		for(int j=0; j<nodeNum[i]; j++){
			if(j == 0){
				for(int k=0; k<NodeTo[i][j].size(); k++){
					stringstream startname;
					startname<<"Arc"<<i<<"_"<<NodeTo[i][j][k];
					start += var[startname.str()];
				}
			}
			else if(j == nodeNum[i]-1){
				for(int k=0; k<NodeFrom[i][j].size(); k++){
					stringstream endname;
					endname<<"Arc"<<i<<"_"<<NodeFrom[i][j][k];
					end += var[endname.str()];
				}
			}
			else{
				GRBLinExpr s, t;
				//from
				for(int k=0; k<NodeFrom[i][j].size(); k++){
					stringstream name;
					name<<"Arc"<<i<<"_"<<NodeFrom[i][j][k];
					s += var[name.str()];
				}
				for(int k=0; k<CrossNodeFrom[i][j].size(); k++){
					stringstream name;
					name<<"crossArc"<<CrossNodeFrom[i][j][k];
					s += var[name.str()];
				}

				// to
				for(int k=0; k<NodeTo[i][j].size(); k++){
					stringstream name;
					name<<"Arc"<<i<<"_"<<NodeTo[i][j][k];
					t += var[name.str()];
				}
				for(int k=0; k<CrossNodeTo[i][j].size(); k++){
					stringstream name;
					name<<"crossArc"<<CrossNodeTo[i][j][k];
					t += var[name.str()];
				}
				model->addConstr(s == t,"");
			}

		}
	}
	model->addConstr(start == var["guard"],"source");
	model->addConstr(end == var["guard"],"sink");

	model -> update();
	model->set(GRB_IntAttr_ModelSense,1);//1->min
    model->update();
    model->write("model.lp");
	model->optimize();
    if ( model->get(GRB_IntAttr_Status)==2 ) {
		model->write("solution.sol");
	}
    int NodeN=0;
    int ArcN=0;
    for(int i=0; i<trackN; i++){
        NodeN += nodeNum[i];
        ArcN += arcNum[i];
    }
    ArcN += crossArc.size();
	cout<<"nodeN: "<<NodeN<<endl;
	cout<<"ArcN: "<<ArcN<<endl;
	fstream w;
	/*w.open("result.txt",ios::out);
	for(int i=0; i<trackN; i++){
		for(int j=0; j<arc[i].size(); j++){
			stringstream ss;
			ss<<"ArcVisited"<<i<<"_"<<j;
			w<<var[ss.str()].get(GRB_DoubleAttr_X)<<" ";
		}
	}
	w.close();
	*/

}
