#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <math.h>
#include <algorithm>
#include <tuple>
#include <ctime>

using namespace std;
typedef vector<vector<double>> matrix;
typedef vector<pair<double, double>> row;
typedef tuple<int, int, int> triple;

double inf = numeric_limits<double>::infinity();
vector<int> route;
matrix distances;

double dist(double x, double y, double x1, double y1)
{
    return sqrt((x1 - x)*(x1 - x) + (y1 - y)*(y1 - y));
}

int proper_min(vector<int>* visited, int a, int num)
{
    double mini = inf;
    int ind = 0;
    bool fl = true;
    for (int i = 0; i < num; i++)
    {
        if(distances[a][i] < mini)
        {
            for(int g = 0; g < visited->size(); g++) {
                if (i == visited->at(g))
                    fl = false;
            }
            if(fl)
            {
                ind = i;
                mini = distances[a][i];
            }
        }
        fl = true;
    }
    return ind;
}

void greedy(vector<int>* tour, int num)
{
    vector<int> visited;
    int city = 0;
    while (visited.size() < num)
    {
        visited.push_back(city);
        tour->push_back(city);
        city = proper_min(&visited, city, num);
    }
}

void reverse_segment_if_better(vector<int>* tour, int i, int j, int k)
{
    int A = 0;
    if(i - 1 < 0)
        A = tour->at(tour->size()-1);
    else
        A = tour->at(i-1);
    int B = tour->at(i);
    int C = tour->at(j-1);
    int D = tour->at(j);
    int E = tour->at(k-1);
    int F = tour->at(k % tour->size());

    double d0 = distances[A][B] + distances[C][D] + distances[E][F];
    double d1 = distances[A][C] + distances[B][D] + distances[E][F];
    double d2 = distances[A][B] + distances[C][E] + distances[D][F];
    double d3 = distances[A][D] + distances[E][B] + distances[C][F];
    double d4 = distances[F][B] + distances[C][D] + distances[E][A];

     if(d0 > d1)
         reverse(tour->begin()+i, tour->begin()+j);
     else if(d0 > d2)
         reverse(tour->begin()+j, tour->begin()+k);
     else if(d0 > d4)
         reverse(tour->begin()+i, tour->begin()+k);
     else if(d0 > d3)
     {
         vector<int> tmp;
         for(int z = j; z < k; z++)
             tmp.push_back(route[z]);
         for(int z = i; z < j; z++)
             tmp.push_back(route[z]);
         for(int f = i; f < k; f++)
             route[f] = tmp[f];
     }
}

vector<triple> combinations(int n)
{
    vector<triple> res;
    for(int i = 0; i < n; i++)
        for(int j = i+2; j < n; j++)
            for(int k = j+2; k < n; k++)
                res.emplace_back(make_tuple(i,j,k));
    return res;
}


void three_opt(vector<int>* tour, clock_t time)
{
    clock_t one = 0;
    while((float)(clock() - time + one)/CLOCKS_PER_SEC < 9.5){
        clock_t begint = clock();
        vector<triple> comb = combinations(tour->size());
        for(int i = 0; i < comb.size(); i++)
        {
            reverse_segment_if_better(tour, get<0>(comb[i]), get<1>(comb[i]), get<2>(comb[i]));
        }
        one = clock() - begint;
    }
}


int main() {
   clock_t begin_time = clock();
   int num = 0;
   double x, y;
   cin >> num;
   row points;

    for (int i =0; i < num; i++)
   {
       cin >> x >> y;
       points.push_back(make_pair(x,y));
   }

   for (int i = 0; i < num; i++)
   {
       vector<double> temp;
       for (int j = 0; j < num; j++)
       {
           if (i == j)
               temp.push_back(inf);
           else
               temp.push_back(dist(points[i].first, points[i].second, points[j].first, points[j].second));
       }
       distances.push_back(temp);
   }

   greedy(&route, num);

   three_opt(&route, begin_time);

   int ind = 0;
   bool fl = false;
   for(int i = 0; i < num; i++)
   {
       if(route[i]== 0)
       {
           ind = i;
           fl = true;
       }
       if(fl)
           cout << route[i] + 1 << ' ';
   }
   for(int i = 0; i < ind; i++)
       cout << route[i] + 1 << ' ';
   cout << 1;
    return 0;
}
