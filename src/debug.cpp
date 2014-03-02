
/*
 * Copyright (c) 2013 Stefan Nozinic
 *
 * This file is part of the pageproc. Pdf2epub is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * Pdf2epub is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "debug.hpp"

#include <iostream>
#include <vector> 
#include <string>

using namespace std;

void print_int_vector(string msg, vector<int> v) 
{
	cout << "\n\n" << msg << ": Vector: " << v.size() << " elements:\n"; 
	for (int i = 0; i<v.size(); i++) 
	{
		cout << i << ": " << v.at(i) << endl; 
	}
	cout << "\n";
}

void print_byte_vector(vector<char> v) 
{
	for (int i = 0; i<v.size(); i++) 
	{
		cout << v[i];
	}
}

void printLines(vector<HVLine> lines) 
{
	cout << "Number of lines: " << lines.size() << endl; 
	cout << "\tx\ty\tlength\n";
	for (int i = 0; i<lines.size(); i++) 
	{
		cout << "Line: \t" << lines.at(i).x << "\t" << lines.at(i).y << "\t" << lines.at(i).length << endl; 
	}
}
