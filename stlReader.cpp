// Name: Nicholas Czaban
// Assignment: Final Project
// Title: STL File Reader
// Course: CS 200 
// Semester: Spring year
// Instructor: D. Byrnes
// Date: 2/19/18
// Program description: Reads STL files created by OpenSCAD and creates a vector of Facets
// Known Bugs: The reader can only handle ASCII-style STL files. It cannot read binary-style STL files.
// Instructions: Use a #include "stlReader.cpp" in your program and call the functions below

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>

using std::endl;
using std::cout;
using std::string;

struct Facet{
  double normal[3]; // The x,y,z values of the normal vector
  double vertices[3][3]; // The x,y,z values of the three vertices for each Facet. [][i] covers the x,y,z values, while [j][] handles the points
};

std::ostream& operator <<(std::ostream &o, const Facet& f){
  o << endl;
  o << "Normal Vector: x=" << f.normal[0] << " y=" << f.normal[1] << " z=" << f.normal[2] << endl;
  for(int i = 0; i < 3; i++){
	o << "Vertex " << i+1 << ": x=" << f.vertices[i][0] << " y=" << f.vertices[i][1] << " z=" << f.vertices[i][2] << endl;
  }
  o << endl;
}

std::vector<Facet> readFile(std::string filename){
  std::vector<Facet> facets;
  std::ifstream file(filename.c_str());
  string line;
  if(file.is_open()){
	Facet tmpFacet;
	int vertexNum = 0;
	while(std::getline(file, line)){
	  int pos=0, found;
	  char str[line.length()];
	  for(int i = 0; i < line.length(); i++){
		str[i] = line[i];
	  }
	  char* token;
	  try{
		token = strtok(str, " ");
		string sToken(token);
		if(sToken == "facet"){
		  token = strtok(NULL, " ");
		  token = strtok(NULL, " ");
		  tmpFacet.normal[0] = atof(token);
		  token = strtok(NULL, " ");
		  tmpFacet.normal[1] = atof(token);
		  token = strtok(NULL, " ");
		  tmpFacet.normal[2] = atof(token);
		}
		else if(sToken == "vertex"){
		  token = strtok(NULL, " ");
		  tmpFacet.vertices[vertexNum][0] = atof(token);
		  token = strtok(NULL, " ");
		  tmpFacet.vertices[vertexNum][1] = atof(token);
		  token = strtok(NULL, " ");
		  tmpFacet.vertices[vertexNum][2] = atof(token);
		  vertexNum = (vertexNum + 1)%3;
		  if (vertexNum == 0){
			facets.push_back(tmpFacet);
		  }
		  else if(sToken == "endsolid"){
			cout << sToken << endl;
		  }
		}
	  }
	  catch(...){
		cout << "Exception occurred. line is currently is currently |" << line << "|" << endl;
	  }
	}
  }
  else cout << "File could not be opened" << endl;
  return facets;
}

// main() used to test functionality of stlReader, is not needed for anything else
/* 
int main(){
  std::vector<Facet> facets = readFile("test.stl");
  for(int i = 0; i<facets.size(); i++){
	cout << facets[i];
  }
}
*/
