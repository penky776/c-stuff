#include <stdio.h>
#include <stdlib.h>

int getIntRandRange(int min, int max){
	if (min > max){
		printf("wrong usage of getIntRandRange(): max must be greater than min");
		exit(-1);
	}
	int retval = (min + rand()/(RAND_MAX/(max-min)));
	return retval;
}

float getFloatRandRange(float min, float max) {
	if (min > max){
		printf("wrong usage of getFloatRandRange(): max must be greater than min");
		exit(-1);
	}
	float retval = (min + (float)rand()/(RAND_MAX/(max-min)));
	return retval;
}

typedef enum {
	ice, ev, hybrid
} engine_type;

typedef enum {
    tesla, bmw, toyota, mercedes_benz, hyundai, volkswagen, nissan
} car_make;

typedef struct
{
    float ice_efficiency; // l/100km
    int ev_efficiency;      // kWh/100km
} hybrid_efficiency;

typedef union {
    float ice;
    int ev; 
    hybrid_efficiency hybrid;
} efficiency;

typedef struct {
    engine_type engine_type;
    car_make car_make;
    efficiency efficiency;
    int max_torque;
} Car;

engine_type getRandomEngineType(void){
	return (engine_type)getIntRandRange(0, 2);
}

car_make get_random_car_type(void) {
    return (car_make)getIntRandRange(0, 6);
}

void write_file(char filename[], Car cars[1000]) {
    FILE *fp;
    fp = fopen(filename, "wb");
    if(fp == NULL)
    {
        printf("Cannot create file.\n");
        return;
    }

    for (int n=0; n<1000; n++) {
        fwrite(&cars[n], sizeof(Car), 1, fp);
    }

    fclose(fp);
}

const char* engineTypeToStr(engine_type t) {
    switch (t) {
        case ice: return "ICE";
        case ev: return "EV";
        case hybrid: return "HYBRID";
        default: return "UNKNOWN";
    }
}

const char* carMakeToStr(car_make m) {
    switch (m) {
        case tesla: return "Tesla";
        case bmw: return "BMW";
        case toyota: return "Toyota";
        case mercedes_benz: return "Mercedes-Benz";
        case hyundai: return "Hyundai";
        case volkswagen: return "Volkswagen";
        case nissan: return "Nissan";
        default: return "Unknown";
    }
}

int main(int argc, char* argv[]) {
    // generate cars
    Car cars[1000];
    
    for (int n=0; n<1000; n++) {
        Car car;
        car.car_make = get_random_car_type();
        car.engine_type = getRandomEngineType();
        car.max_torque = getIntRandRange(250, 3800);
        efficiency eff;
        switch(car.engine_type) {
            case ice: {
                eff.ice = getFloatRandRange(3.3, 8.5);
            }
            break;
            case ev: {
                eff.ev = getIntRandRange(12, 18);
            }
            break;
            case hybrid: {
                hybrid_efficiency h;
                h.ev_efficiency = getIntRandRange(12, 18);
                h.ice_efficiency = getFloatRandRange(3.3, 8.5);
                eff.hybrid = h;
            }
        }
        car.efficiency = eff;

        cars[n] = car;
    }

    write_file(argv[1], cars);
    
    FILE *fp = fopen(argv[1], "rb");
    int make_count[7] = {0};
    int engine_count[3] = {0};

    Car best_ice = {0};
    Car best_ev = {0};
    Car best_hybrid = {0};

    Car best_torque_ice = {0};
    Car best_torque_ev = {0};
    Car best_torque_hybrid = {0};

    int ice_set = 0, ev_set = 0, hybrid_set = 0;
    int ti = 0, te = 0, th = 0;

    Car c;
    printf("Car makes(quantity): \n");
    while (fread(&c, sizeof(Car), 1, fp)) { 
        make_count[c.car_make]++;
        engine_count[c.engine_type]++;

        switch (c.engine_type) {
            case ice:
                if (!ice_set || c.efficiency.ice < best_ice.efficiency.ice) {
                    best_ice = c;
                    ice_set = 1;
                }
                if (!ti || c.max_torque > best_torque_ice.max_torque) {
                    best_torque_ice = c;
                    ti = 1;
                }
                break;

            case ev:
                if (!ev_set || c.efficiency.ev > best_ev.efficiency.ev) {
                    best_ev = c;
                    ev_set = 1;
                }
                if (!te || c.max_torque > best_torque_ev.max_torque) {
                    best_torque_ev = c;
                    te = 1;
                }
                break;

            case hybrid:
                if (!hybrid_set) {
                    best_hybrid = c;
                    hybrid_set = 1;
                } else if (c.efficiency.hybrid.ice_efficiency <
                        best_hybrid.efficiency.hybrid.ice_efficiency) {
                    best_hybrid = c;
                }
                break;
        }

    }

    fclose(fp);
    printf("Car makes (quantity):\n");
    printf("Tesla BMW Toyota Mercedes-Benz Hyundai Volkswagen Nissan\n");
    printf("-------------------------------------------------------\n");
    printf("%d %d %d %d %d %d %d\n\n",
        make_count[tesla],
        make_count[bmw],
        make_count[toyota],
        make_count[mercedes_benz],
        make_count[hyundai],
        make_count[volkswagen],
        make_count[nissan]);

    printf("Engine types (quantity):\n");
    printf("ICE EV Hybrid\n");
    printf("----------------------\n");
    printf("%d %d %d\n\n",
        engine_count[ice],
        engine_count[ev],
        engine_count[hybrid]);

    printf("Best engine efficiency\n");
    printf("----------------------\n");
    printf("ICE: %s %.1f l/100km\n",
        carMakeToStr(best_ice.car_make),
        best_ice.efficiency.ice);

    printf("EV: %s %d kWh/100km\n",
        carMakeToStr(best_ev.car_make),
        best_ev.efficiency.ev);

    printf("Hybrid: %s %.1f l/100km and %d kWh/100km\n",
        carMakeToStr(best_hybrid.car_make),
        best_hybrid.efficiency.hybrid.ice_efficiency,
        best_hybrid.efficiency.hybrid.ev_efficiency);

    printf("\nHighest maximum torque\n");
    printf("----------------------\n");

    printf("ICE: %s %d Nm\n",
        carMakeToStr(best_torque_ice.car_make),
        best_torque_ice.max_torque);

    printf("EV: %s %d Nm\n",
        carMakeToStr(best_torque_ev.car_make),
        best_torque_ev.max_torque);

    printf("Hybrid: %s %d Nm\n",
        carMakeToStr(best_torque_hybrid.car_make),
        best_torque_hybrid.max_torque);
        
    // debug
    // for (int i = 0; i < 1000; i++) {
    //     printf("%s | %s | torque=%d\n",
    //         engineTypeToStr(cars[i].engine_type),
    //         carMakeToStr(cars[i].car_make),
    //         cars[i].max_torque);
    // }

}