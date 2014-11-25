#ifndef CONFIG_PARAMS_H_
#define CONFIG_PARAMS_H_

#define SodaCost             2
#define NumStudents          2
#define MaxPurchases         8
#define NumVendingMachines   3
#define MaxStockPerFlavour   5
#define MaxShippedPerFlavour 3
#define TimeBetweenShipments 3
#define ParentalDelay        2
#define NumCouriers          1

struct ConfigParms {
    unsigned int sodaCost;                 // MSRP per bottle
    unsigned int numStudents;              // number of students to create
    unsigned int maxPurchases;             // maximum number of bottles a student purchases
    unsigned int numVendingMachines;       // number of vending machines
    unsigned int maxStockPerFlavour;       // maximum number of bottles of each flavour stocked
    unsigned int maxShippedPerFlavour;     // number of bottles of each flavour in a shipment
    unsigned int timeBetweenShipments;     // length of time between shipment pickup
    unsigned int parentalDelay;            // length of time between cash deposits
    unsigned int numCouriers;              // number of couriers in the pool
};

#endif
