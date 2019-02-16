#include <Mesh.h>
#include <iostream>
#include <sstream>
#include <fstream>

// rozbijamy string s o delimiter t
std::vector<std::string> split(std::string s, std::string t) {
	std::vector<std::string> res;
	while (1) {
		int pos = s.find(t);
		if ( pos == -1 ) {
			res.push_back(s); 
			break;
		}
		res.push_back(s.substr(0, pos));
		s = s.substr(pos+1, s.size() - pos - 1);
	}
	return res;
}


Mesh::Mesh() :mLoaded(false) {
	// pusty konstruktor
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

bool Mesh::loadOBJ(const std::string &filename) {

	// tymczasowe kontenerki
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	// sprawdzamy czy mamy plik OBJ
	if (filename.find(".obj") != std::string::npos)
	{

		// otwieramy plik
		std::ifstream fin(filename, std::ios::in);
		// sprawdzamy czy sie plik otworzyl
		if (!fin)
		{
			std::cerr << "Cannot open " << filename << std::endl;
			return false;
		}

		// udalo sie otworzyc - wczytywanie
		std::cout << "Loading OBJ file " << filename << " ..." << std::endl;

		std::string lineBuffer;
		while (std::getline(fin, lineBuffer))
		{
			// wczytujemy plik OBJ liniami
			std::stringstream ss(lineBuffer);
			std::string cmd;
			ss >> cmd;

			// sprawdzamy TYP
			// v - wierzcholki
			if ( cmd == "v" )
			{
				glm::vec3 vertex;
				int dim = 0;
				while ( dim < 3 && ss >> vertex[dim] )
					dim++;

				tempVertices.push_back(vertex);
			}
			// vt - tekstury
			else if ( cmd == "vt" )
			{
				glm::vec2 uv;
				int dim = 0;
				while ( dim < 2 && ss >> uv[dim] )
					dim++;				
				tempUVs.push_back(uv);
			}
			// vn - normalne
			else if ( cmd == "vn" )
			{
				glm::vec3 normal;
				int dim = 0;
				while ( dim < 3 && ss >> normal[dim] )
					dim++;

				normal = glm::normalize(normal);
				tempNormals.push_back(normal);
			}
			// f - faces
			else if ( cmd == "f" )
			{
				std::string faceData;
				int vertexIndex, uvIndex, normalIndex;	// v/vt/vn

				while ( ss >> faceData ) {
					
					std::vector<std::string> data = split(faceData, "/");

					// wierzcholki
					if ( data[0].size() > 0 ) {
						sscanf(data[0].c_str(), "%d", &vertexIndex);
						vertexIndices.push_back(vertexIndex);
					}

					// jesli posiada koordynaty tekstury
					if ( data.size() >= 1 ) {
						if ( data[1].size() > 0 ) {
							sscanf(data[1].c_str(), "%d", &uvIndex);
							uvIndices.push_back(uvIndex);
						}
					}

					// jesli posiada wektor normalny
					if ( data.size() >= 2 ) {
						if ( data[2].size() > 0 ) {
							sscanf(data[2].c_str(), "%d", &normalIndex);
							normalIndices.push_back(normalIndex);
						}
					}
				}


				// STARY KOOOOOOD
/*				int p1, p2, p3; //to store mesh index
				int t1, t2, t3; //to store texture index
				int n1, n2, n3;
				const char* face = lineBuffer.c_str();

				// !!!!!!!!!!!!!! TUTAJ PODMIENILE sscanf_s na sscanf !!!!!!!!!!!!!!!!!!!!!!!!!!!
				int match = sscanf(face, "f %i/%i/%i %i/%i/%i %i/%i/%i",
					&p1, &t1, &n1,
					&p2, &t2, &n2,
					&p3, &t3, &n3);
				if (match != 9)
					std::cout << "Failed to parse OBJ file using our very simple OBJ loader" << std::endl;

				// We are ignoring normals (for now)

				vertexIndices.push_back(p1);
				vertexIndices.push_back(p2);
				vertexIndices.push_back(p3);

				uvIndices.push_back(t1);
				uvIndices.push_back(t2);
				uvIndices.push_back(t3);
*/
			}
		}

		// Zamykamy plik
		fin.close();


		// Dla kazdego wierzcholka kazdego trojkata
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			Vertex meshVertex;

			if ( tempVertices.size() > 0 ) {
				glm::vec3 vertex = tempVertices[vertexIndices[i] -1];
				meshVertex.position = vertex;
			}

			if ( tempNormals.size() > 0 ) {
				glm::vec3 normal = tempNormals[normalIndices[i] -1];
				meshVertex.normal = normal;
			}

			if ( tempUVs.size() > 0 ) {
				glm::vec2 uv = tempUVs[uvIndices[i] -1];
				meshVertex.texCoords = uv;
			}

			mVertices.push_back(meshVertex);


			// STARY KOD
/*			// Get the attributes using the indices
			glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
			glm::vec2 uv = tempUVs[uvIndices[i] - 1];

			Vertex meshVertex;
			meshVertex.position = vertex;
			meshVertex.texCoords = uv;

			mVertices.push_back(meshVertex);
*/
		}

		// Tworzymy i inicjujemy Buffory
		initBuffers();

		return (mLoaded = true);
	}

	// We shouldn't get here so return failure
	return false;
}

void Mesh::draw() {

	// upewniamy sie ze mamy czym rysowac
	if ( !mLoaded ) { return; }

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

	glBindVertexArray(0);
}

void Mesh::initBuffers() {

	// generujemy obiekty
	glGenBuffers(1, &mVBO);
	// wypelniamy buffor naszym arrayem
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

	// tworzymy buffor tablic i ustawiamy go jako biezacy
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// przekazujemy dane dla shadera wierzcholkow
	// pozycja
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(0);

	// normalne
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// tex coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// odlaczenie zeby sie nie psulo nic
	glBindVertexArray(0);
}