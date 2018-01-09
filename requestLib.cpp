/*
 * =========================================================================================
 * Name        : eventLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This library contains functions used for event management
 * =========================================================================================
 */

#include "requestLib.h"


void loadRequests(char* fName, L1List<VM_Request> &rList) {
    // TODO: write your code to load requests. Each request is separated by a whitespace
	ifstream inFile(fName);

	if (inFile) {
		string line;
		while (getline(inFile, line)) {
			/// On Windows, lines on file ends with \r\n. So you have to remove \r
			if (line[line.length() - 1] == '\r')
				line.erase(line.length() - 1);
			if (line.length() < 1) continue;

			istringstream iss(line);
			if (iss) {
				string sub;
				iss >> sub;
				if (sub.length()) {
					VM_Request __r(sub);
					int i = 0;
					while (iss) {
						if (i >= MAX_PARAM_SIZE) break;
						iss >> __r.params[i++];
					}
					rList.insertHead(__r);
				}
			}
		}
		rList.reverse();
		inFile.close();
	}
	else {
		cout << "The file is not found!";
	}
}
