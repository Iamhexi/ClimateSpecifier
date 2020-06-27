#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h>

using namespace std;

enum ClimateType { // Koppen climate classification: https://en.wikipedia.org/wiki/K%C3%B6ppen_climate_classification
    tropical = 0,
    dry = 1,
    temperate = 2,
    continental = 3,
    polar = 4,
    UNIDENTIFIED = -1
};

struct ClimateVector { // each value is yearly
    ClimateType climateType;
    double avgTemp;
    double minTemp;
    double maxTemp;
    double precip;
};


vector <ClimateVector> climates;
const short int HOW_MANY_NEAREST_NEIGHBOURS = 3;
string DATA_FILENAME = "datapoints.txt";



bool loadFromFile(){
    ifstream input(DATA_FILENAME);

    if (!input.good())
        return false;

    double currentVal;
    int climateTypeAsInt;

    while (!input.eof()){

        ClimateVector climate;

        input >> climateTypeAsInt;
        ClimateType currentClimateType = static_cast<ClimateType>(climateTypeAsInt);
        climate.climateType = currentClimateType;

        input >> currentVal;
        climate.avgTemp = currentVal;
        input >> currentVal;
        climate.minTemp = currentVal;
        input >> currentVal;
        climate.maxTemp = currentVal;
        input >> currentVal;
        climate.precip = currentVal;

        climates.push_back(climate);
    }

    return true;
}

double calcDistanceBetweenVectors(ClimateVector climateA, ClimateVector climateB){
    double distance = pow(climateB.avgTemp - climateA.avgTemp, 2);
    distance += pow(climateB.minTemp - climateA.minTemp, 2);
    distance += pow(climateB.maxTemp - climateA.maxTemp, 2);
    distance += pow(climateB.precip - climateB.precip, 2);

    return sqrt(distance);
}

void showDatapoints(){
    int size = climates.size();

    for (int i=0;i<size;i++){
        cout << "Datapoint ID: " << i+1 << "\n";
        cout << "Average temperature: " << climates[i].avgTemp << " st. C\n";
        cout << "Min temperature: " << climates[i].minTemp << " st. C\n";
        cout << "Max temperature: " << climates[i].maxTemp << " st. C\n";
        cout << "Rainfall (yearly): " << climates[i].precip << " mm\n";
        cout << "\n";
    }
}

ClimateType specifyClimateType(double avgTemp, double minTemp, double maxTemp, double precip){
    int size = climates.size();
    vector <double> distances;
    distances.clear();

    ClimateVector soughtClimate;
    soughtClimate.avgTemp = avgTemp;
    soughtClimate.minTemp = minTemp;
    soughtClimate.maxTemp = maxTemp;
    soughtClimate.precip = precip;

    for (int i=0;i<size;i++)
        distances.push_back(calcDistanceBetweenVectors(soughtClimate, climates[i]));

    sort(distances.begin(), distances.end());

    ClimateType kNearestNeighbours[HOW_MANY_NEAREST_NEIGHBOURS];
    short int index = 0;

    for (int i=0;i<size;i++){
        for (int j=0;j<3;j++)
            if (calcDistanceBetweenVectors(soughtClimate, climates[i]) == distances[j]){
                kNearestNeighbours[index] = climates[i].climateType;
                index++;
            }

    }


    for (int i=0;i<HOW_MANY_NEAREST_NEIGHBOURS;i++)
        for (int j=0;j<HOW_MANY_NEAREST_NEIGHBOURS;j++)
            if (kNearestNeighbours[i] != kNearestNeighbours[j])
                return UNIDENTIFIED;

    return kNearestNeighbours[0];
}

string convertClimateTypeVectorToString(ClimateType climateType){
    if (climateType == ClimateType::polar)
        return "Polar";
    else if (climateType == ClimateType::dry)
        return "Dry";
    else if (climateType == ClimateType::continental)
        return "Continental";
    else if (climateType == ClimateType::tropical)
        return "Tropical";
    else if (climateType == ClimateType::temperate)
        return "Temperate";
    else
        return "Could not indentify the climate type!";
}

int main(){

    if (!loadFromFile())
        cout << "Couldn't load datapoints!\n";

    double meanTemp, maxTemp, minTemp, avgRainfall;

    cout << "The ML algorithm specifies the type of climate according to the provided weather data from this location.\n";

    cout << "\nGive the yearly mean temperature: ";
    cin >> meanTemp;

    cout << "\nGive the yearly average of the minimum monthly temperatures: ";
    cin >> minTemp;

    cout << "\nGive the yearly average of the maximum monthly temperatures: ";
    cin >> maxTemp;

    cout << "\nGive the yearly average of the monthly precipitation: ";
    cin >> avgRainfall;

    cout << "\nThe climate in the given location is " <<
     convertClimateTypeVectorToString(specifyClimateType(meanTemp, minTemp, maxTemp, avgRainfall));


    return 0;
}
