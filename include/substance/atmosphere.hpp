#ifndef _ATMOSPHERE_HPP_
#define _ATMOSPHERE_HPP_
#pragma once

#include "type_define.hpp"
#include <array>
#include <cmath>

namespace carpio {

class Atmosphere{

protected:
	Vt _altitude;
	Vt _altitude_geopotential;
	Vt _density;
	Vt _temperature;
	Vt _pressure;
	Vt _speed_of_sound;
	Vt _mean_molecular_weight;
	Vt _gravity;
	Vt _dynamic_viscosity;

public:
	Atmosphere() : _altitude(0){
		_initial();
	}

	Atmosphere(Vt Z) : _altitude(Z){
		ASSERT(_check_altitude(Z));
		_initial();
	}
	Atmosphere(const Atmosphere& atmos){
		_altitude              = atmos._altitude;
		_altitude_geopotential = atmos._altitude_geopotential;
		_density               = atmos._density;
		_temperature           = atmos._temperature;
		_pressure              = atmos._pressure;
		_speed_of_sound        = atmos._speed_of_sound;
		_mean_molecular_weight = atmos._mean_molecular_weight;
		_gravity               = atmos._gravity;
		_dynamic_viscosity     = atmos._dynamic_viscosity;
		_initial_Pb();
		_initial_Tmb();
	}

	void set_altitude(Vt Z){
		ASSERT(_check_altitude(Z));
		_altitude = Z;
		_refresh();
	}

	Vt altitude() const{
		return _altitude;
	}

	Vt geo_potential_altitude() const{
		return _altitude_geopotential;
	}

	Vt geo_potential_altitude(Vt Z) const{
		return r0 * Z / (r0 + Z);
	}

	Vt molecular_scale_temperature() const{
		return _Tm(_altitude_geopotential);
	}

	Vt mean_molecular_weight() const{
		return _mean_molecular_weight;
	}

	Vt pressure() const{
		return _pressure;
	}

	Vt temperature() const{
		return _temperature;
	}

	Vt speed_of_sound() const{
		return _speed_of_sound;
	}
	Vt density() const{
		return _density;
	}

	Vt gravity(Vt Z) const{
		return g0 * std::pow(r0 / (r0 + Z), 2.0);
	}

	Vt gravity() const{
		return _gravity;
	}

	Vt dynamic_viscosity() const{
		return _dynamic_viscosity;
	}

	Vt kinematic_viscosity() const{
		return dynamic_viscosity() * density();
	}

	Vt coeffiecient_of_thermal_conductivity() const{
		Vt t = temperature();
		return (2.64638e-3 * std::pow(t, 1.5)) / (t + 245.4 * std::pow(10.0, -(12/t)));
	}

protected:
	void _refresh() {
		_altitude_geopotential = geo_potential_altitude(_altitude);
		if (_altitude <= 86e3) {
			_mean_molecular_weight = _mean_molecular_weight_lower(_altitude);
			_temperature = _temperature_lower(_altitude_geopotential,
					_mean_molecular_weight);
			_pressure = _pressure_lower(_altitude_geopotential);
			_speed_of_sound = _speed_of_sound_lower(_temperature,
					_mean_molecular_weight);
			_dynamic_viscosity = _dynamic_viscosity_lower(_temperature);
		} else {
			_mean_molecular_weight = _mean_molecular_weight_upper(_altitude);
			_pressure = _pressure_upper(_altitude);
			_speed_of_sound = _speed_of_sound_86km();
		}
		_gravity = gravity(_altitude);
		_density = pressure() * mean_molecular_weight()
				/ (Rstar * temperature());
	}


	void _initial() {
		_initial_Tmb();
		_initial_Pb();
		_refresh();
	}

	Vt _temperature_lower(Vt H, Vt M) const{
		return _Tm(H) / M0 * M;
	}

	Vt _pressure_lower(Vt H) const{
	    auto b = _find_b(H);
	    auto i = b;
	    if(Lmb[i] == 0){
	        return Pb[i] * std::exp(-g0 * M0 * (H - Hb[i]) / (Rstar * Tmb[i]));
	    }else{
	        return Pb[i] * std::pow( Tmb[i] / (Tmb[i] + Lmb[i] * (H - Hb[i])),
	        		                 (g0 * M0) / (Rstar * Lmb[i]));
	    }
	}

	Vt _speed_of_sound_lower(Vt T, Vt M) const{
		return std::sqrt(r * Rstar * T / M);
	}

	Vt _speed_of_sound_86km() const {
		Vt mw = _mean_molecular_weight_lower(86000);
		Vt ag = geo_potential_altitude(86000);
		Vt t = _temperature_lower(ag, mw);
		return _speed_of_sound_lower(t, mw);
	}

	Vt _mean_molecular_weight_ratio_lower(Vt Z) const{
		if(Z < Ztable[0]){
			return 1.0;
		}
	    ASSERT(Z <= Ztable[12]);
	    St i = 0;
	    while( i < 12 && Z > Ztable[i + 1]){
	        i += 1;
	    }
	    return Mratiotable[i]
			     + (Mratiotable[i + 1] - Mratiotable[i])
				 / (Ztable[i + 1] - Ztable[i]) * (Z - Ztable[i]);
	}

	Vt _mean_molecular_weight_lower(Vt Z) const{
		return M0 * _mean_molecular_weight_ratio_lower(Z);
	}

	Vt _dynamic_viscosity_lower(Vt T) const{
		Vt bata = 1.458e-6; //(kg/(s m k1/2)
		Vt S    = 110.4;    //(K)  Sutherland's constant
		return (bata * std::pow(T, 1.5))/(T + S);
	}

	Vt _temperature_upper(Vt Z){
	    if(Z <= 91e3){
	        return 186.8673; // (K)
	    }else if(Z <= 110000){
	        Vt Tc = 263.1905; // (K)
	        Vt A  = -76.3232;  // (K)
	        Vt a  = -19.9429 * 1000; // (m)
	        return Tc + A * std::pow(std::sqrt(1 - ((Z - 91000) / a)), 2);
	    }else if (Z <= 120e3) {
	        Vt T9  = 240; // (K)
	        Vt LK9 = 12 / 1000; // (K / m)
	        Vt Z9  = 110000; // (m)
	        return T9 + LK9 * (Z - Z9);
	    }else if (Z <= 1000e3) {
	        Vt T10   = 360; // (K)
	        Vt Z10   = 120000; // (m)
	        Vt Tinf  = 1000; // (K)
	        Vt lamda = 0.01875 / 1000; // (1 / m);
	        Vt E     = (Z - Z10) * (r0 + Z10) / (r0 + Z);
	        return Tinf - (Tinf - T10) * std::exp(-lamda * E);
	    }
	    SHOULD_NOT_REACH;
	    return 0.0;
	}

	St _interpolation_index(Vt Z){
	    // Find the index for the lower side of the altitude interval
	    St i = 0;
	    while(i < Ztableupper.size() && Z > Ztableupper[i+1]){
	        i += 1;
		}

	    // We are going to reference all elements from i - 1 to i + 1, so we need to
	    // adjust the index away from the boundaries
	    if( i == 0){
	    	i = 1;
	    }

	    return i;
	}

	Vt _pressure_upper(Vt Z){
		St i = _interpolation_index(Z);
		Vt Z0 = Ztableupper[i - 1];
		Vt Z1 = Ztableupper[i];
		Vt Z2 = Ztableupper[i + 1];

		Vt scale0 = (Z - Z1) * (Z - Z2) / ((Z0 - Z1) * (Z0 - Z2));
		Vt scale1 = (Z - Z0) * (Z - Z2) / ((Z1 - Z0) * (Z1 - Z2));
		Vt scale2 = (Z - Z0) * (Z - Z1) / ((Z2 - Z0) * (Z2 - Z1));
	    Vt logP0 = std::log(Ptableupper[i - 1]);
	    Vt logP1 = std::log(Ptableupper[i]);
	    Vt logP2 = std::log(Ptableupper[i + 1]);
	    Vt logP = logP0 * scale0 + logP1 * scale1 + logP2 * scale2;
	    return std::exp(logP);
	}

	Vt _mean_molecular_weight_upper(Vt Z) {
		St i = _interpolation_index(Z);
		Vt Z0 = Ztableupper[i - 1];
		Vt Z1 = Ztableupper[i];
		Vt Z2 = Ztableupper[i + 1];

		Vt scale0 = (Z - Z1) * (Z - Z2) / ((Z0 - Z1) * (Z0 - Z2));
		Vt scale1 = (Z - Z0) * (Z - Z2) / ((Z1 - Z0) * (Z1 - Z2));
		Vt scale2 = (Z - Z0) * (Z - Z1) / ((Z2 - Z0) * (Z2 - Z1));

		Vt M0 = Mtableupper[i - 1];
		Vt M1 = Mtableupper[i];
		Vt M2 = Mtableupper[i + 1];
		return M0 * scale0 + M1 * scale1 + M2 * scale2;
	}




	void _initial_Tmb(){
		Tmb[0] = 288.15; // (K)
		for (St i = 0; i < nHb - 1 ; i++) {
			Tmb[i + 1] = Tmb[i] + Lmb[i] * (Hb[i + 1] - Hb[i]);
		}
	}
	void _initial_Pb(){
		Pb[0] = 101325.0;
		for (St i = 0; i < nHb - 1; i++) {
		    if(Lmb[i] == 0){
		       Pb[i + 1] = Pb[i] * std::exp(-g0 * M0 * (Hb[i + 1] - Hb[i]) / (Rstar * Tmb[i]));
		    }else{
		       Pb[i + 1] = Pb[i] * std::pow(Tmb[i] / (Tmb[i] + Lmb[i] * (Hb[i + 1] - Hb[i])),
		    		                        (g0 * M0 / (Rstar * Lmb[i])));
		    }
		}
	}
	bool _check_altitude(Vt Z){
	    return Z >= -5000 && Z <= 1000000;
	}
	// Molecular-scale temperature
	Vt _Tm(Vt H) const{
		auto b = _find_b(H);
		auto i = b;
		return Tmb[i] + Lmb[i] * (H - Hb[i]);
	}
	St _find_b(Vt H) const{
	    St i = 1;
	    while(i < nHb && H > Hb[i]){
	        i += 1;
	    }
	    return i - 1;
	}

	//constant
	const double r0    = 6356766.0; // (m), effective Earth radius at 45 deg N latitude
	const double g0    = 9.80665;      // (m / s²) or (m² / s² m')
	const double M0    = 28.9644;      // (kg / kmol)
	const double Rstar = 8.31432e3; // (N m / kmol K)
	const double r     = 1.4;           // air constant

	const St nHb = 7;
	// table 4 The defined reference levels and gradients of
	//         the linearly segmented temperature-height from
	//         the surface to 86 geometric kilometers
	//      b = 1        2       3       4       5      6        7
	const std::array<Vt, 7> Hb =  // Geopotential height
			  {{ 0,       11000,  20000,  32000,  47000,  51000,  71000 }}; // (m')
	const std::array<Vt, 7> Lmb =  // Molecular-scale temperature gradient
			  {{ -6.5e-3, 0.0e-3, 1.0e-3, 2.8e-3, 0.0,   -2.8e-3, -2.0e-3 }}; // (K / m')

	const std::array<Vt, 13> Ztable = {{ 80.0e3, 80.5e3, 81.0e3, 81.5e3, 82.0e3,
			82.5e3, 83.0e3, 83.5e3, 84.0e3, 84.5e3, 85.0e3, 85.5e3, 86.0e3 }}; // (m)
	const std::array<Vt, 13> Mratiotable = {{ 1.0, 0.999996, 0.999989, 0.999971,
			0.999941, 0.999909, 0.999870, 0.999829, 0.999786, 0.999741,
			0.999694, 0.999641, 0.999579 }};
	std::array<Vt, 7> Tmb;
	std::array<Vt, 7> Pb;


     // based on David's code, which was based on Regan
	//M and log(P) are interpolated quadratically
    const std::vector<Vt> Ztableupper = {
    		86000.0,  87000.0, 88000.0,   89000.0,  90000.0,  91000.0,
			93000.0,  95000.0, 97000.0,   99000.0,  101000.0, 103000.0,
			105000.0, 107000.0, 109000.0, 110000.0, 111000.0, 112000.0, 113000.0, 114000.0, 115000.0,
			116000.0, 117000.0, 118000.0, 119000.0, 120000.0, 125000.0, 130000.0,
			135000.0, 140000.0, 145000.0, 150000.0, 160000.0, 170000.0, 180000.0,
			190000.0, 200000.0, 210000.0, 220000.0, 230000.0, 240000.0, 250000.0,
			260000.0, 270000.0, 280000.0, 290000.0, 300000.0, 310000.0, 320000.0,
			330000.0, 340000.0, 350000.0, 360000.0, 370000.0, 380000.0, 390000.0,
			400000.0, 410000.0, 420000.0, 430000.0, 440000.0, 450000.0, 460000.0,
			470000.0, 480000.0, 490000.0, 500000.0, 525000.0, 550000.0, 575000.0,
			600000.0, 625000.0, 650000.0, 675000.0, 700000.0, 725000.0, 750000.0,
			775000.0, 800000.0, 825000.0, 850000.0, 875000.0, 900000.0, 925000.0,
			950000.0, 975000.0, 1000000.0}; // (m)
    const std::vector<Vt> Ptableupper = {
    		3.7338E-1, 3.1259E-1, 2.6173E-1, 2.1919E-1, 1.8359E-1,
            1.5381E-1, 1.0801E-1, 7.5966E-2, 5.3571E-2, 3.7948E-2, 2.7192E-2,
            1.9742E-2, 1.4477E-2, 1.0751E-2, 8.1142E-3, 7.1042E-3, 6.2614E-3,
            5.5547E-3, 4.9570E-3, 4.4473E-3, 4.0096E-3, 3.6312E-3, 3.3022E-3,
            3.0144E-3, 2.7615E-3, 2.5382E-3, 1.7354E-3, 1.2505E-3, 9.3568E-4,
            7.2028E-4, 5.6691E-4, 4.5422E-4, 3.0395E-4, 2.1210E-4, 1.5271E-4,
            1.1266E-4, 8.4736E-5, 6.4756E-5, 5.0149E-5, 3.9276E-5, 3.1059E-5,
            2.4767E-5, 1.9894E-5, 1.6083E-5, 1.3076E-5, 1.0683E-5, 8.7704E-6,
            7.2285E-6, 5.9796E-6, 4.9630E-6, 4.1320E-6, 3.4498E-6, 2.8878E-6,
            2.4234E-6, 2.0384E-6, 1.7184E-6, 1.4518E-6, 1.2291E-6, 1.0427E-6,
            8.8645E-7, 7.5517E-7, 6.4468E-7, 5.5155E-7, 4.7292E-7, 4.0642E-7,
            3.5011E-7, 3.0236E-7, 2.1200E-7, 1.5137E-7, 1.1028E-7, 8.2130E-8,
            6.2601E-8, 4.8865E-8, 3.9048E-8, 3.1908E-8, 2.6611E-8, 2.2599E-8,
            1.9493E-8, 1.7036E-8, 1.5051E-8, 1.3415E-8, 1.2043E-8, 1.0873E-8,
            9.8635E-9, 8.9816E-9, 8.2043E-9, 7.5138E-9}; // (Pa)
    const std::vector<Vt> Mtableupper = {
    		28.95, 28.95, 28.94, 28.93, 28.91, 28.89, 28.82, 28.73,
			28.62, 28.48, 28.30, 28.10, 27.88, 27.64, 27.39, 27.27, 27.14, 27.02,
			26.90, 26.79, 26.68, 26.58, 26.48, 26.38, 26.29, 26.20, 25.80, 25.44,
        	25.09, 24.75, 24.42, 24.10, 23.49, 22.90, 22.34, 21.81, 21.30, 20.83,
			20.37, 19.95, 19.56, 19.19, 18.85, 18.53, 18.24, 17.97, 17.73, 17.50,
			17.29, 17.09, 16.91, 16.74, 16.57, 16.42, 16.27, 16.13, 15.98, 15.84,
			15.70, 15.55, 15.40, 15.25, 15.08, 14.91, 14.73, 14.54, 14.33, 13.76,
			13.09, 12.34, 11.51, 10.62, 9.72,  8.83, 8.00, 7.24, 6.58, 6.01, 5.54, 5.16,
			4.85,  4.60,  4.40,  4.25,  4.12,  4.02, 3.94}; // (kg / kmol)
};


std::ostream& operator<<(std::ostream& stream, const Atmosphere& atmos) {
	stream << "Altitude : " << atmos.altitude() << " m" << std::endl;
	stream << " - Temperature       : " << atmos.temperature()   << " K" << std::endl;
	stream << " - Pressure          : " << atmos.pressure()      << " Pa" << std::endl;
	stream << " - Density           : " << atmos.density()       << " kg/m3" << std::endl;
	stream << " - Speed of Sound    : " << atmos.speed_of_sound()<< " m/s" << std::endl;
	stream << " - Gravity           : " << atmos.gravity()       << " m/s2" << std::endl;
	stream << " - Dynamic Viscosity    : " << atmos.dynamic_viscosity()<< " Pa s" << std::endl;
	stream << " - Kinematic Viscosity  : " << atmos.kinematic_viscosity()<< " m2 / s" << std::endl;
	stream << " - Thermal Conductivity : " << atmos.coeffiecient_of_thermal_conductivity()<< " W/(m K)" << std::endl;
	return stream;
}





}

#endif
