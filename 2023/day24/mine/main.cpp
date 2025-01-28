
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::regex;
using std::smatch;
using std::regex_search;

struct Hailstone{
    Hailstone(long long pa,long long pb,long long pc,int va, int vb, int vc):px(pa),py(pb),pz(pc),vx(va),vy(vb),vz(vc){}
    long long px,py,pz;
    int vx,vy,vz;
};

/* Thanks Copilot for the Gaussian Elimination algo */
vector<long double> gaussianElimination(vector<vector<long double>> matrix) {
    int n = matrix.size();
    
    // Forward elimination
    for (int i = 0; i < n; i++) {
        // Make the diagonal element 1 and normalize the row
        long double diag = matrix[i][i];
        for (int j = 0; j <= n; j++) {
            matrix[i][j] /= diag;
        }
        
        // Eliminate the below rows
        for (int k = i + 1; k < n; k++) {
            long double factor = matrix[k][i];
            for (int j = 0; j <= n; j++) {
                matrix[k][j] -= factor * matrix[i][j];
            }
        }
    }

    // Back substitution
    vector<long double> result(n);
    for (int i = n - 1; i >= 0; i--) {
        result[i] = matrix[i][n];
        for (int j = i + 1; j < n; j++) {
            result[i] -= matrix[i][j] * result[j];
        }
    }
    return result;
}

int main(){
    std::ifstream file("input");
    std::regex pattern(R"((-?\d+),\s+(-?\d+),\s+(-?\d+)\s+@\s+(-?\d+),\s+(-?\d+),\s+(-?\d+))"); 
    std::smatch matches;

    string line;
    vector<Hailstone> hailstones;
    while(getline(file,line)){
        if(regex_search(line,matches,pattern)){
            hailstones.emplace_back(stoll(matches.str(1)),stoll(matches.str(2)),stoll(matches.str(3)),stoll(matches.str(4)),stoll(matches.str(5)),stoll(matches.str(6)));
        }
    }
    int ret_1 = 0;
    long long x_min = 200000000000000,x_max = 400000000000000;
    //long long x_min = 7,x_max = 27;
    for(int i=0;i<hailstones.size();++i){
        for(int j=i+1;j<hailstones.size();++j){
            int a = -hailstones[i].vx,b = hailstones[j].vx,c = -hailstones[i].vy,d = hailstones[j].vy;
            int det = a*d-b*c;
            if(det){
                long long x = hailstones[i].px - hailstones[j].px,y = hailstones[i].py - hailstones[j].py;
                float t1 = (d*x-b*y)/float(det);
                float t2 = (-c*x+a*y)/float(det);
                if(t1>0&&t2>0){
                    float x_intersect = hailstones[i].px + hailstones[i].vx*t1;
                    float y_intersect = hailstones[i].py + hailstones[i].vy*t1;
                    if(x_intersect>=x_min&&x_intersect<=x_max&&y_intersect>=x_min&&y_intersect<=x_max){
                        ++ret_1;
                    }
                }
            }
        }    
    }
    cout << "Part 1: " << ret_1 << endl;

    vector<vector<long double>> matrix_xy = {
        {static_cast<long double>(hailstones[1].vy-hailstones[0].vy), static_cast<long double>(-hailstones[1].vx+hailstones[0].vx),
        static_cast<long double>(-hailstones[1].py+hailstones[0].py), static_cast<long double>(hailstones[1].px-hailstones[0].px),
        static_cast<long double>(hailstones[1].px*hailstones[1].vy-hailstones[1].py*hailstones[1].vx-(hailstones[0].px*hailstones[0].vy-hailstones[0].py*hailstones[0].vx))},

        {static_cast<long double>(hailstones[2].vy-hailstones[0].vy), static_cast<long double>(-hailstones[2].vx+hailstones[0].vx),
        static_cast<long double>(-hailstones[2].py+hailstones[0].py), static_cast<long double>(hailstones[2].px-hailstones[0].px),
        static_cast<long double>(hailstones[2].px*hailstones[2].vy-hailstones[2].py*hailstones[2].vx-(hailstones[0].px*hailstones[0].vy-hailstones[0].py*hailstones[0].vx))},

        {static_cast<long double>(hailstones[3].vy-hailstones[0].vy), static_cast<long double>(-hailstones[3].vx+hailstones[0].vx),
        static_cast<long double>(-hailstones[3].py+hailstones[0].py), static_cast<long double>(hailstones[3].px-hailstones[0].px),
        static_cast<long double>(hailstones[3].px*hailstones[3].vy-hailstones[3].py*hailstones[3].vx-(hailstones[0].px*hailstones[0].vy-hailstones[0].py*hailstones[0].vx))},

        {static_cast<long double>(hailstones[4].vy-hailstones[0].vy), static_cast<long double>(-hailstones[4].vx+hailstones[0].vx),
        static_cast<long double>(-hailstones[4].py+hailstones[0].py), static_cast<long double>(hailstones[4].px-hailstones[0].px),
        static_cast<long double>(hailstones[4].px*hailstones[4].vy-hailstones[4].py*hailstones[4].vx-(hailstones[0].px*hailstones[0].vy-hailstones[0].py*hailstones[0].vx))}
    };

    vector<vector<long double>> matrix_xz = {
        {static_cast<long double>(hailstones[1].vz-hailstones[0].vz), static_cast<long double>(-hailstones[1].vx+hailstones[0].vx),
        static_cast<long double>(-hailstones[1].pz+hailstones[0].pz), static_cast<long double>(hailstones[1].px-hailstones[0].px),
        static_cast<long double>(hailstones[1].px*hailstones[1].vz-hailstones[1].pz*hailstones[1].vx-(hailstones[0].px*hailstones[0].vz-hailstones[0].pz*hailstones[0].vx))},

        {static_cast<long double>(hailstones[2].vz-hailstones[0].vz), static_cast<long double>(-hailstones[2].vx+hailstones[0].vx),
        static_cast<long double>(-hailstones[2].pz+hailstones[0].pz), static_cast<long double>(hailstones[2].px-hailstones[0].px),
        static_cast<long double>(hailstones[2].px*hailstones[2].vz-hailstones[2].pz*hailstones[2].vx-(hailstones[0].px*hailstones[0].vz-hailstones[0].pz*hailstones[0].vx))},

        {static_cast<long double>(hailstones[3].vz-hailstones[0].vz), static_cast<long double>(-hailstones[3].vx+hailstones[0].vx),
        static_cast<long double>(-hailstones[3].pz+hailstones[0].pz), static_cast<long double>(hailstones[3].px-hailstones[0].px),
        static_cast<long double>(hailstones[3].px*hailstones[3].vz-hailstones[3].pz*hailstones[3].vx-(hailstones[0].px*hailstones[0].vz-hailstones[0].pz*hailstones[0].vx))},

        {static_cast<long double>(hailstones[4].vz-hailstones[0].vz), static_cast<long double>(-hailstones[4].vx+hailstones[0].vx),
        static_cast<long double>(-hailstones[4].pz+hailstones[0].pz), static_cast<long double>(hailstones[4].px-hailstones[0].px),
        static_cast<long double>(hailstones[4].px*hailstones[4].vz-hailstones[4].pz*hailstones[4].vx-(hailstones[0].px*hailstones[0].vz-hailstones[0].pz*hailstones[0].vx))}
    };
    vector<string> labels{"P_x,P_y,V_x,V_y"};
    vector<long double> result_xy = gaussianElimination(matrix_xy);
    for (int i = 0; i < result_xy.size(); i++) {
        cout << labels[i] << " = " << result_xy[i] << endl;
    }
    vector<long double> result_xz = gaussianElimination(matrix_xz);
    for (int i = 0; i < result_xz.size(); i++) {
        cout << labels[i] << " = " << result_xz[i] << endl;
    }
    long long ret_2 = result_xz[0] + result_xy[1] + result_xz[1];
    cout << "Part 2: " << ret_2 << endl;
    return 0;
}