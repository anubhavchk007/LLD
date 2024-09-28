#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

enum class VehicleType {CAR, MOTORCYCLE, TRUCK};


class Vehicle {
    VehicleType type;
    std::string vehicleID;
public:
    Vehicle(VehicleType vehicletype, std::string vehicleID) : type(vehicletype), vehicleID(vehicleID) {}

    VehicleType getVehicleType() {
        return type;
    }

    std::string getVehicleID() {
        return vehicleID;
    }
};

class Car : public Vehicle {
public:
    Car(std::string vehicleID) : Vehicle(VehicleType::CAR, vehicleID) {}
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(std::string vehicleID) : Vehicle(VehicleType::MOTORCYCLE, vehicleID) {}
};

class Truck : public Vehicle {
public:
    Truck(std::string vehicleID) : Vehicle(VehicleType::TRUCK, vehicleID) {}
};



class ParkingSpot {
    int spotNumber;
    VehicleType spotType;
    bool isOccupied;
    std::string vehicleID;

public:
    ParkingSpot(int spotNumber, VehicleType spotType): spotNumber(spotNumber), spotType(spotType), isOccupied(false), vehicleID("") {}

    bool isFree() {
        return not isOccupied;
    }

    std::string getVehicleID() {
        return vehicleID;
    }

    VehicleType getSpotType() const {
        return spotType;
    }

    void parkVehicle(Vehicle& vehicle) {
        isOccupied = true;
        vehicleID = vehicle.getVehicleID();
    }

    void unparkVehicle () {
        isOccupied = false;
        vehicleID = "";
    }
};

class ParkingLevel {
    int levelNumber;
    std::vector<ParkingSpot> parkingSpots;

public:
    ParkingLevel(int level, int spotsPerLevel, std::unordered_map<VehicleType, int>& spotDistribution) : levelNumber(level) {
        for (int i = 1; i <= spotsPerLevel; i++) {
            if (i <= spotDistribution[VehicleType::CAR]) {
                parkingSpots.emplace_back(i, VehicleType::CAR);
            } else if (i <= spotDistribution[VehicleType::MOTORCYCLE] + spotDistribution[VehicleType::CAR]) {
                parkingSpots.emplace_back(i, VehicleType::MOTORCYCLE);
            } else {
                parkingSpots.emplace_back(i, VehicleType::TRUCK);
            }
        }
    }

    int getLevelNumber() {
        return levelNumber;
    }

    bool parkVehicle(Vehicle& vehicle) {
        for (auto& spot : parkingSpots) {
            if (spot.isFree() and spot.getSpotType() == vehicle.getVehicleType()) {
                spot.parkVehicle(vehicle);
                return true;
            }
        }
        return false;
    }

    bool unparkVehicle (Vehicle& vehicle) {
        for (auto &spot : parkingSpots) {
            if (not spot.isFree() and spot.getVehicleID() == vehicle.getVehicleID()) {
                spot.unparkVehicle();
                return true;
            }
        }
        return false;
    }
};

class ParkingLot {
    std::vector<ParkingLevel> levels;
public:
    ParkingLot(int numLevels, int spotsPerLevel, std::unordered_map<VehicleType, int>& spotDistribution) {
        for (int i = 1; i <= numLevels; i++) {
            levels.emplace_back(i, spotsPerLevel, spotDistribution);
        }
    }

    bool parkVehicle(Vehicle& vehicle) {
        for (auto& level : levels) {
            if (level.parkVehicle(vehicle)) {
                std::cout << "Vehicle " << vehicle.getVehicleID() << " parked in level " << level.getLevelNumber() << std::endl;
                return true;
            }
        }
        std::cout << "There are no empty parking spots." << std::endl;
        return false;
    }

    bool unparkVehicle(Vehicle& vehicle) {
        for (auto& level : levels) {
            if (level.unparkVehicle(vehicle)) {
                std::cout << "The vehicle " << vehicle.getVehicleID() << " parked at level " << level.getLevelNumber() << " is unparked" << std::endl;
                return true;
            }
        }
        std::cout << "Couldn't find the vehicle " << vehicle.getVehicleID() << std::endl;
        return false;
    }
};



int main() {
    std::unordered_map<VehicleType, int> spotDistribution = {
        {VehicleType::CAR, 2},
        {VehicleType::MOTORCYCLE, 0},
        {VehicleType::TRUCK, 1}
    };
    ParkingLot parkinglot(2, 31, spotDistribution);
    Car c1("0"), c2("1"), c3{"2"};
    Truck t1("3"), t2("4"), t3("5");
    Motorcycle m1("5"), m2("6");

    parkinglot.parkVehicle(c1);
    parkinglot.parkVehicle(c2);
    parkinglot.parkVehicle(c3);
    parkinglot.parkVehicle(t1);
    parkinglot.parkVehicle(t2);
    parkinglot.unparkVehicle(t1);
    parkinglot.parkVehicle(t3);
    parkinglot.parkVehicle(m1);
    parkinglot.parkVehicle(m2);

    return 0;
}
