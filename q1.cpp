#include <iostream>
#include <string>

/*
    Abstract base class representing a generic vehicle.
    Provides interface for rental cost calculation and displaying vehicle info.
*/
class Vehicle
{
private:
    std::string make;   // Manufacturer of the vehicle
    std::string model;  // Model of the vehicle
    int year;           // Year of manufacture

public:
    // Constructor to initialize vehicle details
    Vehicle(std::string mk, std::string mdl, int yr)
        : make(mk), model(mdl), year(yr) {};
        
    virtual ~Vehicle() {}

    // Pure virtual function to calculate rental cost for a given number of days
    virtual int calculateRentalCost(int days) = 0;

    // Virtual function to display vehicle information
    virtual void displayInfo()
    {
        std::cout << "Make: " << make << ", Model: " << model << ", Year: " << year << std::endl;
    }
};

/*
    Derived class representing a Car.
    Adds number of doors as a specific property.
*/
class Car : public Vehicle
{
private:
    int numDoors;   // Number of doors in the car

public:
    // Constructor to initialize car details
    Car(std::string mk, std::string mdl, int yr, int doors)
        : Vehicle(mk, mdl, yr), numDoors(doors) {}

    // Calculates rental cost for a car (flat rate per day)
    int calculateRentalCost(int days) override
    {
        return days * 2000; // Flat rate per day
    }

    // Displays car information including number of doors
    void displayInfo() override
    {
        Vehicle::displayInfo();
        std::cout << "Type: Car, Doors: " << numDoors << std::endl;
    }
};

/*
    Derived class representing an SUV.
    Adds four-wheel drive property.
*/
class SUV : public Vehicle
{
private:
    bool fourWheelDrive;    // Indicates if the SUV has 4WD

public:
    // Constructor to initialize SUV details
    SUV(std::string mk, std::string mdl, int yr, bool fwd)
        : Vehicle(mk, mdl, yr), fourWheelDrive(fwd) {}

    // Calculates rental cost for an SUV (higher if 4WD)
    int calculateRentalCost(int days) override
    {
        return days * (fourWheelDrive ? 3000 : 2000);
    }

    // Displays SUV information including 4WD status
    void displayInfo() override
    {
        Vehicle::displayInfo();
        std::cout << "Type: SUV, 4WD: " << (fourWheelDrive ? "Yes" : "No") << std::endl;
    }
};

/*
    Derived class representing a Truck.
    Adds cargo capacity property.
*/
class Truck : public Vehicle
{
private:
    double cargoCapacity;   // Cargo capacity in tons

public:
    // Constructor to initialize truck details
    Truck(std::string mk, std::string mdl, int yr, double capacity)
        : Vehicle(mk, mdl, yr), cargoCapacity(capacity) {}

    // Calculates rental cost for a truck (flat rate per day)
    int calculateRentalCost(int days) override
    {
        return days * 2500;
    }

    // Displays truck information including cargo capacity
    void displayInfo() override
    {
        Vehicle::displayInfo();
        std::cout << "Type: Truck, Cargo Capacity: " << cargoCapacity << " tons" << std::endl;
    }
};

int main()
{
    // Create instances of each vehicle type
    Car myCar("Toyota", "Corolla", 2022, 4);
    SUV mySUV("Ford", "Explorer", 2021, true);
    Truck myTruck("Volvo", "FH16", 2020, 10.5);

    // Store pointers to vehicles in an array for polymorphic access
    Vehicle *fleet[] = {&myCar, &mySUV, &myTruck};

    // Display info and rental cost for each vehicle
    for (Vehicle *v : fleet)
    {
        v->displayInfo();
        std::cout << "Rental Cost for 3 days: ksh" << v->calculateRentalCost(3) << std::endl;
    }

    return 0;
}
