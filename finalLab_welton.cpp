/*
Name: finalLab_welton
Author: Christopher Welton
Description: This program creates a random 20x20 2D array and returns the peak values (max & min) for the board
*/
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <iomanip>


using namespace std;

struct MaxMinResult {
    bool min;
    bool max;
};

const pair<int, int>  DIRECTIONS[8] = {
    make_pair(-1, 0),  //up 1
    make_pair(0, -1),  //left 1
    make_pair(0, 1),  //right 1
    make_pair(1, 0), //down 1
    make_pair(1, 1),  //diag down-right
    make_pair(-1, 1), //diag up-right
    make_pair(-1, -1), //diag up-left
    make_pair(1, -1) //diag down-left
    };

MaxMinResult isValueAPeak(int i, int j, const vector<vector<int> >& grid){
    MaxMinResult result;
    int minValue = grid[i][j];
    int maxValue = grid[i][j];

    for(int k=0; k < 8; k++){
        int neighborX = i + DIRECTIONS[k].first;
        int neighborY = j + DIRECTIONS[k].second;

        if(neighborX >= 0 && neighborX < grid.size() && neighborY >= 0 && neighborY < grid[0].size()){
            if(grid[neighborX][neighborY] < minValue){
                minValue = grid[neighborX][neighborY];
            } else if(grid[neighborX][neighborY] > maxValue){
                maxValue = grid[neighborX][neighborY];
            }
        }
    }
    result.min = (grid[i][j] == minValue);
    result.max = (grid[i][j] == maxValue);

    return result;
}

vector<vector<int> > generateBoard(int x, int y){
    vector<vector<int> > board(x, vector<int> (y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,101);
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            board[i][j] = dist6(rng);
        }
    }

    return board;
}

map<string, vector<vector<pair<int, int> > > > generateResultsMap() {
    map<string, vector<vector<pair<int, int> > > > results;
    vector<vector<pair<int, int> > > cornerPeaks; 
    cornerPeaks.push_back(vector<pair<int, int> >());
    cornerPeaks.push_back(vector<pair<int, int> >());
    results["CORNER"] = cornerPeaks;

    vector<vector<pair<int, int> > > edgePeaks;
    edgePeaks.push_back(vector<pair<int, int> > ());
    edgePeaks.push_back(vector<pair<int, int> > ());
    results["NON-CORNER EDGE"] = edgePeaks;

    vector<vector<pair<int, int> > > interiorPeaks;
    interiorPeaks.push_back(vector<pair<int, int> > ());
    interiorPeaks.push_back(vector<pair<int, int> > ());
    results["INSIDE (NON-EDGE/NON-CORNER)"] = interiorPeaks;

    return results;
}

map<string, vector<vector< pair<int, int> > > > findPeaks(vector<vector<int> > board){
    map<string, vector<vector<pair<int, int> > > > results = generateResultsMap();

    for(int i=0; i< board.size(); i++){
        for(int j=0; j<board.size(); j++){
            MaxMinResult result;

            // Corner check
            if((i == 0 && j== 0) || (i == 0 && j == board.size() -1) || (i == board[0].size() - 1 && j == 0) || (i == board[0].size() - 1 && j == board.size() -1)){
                result = isValueAPeak(i, j, board);
                if(result.min){
                    results["CORNER"][0].push_back(make_pair(i, j));
                }
                if(result.max){
                    results["CORNER"][1].push_back(make_pair(i, j));
                }
            }
            //Edge check
             else if((i == 0 && (j != 0 && j != board[0].size() - 1)) || (j== 0 && (i != 0 && i != board[0].size() -1)) || (j == board[0].size() -1 && (i != board[0].size() -1 && i != 0)) || (i == board.size() -1 && (j != 0 && j != board[0].size() -1))){
                result = isValueAPeak(i, j, board);
                // printf("Results for value %d are Min: %s and Max: %s \n", board[i][j], result.min ? "true" : "false", result.max ? "true" : "false");
                if(result.min){
                    results["NON-CORNER EDGE"][0].push_back(make_pair(i, j));
                }
                if(result.max){
                    results["NON-CORNER EDGE"][1].push_back(make_pair(i, j));
                }
            }
            //Default
            else {
                result = isValueAPeak(i, j, board);
                if(result.min){
                    results["INSIDE (NON-EDGE/NON-CORNER)"][0].push_back(make_pair(i, j));
                }
                if(result.max){
                    results["INSIDE (NON-EDGE/NON-CORNER)"][1].push_back(make_pair(i, j));
                }
            }
        }
    }

    return results;
}

int main() {
    map<string, vector<vector<pair<int, int> > > > peaks;
    vector<vector<int> > board = generateBoard(20, 20);

    for(int i=0; i<board.size(); i++){
        for(int j=0; j < board[0].size(); j++){
            cout << setw(3) << board[i][j] << " ";
        }
        cout << endl;
    }
    peaks = findPeaks(board);
    map<string, vector<vector<pair<int, int> > > >::iterator it;
    vector<int> allMaxValues;
    vector<int> allMinValues;
    for (it = peaks.begin(); it != peaks.end(); it++)
    {
        cout << it-> first << endl;
        vector<pair<int, int> > maxValues = it -> second[1];
        vector<pair<int, int> > minValues = it -> second[0];
       
        if(maxValues.size() > 0){
            for(vector<pair<int, int> >::size_type i=0; i != maxValues.size(); i++){
                allMaxValues.push_back(board[maxValues[i].first][maxValues[i].second]);
                printf("Peak max: %d, found at: (%d, %d) \n", board[maxValues[i].first][maxValues[i].second], maxValues[i].first, maxValues[i].second);
            }
        } 
        if(minValues.size() > 0){
            for(vector<pair<int, int> >::size_type i=0; i != minValues.size(); i++){
                allMinValues.push_back(board[minValues[i].first][minValues[i].second]);
                printf("Peak min: %d, found at: (%d, %d) \n", board[minValues[i].first][minValues[i].second], minValues[i].first, minValues[i].second);
            }
        } 
    }

    std::sort(allMaxValues.begin(), allMaxValues.end());
    std::sort(allMinValues.begin(), allMinValues.end());
    cout << "Mins in order: " << endl;
    for(vector<int>::size_type i=0; i != allMinValues.size(); i++){
        cout << allMinValues[i] << " ";
    }
    cout << "\nMaxes in order: " << endl;
    for(vector<int>::size_type i=0; i != allMaxValues.size(); i++){
        cout << allMaxValues[i] << " ";
    }

    return 0; 
}