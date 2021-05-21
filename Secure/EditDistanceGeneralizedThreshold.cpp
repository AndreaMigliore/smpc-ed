#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <climits>
#include "emp-sh2pc/emp-sh2pc.h"
using namespace emp;
using namespace std;

Integer secure_minimum(Integer a, Integer b, Integer c) {
    Integer t = b.select((a < b), a);
    return c.select((t < c), t);
}

Integer secure_minimum_two(Integer a, Integer b) {
    return b.select((a < b), a);
}

int int_minimum_two(int a, int b) {
    return a < b ? a : b;
}

int int_maximum_two(int a, int b) {
    return a > b ? a : b;
}

int edit_distance_generalized_threshold(vector<int> sequence, int lengthAlice, int lengthBob, int intBitSize) {
    vector<Integer> sequenceAlice;
    vector<Integer> sequenceBob;
    Integer one(intBitSize, 1, ALICE);
    Integer result(intBitSize, 0, ALICE);
    Integer dia;

    int i, j, min, max, gmax, gmin;

    vector<Integer> row (lengthBob+1);

    for (i = 0; i < lengthAlice; i++)
        sequenceAlice.push_back(Integer (intBitSize, sequence[i], ALICE));

    for (i = 0; i < lengthBob; i++)
        sequenceBob.push_back(Integer (intBitSize, sequence[i], BOB));

    gmax = (lengthBob - 1) / 2;
    gmin = 1 - gmax - (lengthAlice - lengthBob);

    for (j = 0; j <= gmax; j++)
        row[j] = Integer (intBitSize, j, ALICE);

    for (i = 1; i <= lengthAlice; i++) {
        row[0] = Integer (intBitSize, i, ALICE) - one;

        min = int_maximum_two(gmin, 1);
        max = int_minimum_two(gmax, lengthBob);
        gmin++;
        gmax++;

        dia = row[min - 1];
        Integer top = row[min];

        dia = dia.select((sequenceAlice[i - 1] != sequenceBob[min - 1]), secure_minimum_two(dia, top) + one);

        row[min] = dia;
        Integer left = dia;
        dia = top;

        for (j = min + 1; j <= max; j++) {
            top = row[j];

            dia = dia.select((sequenceAlice[i - 1] != sequenceBob[j - 1]), secure_minimum(dia, top, left) + one);
            row[j] = dia;
            left = dia;
            dia = top;
        }

        if (lengthBob == max)
            continue;

        dia = dia.select((sequenceAlice[i - 1] != sequenceBob[max]), secure_minimum_two(dia, left) + one);
        row[max + 1] = dia;
    }
    dia = row[lengthBob];
    return dia.reveal<int>();
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
    cout << "Integer bit size = " << intBitSize << endl;
    cout << "String length 1 = " << lengthAlice << endl;
    cout << "String length 2 = " << lengthBob << endl << endl;

    struct timespec start, end; 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    setup_semi_honest(io, party);
    int result = edit_distance_generalized_threshold((party == 1) ? nucleobasesAlice : nucleobasesBob, lengthAlice, lengthBob, intBitSize);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    cout << "Edit distance:\t" << result << endl;

    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = (seconds + nanoseconds*1e-9)*1000;
    
    cout << "Time (milliseconds) = " << elapsed << endl;
    
    finalize_semi_honest();
    
    delete io;
}
