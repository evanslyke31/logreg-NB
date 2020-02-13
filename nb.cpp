#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

double calc_age_lh(int v, double mean, double var) {
	return (1 / (sqrt(M_PI * 2 * var)) * exp(-pow(v - mean,2)/(2 * var)));
}

int main() {

///////////////////////////////////////////////// READ FILE ////////////////////////////////////////////////////

	ifstream ip("titanic_project.csv");

	if (!ip.is_open())
		cout << "Error in reading file" << '\n';

	string pnumTemp, pclassTemp, survivedTemp, sexTemp, ageTemp;
	vector<int> pnum;
	vector<int> pclass;
	vector<bool> survived;
	vector<bool> sex;
	vector<int> age;
	int data_count = 0;

	//loop that extracts datapoints using commas as delimiters.
	getline(ip, pnumTemp, ',');
	getline(ip, pclassTemp, ',');
	getline(ip, survivedTemp, ',');
	getline(ip, sexTemp, ',');
	getline(ip, ageTemp, '\n'); //rid of column names
	while (ip.good()) {
		getline(ip, pnumTemp, ',');
		getline(ip, pclassTemp, ',');
		getline(ip, survivedTemp, ',');
		getline(ip, sexTemp, ',');
		getline(ip, ageTemp, '\n');
		//push the row's data at end of vector.
		if (!(pnumTemp == "")) {
			pnumTemp.erase(remove(pnumTemp.begin(), pnumTemp.end(), '\"'), pnumTemp.end());
			pnum.push_back(stoi(pnumTemp));
			pclass.push_back(stoi(pclassTemp));
			survived.push_back(survivedTemp == "1");
			sex.push_back(sexTemp == "1");
			age.push_back(stoi(ageTemp));
			data_count++;
		}
	}

	vector<bool> survivedTrain, survivedTest;
	vector<int> pclassTrain, pclassTest, sexTrain, sexTest, ageTrain, ageTest;
	for (int i = 0; i < 900; i++) {
		survivedTrain.push_back(survived[i]);
		pclassTrain.push_back(pclass[i]);
		sexTrain.push_back(sex[i]);
		ageTrain.push_back(age[i]);
	}
	for (int i = 900; i < data_count; i++) {
		survivedTest.push_back(survived[i]);
		pclassTest.push_back(pclass[i]);
		sexTest.push_back(sex[i]);
		ageTest.push_back(age[i]);
	}

/////////////////////////////////////////////// ALGORITHM //////////////////////////////////////////////////////

	double apriori[2] = {0,0};
	for (int i = 0; i < survivedTrain.size(); i++)
		if (survivedTrain[i])
			apriori[1]++;
		else
			apriori[0]++;
	apriori[0] /= survivedTrain.size();
	apriori[1] /= survivedTrain.size();

	//cout << apriori[0] << endl;
	//cout << apriori[1] << endl;

	//get survived counts for no and yes
	int count_survived[2] = {0,0};
	count_survived[0] = count_survived[1] = 0;
	for (int i = 0; i < survivedTrain.size(); i++)
		if (survivedTrain[i])
			count_survived[1]++;
		else
			count_survived[0]++;

	//cout << count_survived[0] << endl;
	//cout << count_survived[1] << endl;

	//liklihood for pclass
	double lh_pclass[2][3] = { {0,0,0},{0,0,0} };
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < survivedTrain.size(); k++) {
				if (survivedTrain[k] == i && pclassTrain[k] == j + 1)
					lh_pclass[i][j]++;
			}
			lh_pclass[i][j] /= count_survived[i];
			//cout << lh_pclass[i][j] << endl;
		}
	}

	//Likelihood for sex (not much)
	double lh_sex[2][2] = { {0,0},{0,0} };
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < sexTrain.size(); k++) {
				if (survivedTrain[k] == i && sexTrain[k] == j)
					lh_sex[i][j]++;
			}
			lh_sex[i][j] /= count_survived[i];
			//cout << lh_sex[i][j] << endl;
		}
	}

	//Liklihood for continuous data (age)
	double age_mean[2] = {0,0};
	double age_var[2] = {0,0};
	for(int i = 0; i < 2; i++) {
		int c = 0;
		for(int j = 0; j < ageTrain.size(); j++) {
			if(survivedTrain[j] == i) {
				age_mean[i] += ageTrain[j];
				c++;
			}
		}
		age_mean[i] /= c;
		c = 0;
		for(int j = 0; j < ageTrain.size(); j++) {
			if(survivedTrain[j] == i) {
				age_var[i] += pow(ageTrain[j] - age_mean[i], 2);
				c++;
			}
			
		}
		age_var[i] /= c;
		//cout << age_mean[i] << endl;
		//cout << age_var[i] << endl;
	}

	for(int i = 0; i < 10; i++) {
		int sexV = sexTest[i];
		int pclassV = pclassTest[i];
		int ageV = ageTest[i];
		double num_s = lh_pclass[1][pclassV - 1] * lh_sex[1][sexV] * apriori[1]
				* calc_age_lh(ageV, age_mean[1], age_var[1]);
		double num_p = lh_pclass[0][pclassV - 1] * lh_sex[0][sexV] * apriori[0]
				* calc_age_lh(ageV, age_mean[0], age_var[0]);
		double denominator = num_s + num_p; 
		cout << " " << (num_p/denominator) << (num_s/denominator) <<  endl;
		//cout << sexV << " " << pclassV << " " << ageV << " " << endl;
	}
	return 0;

}