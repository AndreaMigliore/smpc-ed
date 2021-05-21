/*
 * > S. Wu, U. Manber, G. Myers, W. Miller, "An O(NP) sequence 
 * > comparison algorithm". Information Processing Letters,
 * > Vol. 35, pp. 317-323. 1990.
 *
 * > Inspired by: http://par.cse.nsysu.edu.tw/~lcs/Wu%20Algorithm.php
 *
 */

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
using namespace std;

int snake(string s1, string s2, int s1len, int s2len, int k, int j) {
    int i = j - k;
    while(i < s1len && j < s2len && s1.at(i) == s2.at(j)){
        i++;
        j++;
    }
    return j;
}

int edit_distance_wu(string s1, string s2, int s1len, int s2len) {
    int k;
    int p = -1;
    int delta = s2len - s1len;
    int offset = s1len + 1;
    vector<int> fp (s1len + s2len + 3);
    fill(&fp[0], &fp[s1len + s2len + 3], -1);

    do {
        p++;
        for (k = -p; k <= delta-1; k++) {
            fp[k+offset] = snake(s1, s2, s1len, s2len, k, max(fp[k-1+offset]+1, fp[k+1+offset]));
        }
        for (k = delta+p; k >= delta+1; k--) {
            fp[k+offset] = snake(s1, s2, s1len, s2len, k, max(fp[k-1+offset]+1, fp[k+1+offset]));
        }
        fp[delta+offset] = snake(s1, s2, s1len, s2len, delta, max(fp[delta-1+offset]+1, fp[delta+1+offset]));
    } while (fp[delta+offset] != s2len);
    
    return delta + 2*p;
}

int main(int argc, char** argv) {
    string s1, s2, line;
    string txt = argv[1] != NULL ? argv[1] : "1000";
    string prefix = "../input/InputEditDistance";
    string suffix = ".txt";
    ifstream inputFile(prefix+txt+suffix);
    if (inputFile) {
    	getline(inputFile, line);
    	s1 = line;
    	getline(inputFile, line);
    	s2 = line;
    	
    	inputFile.close();
    }
    else {
    	cout << "Cannot open input file" << endl;
    	return 0;
    }
    
    if(s1.length() < s2.length())
        swap(s1, s2);

    int s1len, s2len;
    s1len = s1.length();
    s2len = s2.length();
    cout << "String length 1 = " << s1len << endl;
    cout << "String length 2 = " << s2len << endl << endl;

    struct timespec start, end; 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    int result = edit_distance_wu(s1, s2, s1len, s2len);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    cout << "Edit distance - Wu:\t" << result << endl;

    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = (seconds + nanoseconds*1e-9)*1000;
    
    cout << "Time (milliseconds) = " << elapsed << endl;

    return 0;
}
