#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono> 

using namespace std;
using namespace std::chrono;

////////////////////////////////////////////////// FUNCTIONS ////////////////////////////////////////////////////////

void transpose(int m, int n, double** a, double** t) {
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			t[j][i] = a[i][j];
}

void multiply(int m, int n, int p, double** a, double** b, double** c) {
	for (int i = 0; i < m; i++)
		for (int j = 0; j < p; j++) {
			c[i][j] = 0;
			for (int k = 0; k < n; k++)
				c[i][j] += a[i][k] * b[k][j];
		}
}

//sigmoid < -function(z) { 1.0 / (1 + exp(-z)) }
void sigmoid(int n, double **z, double** o) {
	for (int i = 0; i < n; i++)
		o[i][0] = 1 / (1 + exp((z[i][0])*-1));
}

int main() {
	
/////////////////////////////////////////////// READ FILE //////////////////////////////////////////////////////////

	ifstream ip("plasma.csv");

	if (!ip.is_open())
		cout << "Error in reading file" << '\n';

	string fibrinogenTemp, globulenTemp, ESRTemp;
	vector<float> fibrinogen;
	vector<int> globulen;
	vector<string> ESR;
	int data_count = 0;

	//loop that extracts datapoints using commas as delimiters.
	getline(ip, fibrinogenTemp, ',');
	getline(ip, globulenTemp, '\n'); //rid of column names
	while (ip.good()) {
		getline(ip, fibrinogenTemp, ',');
		getline(ip, globulenTemp, ',');
		getline(ip, ESRTemp, '\n');
		//push the row's data at end of vector.
		if (!(fibrinogenTemp == "")) {
			fibrinogen.push_back(stof(fibrinogenTemp));
			globulen.push_back(stoi(globulenTemp));
			ESR.push_back(ESRTemp);
			data_count++;
		}
	}

///////////////////////////////////////////////// ALGORITHM ///////////////////////////////////////////////////////////
	
	vector<float> predictor = fibrinogen;
	vector<string> target = ESR;

	auto start = high_resolution_clock::now(); //TIMER START!!

	//weights <- c(1, 1)
	double** weights = new double* [2];
	weights[0] = new double[1];
	weights[1] = new double[1];
	weights[0][0] = weights[1][0] = 1;

	//data_matrix <- cbind(rep(1, nrow(train)), train$fibrinogen)
	double** data_matrix = new double* [data_count];
	for (int i = 0; i < data_count; i++) {
		data_matrix[i] = new double[2];
		data_matrix[i][0] = 1;
		data_matrix[i][1] = predictor[i];
	}

	//labels <- as.integer(train$ESR) - 1
	int* labels = new int[data_count];
	for (int i = 0; i < data_count; i++)
		labels[i] = (target[i] == "\"ESR > 20\"");

	double learning_rate = .001;
	
	//ALLOCATE to cut time by a factor of 7
	double** prob_vector = new double*[data_count];
	for (int i = 0; i < data_count; i++)
		prob_vector[i] = new double[1];

	double** error_vector = prob_vector;
	double** c = prob_vector;

	double** tpose = new double* [2];
	tpose[0] = new double[data_count];
	tpose[1] = new double[data_count];

	double w1, w2;
	for (int i = 0; i < 500000; i++) {
		
		//prob_vector <- sigmoid(data_matrix %*% weights)
		multiply(data_count, 2, 1, data_matrix, weights, c);
		sigmoid(data_count, c, prob_vector);
		
		//error <- labels - prob_vector
		for (int i = 0; i < data_count; i++)
			error_vector[i][0] = labels[i] - prob_vector[i][0];

		//weights <- weights + learning_rate * t(data_matrix) %*% error
		w1 = weights[0][0];
		w2 = weights[1][0];
		transpose(data_count, 2, data_matrix, tpose);
		multiply(2, data_count, 1, tpose, error_vector, weights);
		weights[0][0] *= learning_rate;
		weights[1][0] *= learning_rate;
		weights[0][0] += w1;
		weights[1][0] += w2;

	}

	auto stop = high_resolution_clock::now();//TIMER STOP!!!

	cout << weights[0][0] << endl;
	cout << weights[1][0] << endl;

	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Time elapsed: " << duration.count() << " milliseconds" << endl;

	return 0;
}