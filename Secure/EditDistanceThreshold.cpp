#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <climits>
#include "emp-sh2pc/emp-sh2pc.h"
using namespace emp;
using namespace std;

Integer minimum(Integer a, Integer b, Integer c) {
    Integer t = b.select((a < b), a);
    return c.select((t < c), t);
}

int int_minimum_two(int a, int b) {
    return a < b ? a : b;
}

int int_maximum_two(int a, int b) {
    return a > b ? a : b;
}

int edit_distance_threshold(vector<int> sequence, int lengthAlice, int lengthBob, int threshold, int intBitSize) {
    vector<Integer> sequenceAlice;
    vector<Integer> sequenceBob;
    Integer one(intBitSize, 1, ALICE);
    Integer intmax(intBitSize, lengthAlice, ALICE);
    Integer resultFor(intBitSize, 0, ALICE);
    int i, row, col, min, max;

    for (i = 0; i < lengthAlice; i++)
        sequenceAlice.push_back(Integer (intBitSize, sequence[i], ALICE));

    for (i = 0; i < lengthBob; i++)
        sequenceBob.push_back(Integer (intBitSize, sequence[i], BOB));

    if (threshold == -1)
        threshold = lengthAlice;

    vector<Integer> p (lengthBob + 2);
    vector<Integer> d (lengthBob + 2);

    for (i = 0; i < lengthBob + 1; i++)
        d[i] = intmax;

    int boundary = int_minimum_two(lengthBob, threshold) + 1;
    for(i = 0; i < boundary; i++)
        p[i] = Integer(intBitSize, i, ALICE);

    for( ; i < lengthBob + 1; i++)
        p[i] = intmax;

    for(row = 1; row <= lengthAlice; row++) {
        d[0] = Integer(intBitSize, row, ALICE);

        min = int_maximum_two(1, row - threshold);
        max = int_minimum_two(lengthBob, row + threshold);

        if (min > max)
            return -1;

        if(min > 1)
            d[min-1] = intmax;

        for(col = min; col <= max; col++) {
            resultFor = minimum(p[col-1], d[col-1], p[col]) + one;
            d[col] = resultFor.select((sequenceAlice[row-1] == sequenceBob[col-1]), p[col-1]);
        }
        p.swap(d);
    }

    if (p[lengthBob].reveal<int>() < threshold)
        return p[lengthBob].reveal<int>();
    else
        return -1;
}

int main(int argc, char** argv) {
    int port, party;
    parse_party_and_port(argv, &party, &port);
    NetIO * io = new NetIO(party==ALICE ? nullptr : "127.0.0.1", port);

    string line;
    int lengthAlice = 0;
    int lengthBob = 0;
    vector<int> nucleobasesAlice;
    vector<int> nucleobasesBob;
    string txt = argv[3] != NULL ? argv[3] : "1000";
    string prefix = "input/InputEditDistance";
    string suffix = ".txt";
    ifstream inputFile(prefix+txt+suffix);
    if (inputFile) {
        getline(inputFile, line);
        lengthAlice = line.length();
        for(std::string::iterator it = line.begin(); it != line.end(); ++it) {
            switch(*it) {
            case 'A':
            case 'a':
                nucleobasesAlice.push_back(1);
                break;
            case 'C':
            case 'c':
                nucleobasesAlice.push_back(2);
                break;
            case 'G':
            case 'g':
                nucleobasesAlice.push_back(3);
                break;
            case 'T':
            case 't':
                nucleobasesAlice.push_back(4);
                break;
            }
        }

        getline(inputFile, line);
        lengthBob = line.length();
        for(std::string::iterator it = line.begin(); it != line.end(); ++it) {
            switch(*it)    {
            case 'A':
            case 'a':
                nucleobasesBob.push_back(1);
                break;
            case 'C':
            case 'c':
                nucleobasesBob.push_back(2);
                break;
            case 'G':
            case 'g':
                nucleobasesBob.push_back(3);
                break;
            case 'T':
            case 't':
                nucleobasesBob.push_back(4);
                break;
            }
        }
        inputFile.close();
    }
    else {
        cout << "Cannot open input file" << endl;
        delete io;
        return 0;
    }

    if(lengthAlice < lengthBob){
        swap(lengthAlice, lengthBob);
        nucleobasesAlice.swap(nucleobasesBob);
    }
    
    int intBitSize = ceil(log(lengthAlice)/log(2))+1;
    int threshold = lengthAlice*6/10;
    if (argc == 5 && argv[4] != NULL)
        threshold = atoi(argv[4]);

    cout << "Integer bit size = " << intBitSize << endl;
    cout << "String length 1 = " << lengthAlice << endl;
    cout << "String length 2 = " << lengthBob << endl;
    cout << "Threshold = " << threshold << endl << endl;

    struct timespec start, end; 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    setup_semi_honest(io, party);
    int result = edit_distance_threshold((party == 1) ? nucleobasesAlice : nucleobasesBob, lengthAlice, lengthBob, threshold, intBitSize);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    cout << "Edit distance:\t" << result << endl;

    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = (seconds + nanoseconds*1e-9)*1000;
    
    cout << "Time (milliseconds) = " << elapsed << endl;
    
    finalize_semi_honest();
    
    delete io;
}
