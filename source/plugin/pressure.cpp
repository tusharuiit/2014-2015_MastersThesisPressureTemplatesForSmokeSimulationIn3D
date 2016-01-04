/*****************************************************************************
 *
 * MantaFlow fluid solver framework
 * Copyright 2011 Tobias Pfaff, Nils Thuerey 
 *
 * This program is free software, distributed under the terms of the
 * GNU General Public License (GPL) 
 * http://www.gnu.org/licenses
 *
 * Plugins for pressure correction: solve_pressure, and ghost fluid helpers
 *
 ******************************************************************************/
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include <fstream>
#include <iostream>

#include "vectorbase.h"
#include "kernel.h"
#include "conjugategrad.h"

#include "shapes.h"

#include "grid.h"
#include "commonkernels.h"

// This is to be defined if using stb_perlin.h
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

// This is to be defined if using stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// This is to be defined if using stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "helper.h"
#include "visual.h"

using namespace std;
namespace Manta {

PYTHON void create_image_template_varying(Grid<Real>& pressure_modification, int time, Real multiplier=1.0, int res_x = 64, int res_y = 64)
{
//	time = ((time/20));
//	time = time * 20 ;
	time = (time / 10) * 10;
	char Result[16]; // string which will contain the number
	sprintf ( Result, "%d", time ); // %d makes the result be a decimal integer 
	string str_time(Result);
	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_13/templates/rotate/1/128/cropped" + str_time + ".png" ;
	
	const char *file_name = str.c_str();
	int x, y, comp;
//	char *file_name = "/home/tushar/manta_mikey_1/manta/scenes/mine/january_2_3/templates/grayscale/portrait_1_512_1.png" ;
	unsigned char *data = stbi_load(file_name, &x, &y, &comp, 1);
	unsigned char *data_temp = data ;

	if (data == NULL)
	{
		// error
		cout << "Error, data was null" << endl ;
        }
	else
	{
		// process
//		cout << " data = " << data << endl ;
	}

	std::cout << " x = " << x << " y = " << y << " comp = " << comp << std::endl ;

	Real sum = 0.0;
	int k = 1 ;
	int idx ;
	for( int i = 0 ; i < res_x ; i++ )
	for( int j = 0 ; j < res_y ; j++ )
	{
//		std::cout << Real(*data) << std::endl ;
		idx = pressure_modification.index(i, j, k);
		pressure_modification[idx] = Real(*data_temp);
		data_temp++;
		sum += pressure_modification[idx];
	}


////   normalize the Kernel
//	Normalization not required
//	for(int i = 0; i < res_x; ++i)
//	{
//		for(int j = 0; j < res_y; ++j)
//		{        
//			idx = pressure_modification.index(i, j, k);
//			pressure_modification[idx] /= sum;
//		}
//	}

	Real max_value = 0.0 ;
//	Make the maximum value 1 and accordingly scale other values
	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			if(pressure_modification[idx] > max_value)
			{
				max_value = pressure_modification[idx] ;
			}
		}
	}

	std::cout << " max value = " << max_value << std::endl ;

	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			pressure_modification[idx] /= (max_value);
			pressure_modification[idx] *= (multiplier);
		}
	}

////	Make the maximum value 1 and accordingly adjust other values
//	for(int i = -(radius_x); i <= (radius_x); ++i)
//	{
//		for(int j = -(radius_y); j <= (radius_y); ++j)
//		{        
//			temp_idx_x = idx_x + i ;
//			temp_idx_y = idx_y + j ;
//			idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
//			pressure_modification[idx] /= (central_value);
//			pressure_modification[idx] *= (multiplier);
//			Real j_temp = j + radius_y ;
//			Real m = (higher_factor - lower_factor)/(2.0 * radius_y) ;
//			Real c = lower_factor ;
//			pressure_modification[idx] = (m * (j_temp) + c) * pressure_modification[idx] ;
//		}
//	}

	stbi_image_free(data);
}


PYTHON void create_image_template_portrait(Grid<Real>& pressure_modification, Real multiplier=1.0, int res_x = 64, int res_y = 64)
{
	int x, y, comp;
	char *file_name = "/home/tushar/manta_mikey_1/manta/scenes/mine/january_2_3/templates/grayscale/portrait_1_128.png" ;
	unsigned char *data = stbi_load(file_name, &x, &y, &comp, 1);
	unsigned char *data_temp = data ;

	if (data == NULL)
	{
		// error
		cout << "Error, data was null" << endl ;
        }
	else
	{
		// process
//		cout << " data = " << data << endl ;
	}

	std::cout << " x = " << x << " y = " << y << " comp = " << comp << std::endl ;

	Real sum = 0.0;
	int k = 1 ;
	int idx ;
	for( int i = 0 ; i < res_x ; i++ )
	for( int j = 0 ; j < res_y ; j++ )
	{
//		std::cout << Real(*data) << std::endl ;
		idx = pressure_modification.index(i, j, k);
		pressure_modification[idx] = Real(*data_temp);
		data_temp++;
		sum += pressure_modification[idx];
	}


////   normalize the Kernel
//	Normalization not required
//	for(int i = 0; i < res_x; ++i)
//	{
//		for(int j = 0; j < res_y; ++j)
//		{        
//			idx = pressure_modification.index(i, j, k);
//			pressure_modification[idx] /= sum;
//		}
//	}

	Real max_value = 0.0 ;
//	Make the maximum value 1 and accordingly scale other values
	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			if(pressure_modification[idx] > max_value)
			{
				max_value = pressure_modification[idx] ;
			}
		}
	}

	std::cout << " max value = " << max_value << std::endl ;

	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			pressure_modification[idx] /= (max_value);
			pressure_modification[idx] *= (multiplier);
		}
	}

////	Make the maximum value 1 and accordingly adjust other values
//	for(int i = -(radius_x); i <= (radius_x); ++i)
//	{
//		for(int j = -(radius_y); j <= (radius_y); ++j)
//		{        
//			temp_idx_x = idx_x + i ;
//			temp_idx_y = idx_y + j ;
//			idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
//			pressure_modification[idx] /= (central_value);
//			pressure_modification[idx] *= (multiplier);
//			Real j_temp = j + radius_y ;
//			Real m = (higher_factor - lower_factor)/(2.0 * radius_y) ;
//			Real c = lower_factor ;
//			pressure_modification[idx] = (m * (j_temp) + c) * pressure_modification[idx] ;
//		}
//	}

	stbi_image_free(data);
}


PYTHON void create_image_template_heart(Grid<Real>& pressure_modification, Real multiplier=1.0, int res_x = 64, int res_y = 64)
{
	int x, y, comp;
	char *file_name = "/home/tushar/manta_mikey_1/manta/scenes/mine/january_2_3/templates/grayscale/two_hollow_hearts_1_0_scaled.png" ;
	unsigned char *data = stbi_load(file_name, &x, &y, &comp, 1);
	unsigned char *data_temp = data ;

	if (data == NULL)
	{
		// error
		cout << "Error, data was null" << endl ;
        }
	else
	{
		// process
//		cout << " data = " << data << endl ;
	}

	std::cout << " x = " << x << " y = " << y << " comp = " << comp << std::endl ;

	Real sum = 0.0;
	int k = 1 ;
	int idx ;
	for( int i = 0 ; i < res_x ; i++ )
	for( int j = 0 ; j < res_y ; j++ )
	{
//		std::cout << Real(*data) << std::endl ;
		idx = pressure_modification.index(i, j, k);
		pressure_modification[idx] = Real(*data_temp);
		data_temp++;
		sum += pressure_modification[idx];
	}


////   normalize the Kernel
//	Normalization not required
//	for(int i = 0; i < res_x; ++i)
//	{
//		for(int j = 0; j < res_y; ++j)
//		{        
//			idx = pressure_modification.index(i, j, k);
//			pressure_modification[idx] /= sum;
//		}
//	}

	Real max_value = 0.0 ;
//	Make the maximum value 1 and accordingly scale other values
	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			if(pressure_modification[idx] > max_value)
			{
				max_value = pressure_modification[idx] ;
			}
		}
	}

	std::cout << " max value = " << max_value << std::endl ;

	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			pressure_modification[idx] /= (max_value);
			pressure_modification[idx] *= (multiplier);
		}
	}

////	Make the maximum value 1 and accordingly adjust other values
//	for(int i = -(radius_x); i <= (radius_x); ++i)
//	{
//		for(int j = -(radius_y); j <= (radius_y); ++j)
//		{        
//			temp_idx_x = idx_x + i ;
//			temp_idx_y = idx_y + j ;
//			idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
//			pressure_modification[idx] /= (central_value);
//			pressure_modification[idx] *= (multiplier);
//			Real j_temp = j + radius_y ;
//			Real m = (higher_factor - lower_factor)/(2.0 * radius_y) ;
//			Real c = lower_factor ;
//			pressure_modification[idx] = (m * (j_temp) + c) * pressure_modification[idx] ;
//		}
//	}

	stbi_image_free(data);
}

//! Kernel: Construct the right-hand side of the poisson equation
KERNEL(bnd=1)
void scale_values_linear(Grid<Real>& pressure_modification,Real multiplier,int time,Real max_absolute,Real max_value,Real min_value)
{
            int idx = pressure_modification.index(i , j , k) ;
            pressure_modification[idx] /= max_value ;
            pressure_modification[idx] *= multiplier ;
}

//! Kernel: Construct the right-hand side of the poisson equation
//KERNEL(bnd=1, reduce=+) returns(int cnt=0) returns(double sum=0)
KERNEL(bnd=1)
void step_1_linear (Grid<Real>& pressure_modification,int res_x,int res_y,int res_z,int levels,int time,Real start_x,Real start_y,Real start_z,int time_steps)
{
//	We move from x:y = start_x:start_y to x:y start_y:start_y
	Real frequency_changer_x = start_x;
	Real frequency_changer_y = start_y;
	Real end_z = 20 ;
	Real frequency_changer_z = start_z ;

//	Real factor_increment = ((start_y - start_x)/time_steps)*2.0;

	int time_depender = ((time - 1) % time_steps) ;
	if(time_depender < (time_steps/2))
	{
		frequency_changer_x = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
		frequency_changer_y = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
		frequency_changer_z = start_z + ((end_z - start_z)/(time_steps/2.0)) * time_depender ;
	}
	else
	{
		time_depender = time_depender - time_steps/2 ;
		frequency_changer_x = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
		frequency_changer_y = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
		frequency_changer_z = end_z + ((start_z - end_z)/(time_steps/2.0)) * time_depender ;
	}

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/february_26/data_files/foo.txt", std::ios_base::app);

//	if(k == 32)
//	{
//		outfile << " i = " << i << " j = " << j << " k = " << k << " time = " << time << " time_depender = " << time_depender << " frequency_changer_x = " << frequency_changer_x << " frequency_changer_y = " << frequency_changer_y << " frequency_changer_z = " << frequency_changer_z << endl ;
//		outfile << endl ;
//	}
    
	int xWrap,yWrap,zWrap;
    int idx ;
    int width = res_x , height = res_y , depth = res_z ;
    double noise = 0.0 ;

            double hx = (double)i/width ;
	hx*=frequency_changer_x ;
            double hy = (double)j/height ;
	hy*=frequency_changer_y ;
            double hz = (double)k/depth ;
	hz*=frequency_changer_z ;
            double noise_sum = 0.0 ;
	    int level = 1 ;
//            for(int level = 1 ; level <= levels ; level++ )
//            {
                noise = stb_perlin_noise3(hx*pow(2,level-1),hy*pow(2,level-1),hz*pow(2,level-1),xWrap=0,yWrap=0,zWrap=0);
                noise = (noise + 1.0) / 2.0; // set range to 0 - 1
                if(noise < 0.0){cout<<"ALERT ALERT"<<endl;}
//                noise_sum += noise;
                noise_sum = noise;
//            }
//            noise_sum /= levels;

            idx = pressure_modification.index(i , j , k);
            pressure_modification[idx] = noise_sum;
//    sum += 1;
//    cnt++;
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_3_3D_kernel_linear(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int res_z=128, int levels = 1,int time = 1, Real start_x=1, Real start_y=1, Real start_z=1, int time_steps=30)
{
	std::cout << "levels = " << levels << std::endl ;

//Get the noise value for that particular index	and assign to the grid

//	step_1 Put_noise_value_2_cell(pressure_modification,multiplier,res_x,res_y,res_z,levels,time);
	step_1_linear(pressure_modification,res_x,res_y,res_z,levels,time,start_x,start_y,start_z,time_steps);

//Get the maximum values
	Real max_absolute = pressure_modification.getMaxAbsValue();
	Real max_value = pressure_modification.getMaxValue();
	Real min_value = pressure_modification.getMinValue();

//Scale the values
	scale_values_linear(pressure_modification,multiplier,time,max_absolute,max_value,min_value);

//    int xWrap=0,yWrap=0,zWrap=0 ;
//	int width = res_x , height = res_y ;
//    double noise = 0.0 ;
//    unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

//   for(int x = 1; x<width-1; x++)
//    {
//        for(int y = 1; y<height-1; y++)
//        {
//            double hx = (double)x/width ;
//            double hy = (double)y/height ;
//            double noise_sum = 0.0 ;
//	    int level = 1 ;
////            for(int level = 1 ; level <= levels ; level++ )
////            {
//                noise = stb_perlin_noise3(hx*pow(2,level-1),hy*pow(2,level-1),1.75,xWrap=0,yWrap=0,zWrap=0);
//                noise = (noise + 1.0) / 2.0; // set range to 0 - 1
//                if(noise < 0.0){cout<<"ALERT ALERT"<<endl;}
////                noise_sum += noise;
////            }
////            noise_sum /= levels;
//            noise_sum = noise;

//            int linear_index = y * width + x ;
//            unsigned char col = int(noise_sum * 255);
//            noise_data[linear_index] = col ;
//        }
//    }

//	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
//	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;

//	char Result[8]; // string which will contain the number
//	sprintf ( Result, "%d", int(levels) ); // %d makes the result be a decimal integer
//	string str_frequency_changer(Result);
//	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/february_26/2/perlin" + str_frequency_changer + ".png" ;
//	const char *file_name_output = str.c_str();
//	std::cout << " file_name_output = " << file_name_output << std::endl ;

////	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

//	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
//	free (noise_data);
//	if(a == 1)
//	{
//		std::cout<<"SUCCESS"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"FAILURE"<<std::endl;
//	}
}

//! Kernel: Construct the right-hand side of the poisson equation
KERNEL(bnd=1)
void scale_values(Grid<Real>& pressure_modification,Real multiplier,int time,Real max_absolute,Real max_value,Real min_value)
{
            int idx = pressure_modification.index(i , j , k) ;
            pressure_modification[idx] /= max_value ;
            pressure_modification[idx] *= multiplier ;
}

//! Kernel: Construct the right-hand side of the poisson equation
//KERNEL(bnd=1, reduce=+) returns(int cnt=0) returns(double sum=0)
KERNEL(bnd=1)
void step_1 (Grid<Real>& pressure_modification,int res_x,int res_y,int res_z,int levels,int time,Real frequency_x,Real frequency_y,Real frequency_z,Real translation_x=0.1, Real translation_y=0.1, Real translation_z=0.1)
{
    int xWrap,yWrap,zWrap;
    int idx ;
    int width = res_x , height = res_y , depth = res_z ;
    double noise = 0.0 ;

            double hx = (double)i/width ;
	hx*=frequency_x ;
            double hy = (double)j/height ;
	hy*=frequency_y ;
            double hz = (double)k/depth ;
	hz*=frequency_z ;
            double noise_sum = 0.0 ;
	    int level = 1 ;
//            for(int level = 1 ; level <= levels ; level++ )
//            {
                noise = stb_perlin_noise3(hx*pow(2,level-1)+time*translation_x,hy*pow(2,level-1)+translation_y*time,hz*pow(2,level-1)+translation_z*time,xWrap=0,yWrap=0,zWrap=0);
                noise = (noise + 1.0) / 2.0; // set range to 0 - 1
                if(noise < 0.0){cout<<"ALERT ALERT"<<endl;}
//                noise_sum += noise;
                noise_sum = noise;
//            }
//            noise_sum /= levels;

            idx = pressure_modification.index(i , j , k);
            pressure_modification[idx] = noise_sum;
//    sum += 1;
//    cnt++;
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_3_3D_kernel (Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int res_z=128, int levels = 1,int time = 1, Real frequency_x=1, Real frequency_y=1, Real frequency_z=1, Real translation_x=0.1, Real translation_y=0.1, Real translation_z=0.1)
{
	std::cout << "levels = " << levels << std::endl ;

//	Get the noise value for that particular index	and assign to the grid

//	step_1 Put_noise_value_2_cell(pressure_modification,multiplier,res_x,res_y,res_z,levels,time);
	step_1(pressure_modification,res_x,res_y,res_z,levels,time,frequency_x,frequency_y,frequency_z,translation_x, translation_y, translation_z);

//	Get the maximum values
	Real max_absolute = pressure_modification.getMaxAbsValue();
	Real max_value = pressure_modification.getMaxValue();
	Real min_value = pressure_modification.getMinValue();

//	Scale the values
	scale_values(pressure_modification,multiplier,time,max_absolute,max_value,min_value);

//	int xWrap=0,yWrap=0,zWrap=0 ;
//	int width = res_x , height = res_y ;
//	double noise = 0.0 ;
//	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

//   for(int x = 1; x<width-1; x++)
//    {
//        for(int y = 1; y<height-1; y++)
//        {
//            double hx = (double)x/width ;
//            double hy = (double)y/height ;
//            double noise_sum = 0.0 ;
//	    int level = 1 ;
////            for(int level = 1 ; level <= levels ; level++ )
////            {
//                noise = stb_perlin_noise3(hx*pow(2,level-1),hy*pow(2,level-1),1.75,xWrap=0,yWrap=0,zWrap=0);
//                noise = (noise + 1.0) / 2.0; // set range to 0 - 1
//                if(noise < 0.0){cout<<"ALERT ALERT"<<endl;}
////                noise_sum += noise;
////            }
////            noise_sum /= levels;
//            noise_sum = noise;

//            int linear_index = y * width + x ;
//            unsigned char col = int(noise_sum * 255);
//            noise_data[linear_index] = col ;
//        }
//    }

////	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
////	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;

//	char Result[8]; // string which will contain the number
//	sprintf ( Result, "%d", int(levels) ); // %d makes the result be a decimal integer
//	string str_frequency_changer(Result);
//	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/february_23/templates/1/perlin" + str_frequency_changer + ".png" ;
//	const char *file_name_output = str.c_str();
//	std::cout << " file_name_output = " << file_name_output << std::endl ;

////	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

//	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
//	free (noise_data);
//	if(a == 1)
//	{
//		std::cout<<"SUCCESS"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"FAILURE"<<std::endl;
//	}
}

PYTHON void read_image_template(Grid<Real>& pressure_modification,Real multiplier=1.0,int res_x=64,int res_y=64,int res_z=64,string smoother_name="")
{
	int x, y, comp;
//	char *file_name = "/home/tushar/manta_mikey_1/manta/build/pictures/january_13/templates/4/128.png" ;
	const char *file_name = smoother_name.c_str();
	unsigned char *data = stbi_load(file_name, &x, &y, &comp, 1);
	unsigned char *data_temp = data ;

	if (data == NULL)
	{
		// error
		cout << "Error, data was null" << endl ;
        }
	else
	{
		// process
//		cout << " data = " << data << endl ;
	}

	std::cout << " x = " << x << " y = " << y << " comp = " << comp << std::endl ;
	std::cout << " res_x = " << res_x << " res_y = " << res_y << " res_z = " << res_z << std::endl ;

	Real sum = 0.0;
	int k = 1 ;
	int idx ;
	for( int i = 0 ; i < res_x ; i++ )
	for( int j = 0 ; j < res_y ; j++ )
	{
//		std::cout << Real(*data) << std::endl ;
		idx = pressure_modification.index(i, j, k);
		pressure_modification[idx] = Real(*data_temp);
		data_temp++;
		sum += pressure_modification[idx];
	}

	Real max_value = pressure_modification.getMaxValue() ;

	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			pressure_modification[idx] /= (max_value);
			pressure_modification[idx] *= (multiplier);
		}
	}
	stbi_image_free(data);
}

//! Kernel: Construct the right-hand side of the poisson equation
//KERNEL(bnd=1, reduce=+) returns(int cnt=0) returns(double sum=0)
KERNEL
void smooth_boundary (Grid<Real>& boundary_zero_smoother,int res_x,int res_y,int res_z, Real multiplier)
{
//	Real V1,V2,V3,V4,V5,V6,V7,V8,V;
//	Real A1,A2,A3,A4,A;
//	Real length_1,length_2,length_total;
//	int idx ;
//	idx = boundary_zero_smoother.index(i , j , k);
////	1
////	x=low; y=low; z=low; 
//	if((i<6)&&(j<6)&&(k<6))
//	{
//		Real x1=(i+0.5),y1=(j+0.5),z1=(k+0.5);
//		Real x2=(5.5-i),y2=(5.5-j),z2=(5.5-k);
//		V=(5+1)*(5+1)*(5+1) ;
//		V1=(x1)*(y1)*(z1) ;
//		V2=(x2)*(y1)*(z1) ;
//		V3=(x1)*(y2)*(z1) ;
//		V4=(x2)*(y2)*(z1) ;
//		V5=(x1)*(y1)*(z2) ;
//		V6=(x2)*(y1)*(z2) ;
//		V7=(x1)*(y2)*(z2) ;
//		V8=(x2)*(y2)*(z2) ;
//		boundary_zero_smoother[idx]=(V1*1+V2*0+V3*0+V4*0+V5*0+V6*0+V7*0+V8*0)/V;
////		boundary_zero_smoother[idx]=0;
//	}
////	2
////	x=high; y=low; z=low;
////	else if((i>(res_x-7))&&(j<6)&&(k<6))
////	{
////		Real x1=(i-(res_x-6.5)),y1=(j+0.5),z1=(k+0.5) ;
////		Real x2=(res_x-(i+0.5)),y2=(5.5-j),z2=(5.5-k) ;
////		V=(5+1)*(5+1)*(5+1);
////		V1=(x1)*(y1)*(z1) ;
////		V2=(x2)*(y1)*(z1) ;
////		V3=(x1)*(y2)*(z1) ;
////		V4=(x2)*(y2)*(z1) ;
////		V5=(x1)*(y1)*(z2) ;
////		V6=(x2)*(y1)*(z2) ;
////		V7=(x1)*(y2)*(z2) ;
////		V8=(x2)*(y2)*(z2) ;
////		boundary_zero_smoother[idx]=(V1*0+V2*1+V3*0+V4*0+V5*0+V6*0+V7*0+V8*0)/V ;
////	}
//////	3
//////	x=low; y=high; z=low;
////	else if(((i<6))&&(j>(res_y-7))&&(k<6))
////	{
////		Real x1=(i+0.5),y1=(j-(res_y-6.5)),z1=(k+0.5) ;
////		Real x2=(5.5-i),y2=(res_y-(j+0.5)),z2=(5.5-k) ;
////		V=(5+1)*(5+1)*(5+1);
////		V1=(x1)*(y1)*(z1) ;
////		V2=(x2)*(y1)*(z1) ;
////		V3=(x1)*(y2)*(z1) ;
////		V4=(x2)*(y2)*(z1) ;
////		V5=(x1)*(y1)*(z2) ;
////		V6=(x2)*(y1)*(z2) ;
////		V7=(x1)*(y2)*(z2) ;
////		V8=(x2)*(y2)*(z2) ;
////		boundary_zero_smoother[idx]=(V1*0+V2*0+V3*1+V4*0+V5*0+V6*0+V7*0+V8*0)/V ;
////	}
//////	4
//////	x=high; y=high; z=low;
////	else if(((i>(res_x-7)))&&(j>(res_y-7))&&(k<6))
////	{
////		Real x1=(i-(res_x-6.5)),y1=(j-(res_y-6.5)),z1=(k+0.5) ;
////		Real x2=(res_x-(i+0.5)),y2=(res_y-(j+0.5)),z2=(5.5-k) ;
////		V=(5+1)*(5+1)*(5+1);
////		V1=(x1)*(y1)*(z1) ;
////		V2=(x2)*(y1)*(z1) ;
////		V3=(x1)*(y2)*(z1) ;
////		V4=(x2)*(y2)*(z1) ;
////		V5=(x1)*(y1)*(z2) ;
////		V6=(x2)*(y1)*(z2) ;
////		V7=(x1)*(y2)*(z2) ;
////		V8=(x2)*(y2)*(z2) ;
////		boundary_zero_smoother[idx]=(V1*0+V2*0+V3*0+V4*1+V5*0+V6*0+V7*0+V8*0)/V ;
////	}
//////	5
//////	x=low; y=low; z=high;
////	else if(((i<6))&&(j<6)&&(k>(res_z-7)))
////	{
////		Real x1=(i+0.5),y1=(j+0.5),z1=(k-(res_z-6.5)) ;
////		Real x2=(5.5-i),y2=(5.5-j),z2=(res_z-(k+0.5)) ;
////		V=(5+1)*(5+1)*(5+1);
////		V1=(x1)*(y1)*(z1) ;
////		V2=(x2)*(y1)*(z1) ;
////		V3=(x1)*(y2)*(z1) ;
////		V4=(x2)*(y2)*(z1) ;
////		V5=(x1)*(y1)*(z2) ;
////		V6=(x2)*(y1)*(z2) ;
////		V7=(x1)*(y2)*(z2) ;
////		V8=(x2)*(y2)*(z2) ;
////		boundary_zero_smoother[idx]=(V1*0+V2*0+V3*0+V4*0+V5*1+V6*0+V7*0+V8*0)/V ;
////	}
//////	6
//////	x=high; y=low; z=high;
////	else if((i>(res_x-7))&&(j<6)&&(k>(res_z-7)))
////	{
////		Real x1=(i-(res_x-6.5)),y1=(j+0.5),z1=(k-(res_z-6.5)) ;
////		Real x2=(res_x-(i+0.5)),y2=(5.5-j),z2=(res_z-(k+0.5)) ;
////		V=(5+1)*(5+1)*(5+1);
////		V1=(x1)*(y1)*(z1) ;
////		V2=(x2)*(y1)*(z1) ;
////		V3=(x1)*(y2)*(z1) ;
////		V4=(x2)*(y2)*(z1) ;
////		V5=(x1)*(y1)*(z2) ;
////		V6=(x2)*(y1)*(z2) ;
////		V7=(x1)*(y2)*(z2) ;
////		V8=(x2)*(y2)*(z2) ;
////		boundary_zero_smoother[idx]=(V1*0+V2*0+V3*0+V4*0+V5*0+V6*1+V7*0+V8*0)/V ;
////	}
//////	7
//////	x=low; y=high; z=high;
////	else if( (i<6) && (j>(res_y-7)) && (k>(res_z-7)) )
////	{
////		Real x1=(i+0.5),y1=(j-(res_y-6.5)),z1=(k-(res_z-6.5)) ;
////		Real x2=(5.5-i),y2=(res_y-(j+0.5)),z2=(res_z-(k+0.5)) ;
////		V=(5+1)*(5+1)*(5+1);
////		V1=(x1)*(y1)*(z1) ;
////		V2=(x2)*(y1)*(z1) ;
////		V3=(x1)*(y2)*(z1) ;
////		V4=(x2)*(y2)*(z1) ;
////		V5=(x1)*(y1)*(z2) ;
////		V6=(x2)*(y1)*(z2) ;
////		V7=(x1)*(y2)*(z2) ;
////		V8=(x2)*(y2)*(z2) ;
////		boundary_zero_smoother[idx]=(V1*0+V2*0+V3*0+V4*0+V5*0+V6*0+V7*1+V8*0)/V ;
////	}
//////	8
//////	x=high; y=high; z=high;
////	else if((i>(res_x-7))&&(j>(res_y-7))&&(k>(res_z-7)))
////	{
////		Real x1=(i-(res_x-6.5)),y1=(j-(res_y-6.5)),z1=(k-(res_z-6.5));
////		Real x2=(res_x-(i+0.5)),y2=(res_y-(j+0.5)),z2=(res_z-(k+0.5));
////		V=(5+1)*(5+1)*(5+1);
////		V1=(x1)*(y1)*(z1);
////		V2=(x2)*(y1)*(z1);
////		V3=(x1)*(y2)*(z1);
////		V4=(x2)*(y2)*(z1);
////		V5=(x1)*(y1)*(z2);
////		V6=(x2)*(y1)*(z2);
////		V7=(x1)*(y2)*(z2);
////		V8=(x2)*(y2)*(z2);
////		boundary_zero_smoother[idx]=(V1*0+V2*0+V3*0+V4*0+V5*0+V6*0+V7*0+V8*1)/V;
////	}
//////	The  8 edges have to be tackled now for bi-linear interpolation
//////	x1-xn y1-y2 z1-z2
////	else if( ((i>5)&&(i<res_x-6)) && (j<6) && (k<6) )
////	{
////		Real y1=(j+0.5),z1=(k+0.5);
////		Real y2=(5.5-j),z2=(5.5-k);
////		A=(5+1)*(5+1) ;
////		A1=(y1)*(z1) ;
////		A2=(y2)*(z1) ;
////		A3=(y2)*(z2) ;
////		A4=(y1)*(z2) ;
////		boundary_zero_smoother[idx]=(A1*1+A2*0+A3*0+A4*0)/A;
////	}
//////	x1-xn yn-1-yn z1-z2
////	else if( ((i>5)&&(i<res_x-6)) && (j>(res_y-7)) && (k<6) )
////	{
////		Real y1=(j-(res_y-6.5)),z1=(k+0.5);
////		Real y2=(res_y-(j+0.5)),z2=(5.5-k);
////		A=(5+1)*(5+1) ;
////		A1=(y2)*(z1) ;
////		A2=(y2)*(z2) ;
////		A3=(y1)*(z1) ;
////		A4=(y1)*(z1) ;
////		boundary_zero_smoother[idx]=(A1*1+A2*0+A3*0+A4*0)/A;
////	}
//////	x1-x2 y1-yn z1-z2
////	else if( (i<6) && ((j>5)&&(j<res_y-6)) && (k<6))
////	{
////		Real x1=(i+0.5),z1=(k+0.5);
////		Real x2=(5.5-i),z2=(5.5-k);
////		A=(5+1)*(5+1) ;
////		A1=(x1)*(z1) ;
////		A2=(x2)*(z1) ;
////		A3=(x2)*(z2) ;
////		A4=(x1)*(z2) ;
////		boundary_zero_smoother[idx]=(A1*1+A2*0+A3*0+A4*0)/A;
////	}
//////	xn-1-xn y1-yn z1-z2
////	else if( (i>(res_x-7)) && ((j>5)&&(j<res_y-6)) && (k<6) )
////	{
////		Real x1=(i-(res_x-6.5)),z1=(k+0.5);
////		Real x2=(res_x-(i+0.5)),z2=(5.5-k);
////		A=(5+1)*(5+1) ;
////		A1=(x2)*(z1) ;
////		A2=(x2)*(z2) ;
////		A3=(x1)*(z2) ;
////		A4=(x1)*(z1) ;
////		boundary_zero_smoother[idx]=(A1*1+A2*0+A3*0+A4*0)/A;
////	}
//////	x1-xn y1-y2 zn-1-zn
////	else if( ((i>5)&&(i<res_x-6)) && (j<6) && (k>(res_z-7)) )
////	{
////		Real y1=(j+0.5),z1=(k-(res_z-6.5));
////		Real y2=(5.5-j),z2=(res_z-(k+0.5));
////		A=(5+1)*(5+1) ;
////		A1=(y1)*(z2) ;
////		A2=(y1)*(z1) ;
////		A3=(y2)*(z1) ;
////		A4=(y2)*(z2) ;
////		boundary_zero_smoother[idx]=(A1*1+A2*0+A3*0+A4*0)/A;
////	}
//////	x1-xn yn-1-yn zn-1-zn
////	else if( ((i>5)&&(i<res_x-6)) && (j>res_y-7) && (k>res_z-7) )
////	{
////		Real y1=(j-(res_y-6.5)),z1=(k-(res_z-6.5));
////		Real y2=(res_y-(j+0.5)),z2=(res_z-(k+0.5));
////		A=(5+1)*(5+1) ;
////		A1=(y2)*(z2) ;
////		A2=(y1)*(z2) ;
////		A3=(y1)*(z1) ;
////		A4=(y2)*(z1) ;
////		boundary_zero_smoother[idx]=(A1*1+A2*0+A3*0+A4*0)/A;
////	}
//////	x1-x2 y1-yn zn-1-zn
////	else if( (i<6) && ((j>5)&&(j<res_y-6)) && (k>res_z-7) )
////	{
////		Real x1=(i+0.5),z1=(k-(res_z-6.5));
////		Real x2=(5.5-i),z2=(res_z-(k+0.5));
////		A=(5+1)*(5+1) ;
////		A1=(x1)*(z2) ;
////		A2=(x1)*(z1) ;
////		A3=(x2)*(z1) ;
////		A4=(x2)*(z2) ;
////		boundary_zero_smoother[idx]=(A1*1+A2*0+A3*0+A4*0)/A;
////	}
//////	xn-1-xn y1-yn zn-1-zn
////	else if( (i>(res_x-7)) && ((j>5)&&(j<res_y-6)) && (k>res_z-7) )
////	{
////		Real x1=(i-(res_x-6.5)),z1=(k-(res_z-6.5));
////		Real x2=(res_x-(i+0.5)),z2=(res_z-(k+0.5));
////		A=(5+1)*(5+1) ;
////		A1=(x2)*(z2) ;
////		A2=(x1)*(z2) ;
////		A3=(x1)*(z1) ;
////		A4=(x2)*(z1) ;
////		boundary_zero_smoother[idx]=(A1*1+A2*0+A3*0+A4*0)/A;
////	}
//////	Do the linear interpolation for the faces
//////	z low
////	else if( ((i>5)&&(i<res_x-6)) && ((j>6)&&(j<res_y-6)) && (k<6) )
////	{
////		Real z1=(k+0.5);
////		Real z2=(5.5-k);
////		length_total=(5+1) ;
////		length_1=(z1) ;
////		length_2=(z2) ;
////		boundary_zero_smoother[idx]=(length_1*1+length_2*0)/length_total;
////	}
//////	z high
////	else if( ((i>5)&&(i<res_x-6)) && ((j>6)&&(j<res_y-6)) && (k>res_z-7) )
////	{
////		Real z1=(k-(res_z-6.5));
////		Real z2=(res_z-(k+0.5));
////		length_total=(5+1) ;
////		length_1=(z1) ;
////		length_2=(z2) ;
////		boundary_zero_smoother[idx]=(length_1*1+length_2*0)/length_total;
////	}
//////	x low
////	else if( (i<6) && ((j>6)&&(j<res_y-6)) && ((k>5)&&(k<res_z-6)) )
////	{
////		Real x1=(i+0.5);
////		Real x2=(5.5-i);
////		length_total=(5+1) ;
////		length_1=(x1) ;
////		length_2=(x2) ;
////		boundary_zero_smoother[idx]=(length_1*1+length_2*0)/length_total;
////	}
//////	x high
////	else if( (i>res_x-7) && ((j>6)&&(j<res_y-6)) && ((k>5)&&(k<res_z-6)) )
////	{
////		Real x1=(i-(res_x-6.5));
////		Real x2=(res_x-(i+0.5));
////		length_total=(5+1) ;
////		length_1=(x1) ;
////		length_2=(x2) ;
////		boundary_zero_smoother[idx]=(length_1*1+length_2*0)/length_total;
////	}
//////	y low
////	else if( ((i>5)&&(i<res_x-6)) && (j<6) && ((k>5)&&(k<res_z-6)) )
////	{
////		Real y1=(j+0.5);
////		Real y2=(5.5-j);
////		length_total=(5+1) ;
////		length_1=(y1) ;
////		length_2=(y2) ;
////		boundary_zero_smoother[idx]=(length_1*1+length_2*0)/length_total;
////	}
//////	y high
////	else if( ((i>5)&&(i<res_x-6)) && (j>res_y-7) && ((k>5)&&(k<res_z-6)) )
////	{
////		Real y1=(j-(res_y-6.5));
////		Real y2=(res_y-(j+0.5));
////		length_total=(5+1) ;
////		length_1=(y1) ;
////		length_2=(y2) ;
////		boundary_zero_smoother[idx]=(length_1*1+length_2*0)/length_total;
////	}
//	else
//	{
//		boundary_zero_smoother[idx]=1;
//	}	
//	boundary_zero_smoother[idx] = boundary_zero_smoother[idx] * multiplier ;

////	boundary_zero_smoother[idx] = noise_sum;
}

//! Kernel: Apply velocity update from poisson equation
KERNEL(bnd=1)
void apply_template_vorticity_density_localized_kernel(FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure, Grid<Real>& density, Grid<Real>& norm, Real density_threshold=0.01, int vorticity_density_based_gradient_scaling = 1, Real curl_strength_modifier = 1.0) 
{
//	std::cout<<"Entered CorrectVelocity"<<std::endl;
	int idx = flags.index(i,j,k);
	if(density[idx] > density_threshold)
	{
		Real scaling_factor = 1 ;
		if( vorticity_density_based_gradient_scaling == 1 )
		{
			scaling_factor = (norm[idx])*density[idx]* curl_strength_modifier ;
			std::cout << " norm = " << norm[idx] << "  density = " << density[idx] << "  strength = " << curl_strength_modifier << std::endl ;
		}
		else
		{
			scaling_factor = 1 ;
		}
		if (flags.isFluid(idx))
		{
			if (flags.isFluid(i-1,j,k)) vel[idx].x -= ( (scaling_factor) * (pressure[idx] - pressure(i-1,j,k)));
			if (flags.isFluid(i,j-1,k)) vel[idx].y -= ((scaling_factor) * (pressure[idx] - pressure(i,j-1,k)));
			if (flags.is3D() && flags.isFluid(i,j,k-1)) vel[idx].z -= ( scaling_factor * (pressure[idx] - pressure(i,j,k-1)));

			if (flags.isEmpty(i-1,j,k)) vel[idx].x -= ( (scaling_factor) * pressure[idx]);
			if (flags.isEmpty(i,j-1,k)) vel[idx].y -= ( (scaling_factor) * pressure[idx]);
			if (flags.is3D() && flags.isEmpty(i,j,k-1)) vel[idx].z -= ( (scaling_factor) * pressure[idx]);
		}
		else if (flags.isEmpty(idx))
		{
			if (flags.isFluid(i-1,j,k)) vel[idx].x += ( (scaling_factor) * pressure(i-1,j,k));
			else                        vel[idx].x  = 0.f;
			if (flags.isFluid(i,j-1,k)) vel[idx].y += ( (scaling_factor) * pressure(i,j-1,k));
			else                        vel[idx].y  = 0.f;
			if (flags.is3D() ) {
			if (flags.isFluid(i,j,k-1)) vel[idx].z += ( (scaling_factor) * pressure(i,j,k-1));
			else                        vel[idx].z  = 0.f;
		}
	}
	}
}

//! Kernel: Apply velocity update from poisson equation
KERNEL(bnd=1) 
void scale_positive_grid_to_max_1(FlagGrid& flags, Grid<Real>& target, Real max_value) 
{
//	std::cout<<"Entered CorrectVelocity"<<std::endl;
	int idx = flags.index(i,j,k);
	if (max_value < VECTOR_EPSILON){max_value = 1;}
	target[idx] = (target[idx] / max_value);
}

PYTHON void apply_template_vorticity_density_localized( FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure_template, Grid<Real>& density,Real density_threshold=0.01, int vorticity_density_based_gradient_scaling = 0,Real curl_strength_modifier = 1.0 )
{
	Grid<Vec3> velCenter(flags.getParent()), curl(flags.getParent()) ;
	Grid<Real> norm(flags.getParent());

	GetCentered(velCenter, vel);
	CurlOp(velCenter, curl);
	GridNorm(norm, curl);
	Real max_value = norm.getMaxAbsValue();
	scale_positive_grid_to_max_1(flags,norm,max_value);
	apply_template_vorticity_density_localized_kernel(flags, vel, pressure_template, density, norm, density_threshold, vorticity_density_based_gradient_scaling, curl_strength_modifier );
}

//! Kernel: Apply velocity update from poisson equation
KERNEL(bnd=1)
void apply_template_vorticity_localized_kernel(FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure, Grid<Real>& density, Grid<Real>& norm, Real density_threshold=0.01, int vorticity_based_gradient_scaling = 1, Real curl_strength_modifier = 1.0) 
{
//	std::cout<<"Entered CorrectVelocity"<<std::endl;
	int idx = flags.index(i,j,k);
	if(density[idx] > density_threshold)
	{
		Real scaling_factor = 1 ;
		if( vorticity_based_gradient_scaling == 1 )
		{
			scaling_factor = (norm[idx])* curl_strength_modifier ;
			std::cout << " norm = " << norm[idx] << "  density = " << density[idx] << "  strength = " << curl_strength_modifier << std::endl ;
		}
		else
		{
			scaling_factor = 1 ;
		}
		if (flags.isFluid(idx))
		{
			if (flags.isFluid(i-1,j,k)) vel[idx].x -= ( (scaling_factor) * (pressure[idx] - pressure(i-1,j,k)));
			if (flags.isFluid(i,j-1,k)) vel[idx].y -= ((scaling_factor) * (pressure[idx] - pressure(i,j-1,k)));
			if (flags.is3D() && flags.isFluid(i,j,k-1)) vel[idx].z -= ( scaling_factor * (pressure[idx] - pressure(i,j,k-1)));

			if (flags.isEmpty(i-1,j,k)) vel[idx].x -= ( (scaling_factor) * pressure[idx]);
			if (flags.isEmpty(i,j-1,k)) vel[idx].y -= ( (scaling_factor) * pressure[idx]);
			if (flags.is3D() && flags.isEmpty(i,j,k-1)) vel[idx].z -= ( (scaling_factor) * pressure[idx]);
		}
		else if (flags.isEmpty(idx))
		{
			if (flags.isFluid(i-1,j,k)) vel[idx].x += ( (scaling_factor) * pressure(i-1,j,k));
			else                        vel[idx].x  = 0.f;
			if (flags.isFluid(i,j-1,k)) vel[idx].y += ( (scaling_factor) * pressure(i,j-1,k));
			else                        vel[idx].y  = 0.f;
			if (flags.is3D() ) {
			if (flags.isFluid(i,j,k-1)) vel[idx].z += ( (scaling_factor) * pressure(i,j,k-1));
			else                        vel[idx].z  = 0.f;
		}
	}
	}
}

PYTHON void apply_template_vorticity_localized( FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure_template, Grid<Real>& density,Real density_threshold=0.01, int vorticity_based_gradient_scaling = 0,Real curl_strength_modifier = 1.0 )
{
	Grid<Vec3> velCenter(flags.getParent()), curl(flags.getParent()) ;
	Grid<Real> norm(flags.getParent());

	GetCentered(velCenter, vel);
	CurlOp(velCenter, curl);
	GridNorm(norm, curl);
	Real max_value = norm.getMaxAbsValue();
	scale_positive_grid_to_max_1(flags,norm,max_value);
	apply_template_vorticity_localized_kernel(flags, vel, pressure_template, density, norm, density_threshold, vorticity_based_gradient_scaling, curl_strength_modifier );
}

//! Kernel: Apply velocity update from poisson equation
KERNEL(bnd=1) 
void apply_template_density_localized_kernel(FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure, Grid<Real>& density, Real density_threshold=0.01, int density_based_gradient_scaling=1)
{
//	std::cout<<"Entered CorrectVelocity"<<std::endl;
	int idx = flags.index(i,j,k);

	int idx_left = flags.index(i-1,j,k) ;
	int idx_right = flags.index(i+1,j,k) ;

	int idx_top = flags.index(i,j+1,k) ;
	int idx_bottom = flags.index(i,j-1,k) ;

	int idx_up = flags.index(i,j,k+1) ;
	int idx_down = flags.index(i,j,k-1) ;

	if(density[idx] > density_threshold)
	{
		Vec3 scaling_factor ; scaling_factor.x=0.0 ; scaling_factor.y=0.0 ; scaling_factor.z=0.0 ;
		if( density_based_gradient_scaling == 1 )
		{
			scaling_factor.x = (density[idx]+density[idx_left]+density[idx_right])/3.0 ;
			scaling_factor.y = (density[idx]+density[idx_top]+density[idx_bottom])/3.0 ;
			scaling_factor.z = (density[idx]+density[idx_up]+density[idx_down])/3.0 ;
		}
		else
		{
			scaling_factor.x = 1 ;
			scaling_factor.y = 1 ;
			scaling_factor.z = 1 ;
		}
		if (flags.isFluid(idx))
		{
			if (flags.isFluid(i-1,j,k)) vel[idx].x -= ( (scaling_factor.x) * (pressure[idx] - pressure(i-1,j,k))) ;
			if (flags.isFluid(i,j-1,k)) vel[idx].y -= ((scaling_factor.y) * (pressure[idx] - pressure(i,j-1,k))) ;
			if (flags.is3D() && flags.isFluid(i,j,k-1)) vel[idx].z -= ( scaling_factor.z * (pressure[idx] - pressure(i,j,k-1))) ;

			if (flags.isEmpty(i-1,j,k)) vel[idx].x -= ( (scaling_factor.x) * pressure[idx]) ;
			if (flags.isEmpty(i,j-1,k)) vel[idx].y -= ( (scaling_factor.y) * pressure[idx]) ;
			if (flags.is3D() && flags.isEmpty(i,j,k-1)) vel[idx].z -= ( (scaling_factor.z) * pressure[idx]) ;
		}
		else if (flags.isEmpty(idx))
		{
			if (flags.isFluid(i-1,j,k)) vel[idx].x += ( (scaling_factor.x) * pressure(i-1,j,k)) ;
			else                        vel[idx].x  = 0.f;
			if (flags.isFluid(i,j-1,k)) vel[idx].y += ( (scaling_factor.y) * pressure(i,j-1,k)) ;
			else                        vel[idx].y  = 0.f;
			if (flags.is3D() ) {
			if (flags.isFluid(i,j,k-1)) vel[idx].z += ( (scaling_factor.z) * pressure(i,j,k-1)) ;
			else                        vel[idx].z  = 0.f;
		}
	}
	}
}

PYTHON void apply_template_density_localized( FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure_template, Grid<Real>& density,Real density_threshold=0.01, int density_based_gradient_scaling = 1 )
{
	apply_template_density_localized_kernel(flags, vel, pressure_template, density, density_threshold, density_based_gradient_scaling);
}

//! Kernel: Apply velocity update from poisson equation
KERNEL(bnd=1) 
void apply_template_density_localized_shape_kernel(FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure, Grid<Real>& density, Shape *  shape, Real density_threshold=0.01, int density_based_gradient_scaling=1)
{
//	std::cout<<"Entered CorrectVelocity"<<std::endl;
	int idx = flags.index(i,j,k);

	int idx_left = flags.index(i-1,j,k) ;
	int idx_right = flags.index(i+1,j,k) ;

	int idx_top = flags.index(i,j+1,k) ;
	int idx_bottom = flags.index(i,j-1,k) ;

	int idx_up = flags.index(i,j,k+1) ;
	int idx_down = flags.index(i,j,k-1) ;

	if(density[idx] > density_threshold)
	{
		Vec3 scaling_factor ; scaling_factor.x=0.0 ; scaling_factor.y=0.0 ; scaling_factor.z=0.0 ;
		if( density_based_gradient_scaling == 1 )
		{
			scaling_factor.x = (density[idx]+density[idx_left]+density[idx_right])/3.0 ;
			scaling_factor.y = (density[idx]+density[idx_top]+density[idx_bottom])/3.0 ;
			scaling_factor.z = (density[idx]+density[idx_up]+density[idx_down])/3.0 ;
		}
		else
		{
			scaling_factor.x = 1 ;
			scaling_factor.y = 1 ;
			scaling_factor.z = 1 ;
		}
		if ( (flags.isFluid(idx)) && (!(shape->isInsideGrid(i,j,k))) )
		{
			if (flags.isFluid(i-1,j,k)) vel[idx].x -= ( (scaling_factor.x) * (pressure[idx] - pressure(i-1,j,k))) ;
			if (flags.isFluid(i,j-1,k)) vel[idx].y -= ((scaling_factor.y) * (pressure[idx] - pressure(i,j-1,k))) ;
			if (flags.is3D() && flags.isFluid(i,j,k-1)) vel[idx].z -= ( scaling_factor.z * (pressure[idx] - pressure(i,j,k-1))) ;

			if (flags.isEmpty(i-1,j,k)) vel[idx].x -= ( (scaling_factor.x) * pressure[idx]) ;
			if (flags.isEmpty(i,j-1,k)) vel[idx].y -= ( (scaling_factor.y) * pressure[idx]) ;
			if (flags.is3D() && flags.isEmpty(i,j,k-1)) vel[idx].z -= ( (scaling_factor.z) * pressure[idx]) ;
		}
		else if (flags.isEmpty(idx))
		{
			if (flags.isFluid(i-1,j,k)) vel[idx].x += ( (scaling_factor.x) * pressure(i-1,j,k)) ;
			else                        vel[idx].x  = 0.f;
			if (flags.isFluid(i,j-1,k)) vel[idx].y += ( (scaling_factor.y) * pressure(i,j-1,k)) ;
			else                        vel[idx].y  = 0.f;
			if (flags.is3D() ) {
			if (flags.isFluid(i,j,k-1)) vel[idx].z += ( (scaling_factor.z) * pressure(i,j,k-1)) ;
			else                        vel[idx].z  = 0.f;
		}
	}
	}
}

PYTHON void apply_template_density_localized_shape( FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure_template, Grid<Real>& density, Shape* shape, Real density_threshold=0.01, int density_based_gradient_scaling = 1 )
{
	apply_template_density_localized_shape_kernel(flags, vel, pressure_template, density, shape, density_threshold, density_based_gradient_scaling);
}

PYTHON void create_boundary_smoother_3D_kernel(Grid<Real>& boundary_zero_smoother,Real multiplier=1.0,int res_x=64,int res_y=64,int res_z=64)
{
	smooth_boundary(boundary_zero_smoother, res_x, res_y, res_z, multiplier) ;
}

//! Kernel: Construct the right-hand side of the poisson equation
KERNEL(bnd=1)
void applyRNG_binary_NoiseToGrid_direct_kernel(Grid<Real>& pressure_modification, Real value, Real fraction_white, int res_x, int res_y, int res_z)
{
	int idx = pressure_modification.index(i , j , k) ;

	Real random_number = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0));
	if(random_number<=fraction_white)
//		(*grid)(i,j,k) = value ;
		pressure_modification[idx] = value ;
	else
		pressure_modification[idx] = 0.0 ;
}


PYTHON void applyRNG_binary_NoiseToGrid_direct(Grid<Real>& grid, Real value=1.0, Real fraction_white=0.5, int res_x = 64, int res_y = 64, int res_z = 64)
{
	applyRNG_binary_NoiseToGrid_direct_kernel(grid, value, fraction_white, res_x, res_y, res_z) ;

}

PYTHON void create_source_density_from_image(Grid<Real>& pressure_modification, Real multiplier=1.0, int res_x = 64, int res_y = 64)
{
	int x, y, comp;
	char *file_name = "/home/tushar/manta_mikey_1/manta/build/pictures/february_20/templates/1/psi_128.png" ;
	unsigned char *data = stbi_load(file_name, &x, &y, &comp, 1);
	unsigned char *data_temp = data ;

	if (data == NULL)
	{
		// error
		cout << "Error, data was null" << endl ;
        }
	else
	{
		// process
//		cout << " data = " << data << endl ;
	}

	std::cout << " x = " << x << " y = " << y << " comp = " << comp << std::endl ;

	Real sum = 0.0;
	int k = 1 ;
	int idx ;
	for( int i = 0 ; i < res_x ; i++ )
	for( int j = 0 ; j < res_y ; j++ )
	{
//		std::cout << Real(*data) << std::endl ;
		idx = pressure_modification.index(i, j, k);
		pressure_modification[idx] = Real(*data_temp);
		data_temp++;
		sum += pressure_modification[idx];
	}

	Real max_value = 0.0 ;
//	Make the maximum value 1 and accordingly scale other values
	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			if(pressure_modification[idx] > max_value)
			{
				max_value = pressure_modification[idx] ;
			}
		}
	}

	std::cout << " max value = " << max_value << std::endl ;

	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			pressure_modification[idx] /= (max_value);
			pressure_modification[idx] *= (multiplier);
		}
	}

	stbi_image_free(data);
}


PYTHON void create_target_density_from_image(Grid<Real>& pressure_modification, Real multiplier=1.0, int res_x = 64, int res_y = 64)
{
	int x, y, comp;
	char *file_name = "/home/tushar/manta_mikey_1/manta/build/pictures/february_20/templates/1/phi_128.png" ;
	unsigned char *data = stbi_load(file_name, &x, &y, &comp, 1);
	unsigned char *data_temp = data ;

	if (data == NULL)
	{
		// error
		cout << "Error, data was null" << endl ;
        }
	else
	{
		// process
//		cout << " data = " << data << endl ;
	}

	std::cout << " x = " << x << " y = " << y << " comp = " << comp << std::endl ;

	Real sum = 0.0;
	int k = 1 ;
	int idx ;
	for( int i = 0 ; i < res_x ; i++ )
	for( int j = 0 ; j < res_y ; j++ )
	{
//		std::cout << Real(*data) << std::endl ;
		idx = pressure_modification.index(i, j, k);
		pressure_modification[idx] = Real(*data_temp);
		data_temp++;
		sum += pressure_modification[idx];
	}

	Real max_value = 0.0 ;
//	Make the maximum value 1 and accordingly scale other values
	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			if(pressure_modification[idx] > max_value)
			{
				max_value = pressure_modification[idx] ;
			}
		}
	}

	std::cout << " max value = " << max_value << std::endl ;

	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			pressure_modification[idx] /= (max_value);
			pressure_modification[idx] *= (multiplier);
		}
	}

	stbi_image_free(data);
}

PYTHON void create_source_density_convolve_from_image(Grid<Real>& pressure_modification, Real multiplier=1.0, int res_x = 64, int res_y = 64)
{
	int x, y, comp;
	char *file_name = "/home/tushar/manta_mikey_1/manta/build/pictures/february_20/templates/1/psi_128_grayscale_blur_90.png" ;
	unsigned char *data = stbi_load(file_name, &x, &y, &comp, 1);
	unsigned char *data_temp = data ;

	if (data == NULL)
	{
		// error
		cout << "Error, data was null" << endl ;
        }
	else
	{
		// process
//		cout << " data = " << data << endl ;
	}

	std::cout << " x = " << x << " y = " << y << " comp = " << comp << std::endl ;

	Real sum = 0.0;
	int k = 1 ;
	int idx ;
	for( int i = 0 ; i < res_x ; i++ )
	for( int j = 0 ; j < res_y ; j++ )
	{
//		std::cout << Real(*data) << std::endl ;
		idx = pressure_modification.index(i, j, k);
		pressure_modification[idx] = Real(*data_temp);
		data_temp++;
		sum += pressure_modification[idx];
	}

	Real max_value = 0.0 ;
//	Make the maximum value 1 and accordingly scale other values
	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			if(pressure_modification[idx] > max_value)
			{
				max_value = pressure_modification[idx] ;
			}
		}
	}

	std::cout << " max value = " << max_value << std::endl ;

	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			pressure_modification[idx] /= (max_value);
			pressure_modification[idx] *= (multiplier);
		}
	}

	stbi_image_free(data);
}

PYTHON void create_target_density_convolve_from_image(Grid<Real>& pressure_modification, Real multiplier=1.0, int res_x = 64, int res_y = 64)
{
	int x, y, comp;
	char *file_name = "/home/tushar/manta_mikey_1/manta/build/pictures/february_20/templates/1/phi_128_grayscale_blur_64.png" ;
	unsigned char *data = stbi_load(file_name, &x, &y, &comp, 1);
	unsigned char *data_temp = data ;

	if (data == NULL)
	{
		// error
		cout << "Error, data was null" << endl ;
        }
	else
	{
		// process
//		cout << " data = " << data << endl ;
	}

	std::cout << " x = " << x << " y = " << y << " comp = " << comp << std::endl ;

	Real sum = 0.0;
	int k = 1 ;
	int idx ;
	for( int i = 0 ; i < res_x ; i++ )
	for( int j = 0 ; j < res_y ; j++ )
	{
//		std::cout << Real(*data) << std::endl ;
		idx = pressure_modification.index(i, j, k);
		pressure_modification[idx] = Real(*data_temp);
		data_temp++;
		sum += pressure_modification[idx];
	}

	Real max_value = 0.0 ;
//	Make the maximum value 1 and accordingly scale other values
	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			if(pressure_modification[idx] > max_value)
			{
				max_value = pressure_modification[idx] ;
			}
		}
	}

	std::cout << " max value = " << max_value << std::endl ;

	for(int i = 0; i < res_x; ++i)
	{
		for(int j = 0; j < res_y; ++j)
		{        
			idx = pressure_modification.index(i, j, k);
			pressure_modification[idx] /= (max_value);
			pressure_modification[idx] *= (multiplier);
		}
	}

	stbi_image_free(data);
}


//	This writes the vtk files
PYTHON	void do_vtk(int file_out, int t, Real xlength, Real ylength, int res_x, int res_y, Real dx, Real dy, MACGrid& vel, Grid<Real>& pressure)
{
	int imax = res_x ; int jmax = res_y ;
	int output_count = t ;

//	Step 1 : Create U, V, P
	Real **U = matrix(0, imax, 0, jmax + 1);
	Real **V = matrix(0, imax + 1, 0, jmax);
	Real **P = matrix(0, imax + 1, 0, jmax + 1);

	int i, j, k = 0 ;
	for(i = 1; i <= imax; ++i)
	for(j = 1; j <= jmax; ++j)
	{
//	This comment is from the original file
		/* boundary values will be overwritten anyway */
		int idx = pressure.index(i,j,k);
		U[i][j] = vel[idx].x;
		V[i][j] = vel[idx].y;
		P[i][j] = pressure[idx];
	}

	char file_out_char[75] = "/home/tushar/manta_mikey_1/manta/build/pictures/january_3/1/dummy" ;
	write_vtkFile(file_out_char, output_count, xlength, ylength, imax, jmax, dx, dy, U, V, P);

//	free dynamic memory
	free_matrix( U, 0, imax , 0, jmax + 1 );
	free_matrix( V, 0, imax + 1, 0, jmax );
	free_matrix( P, 0, imax + 1, 0, jmax + 1 );
}

//	This is the time-varying translated perlin template being created with grid-size equal to the flag grid
//	It is created on January 9, 2013
PYTHON void create_perlin_template_varying_translated(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real frequency_changer=1.0)
{

	frequency_changer = ( ( int(frequency_changer) + 150) % 300 ) * 0.1;
	std::cout << "frequency_changer = " << std::endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x / (double)width;
			hx = hx * frequency_changer ;
			double hy = (double)y / (double)height;
			hy = hy * frequency_changer ;
			noise = stb_perlin_noise3(hx,hy,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

//	char Result[8]; // string which will contain the number
//	sprintf ( Result, "%d", frequency_changer ); // %d makes the result be a decimal integer
//	string str_frequency_changer(Result);
//	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/7/templates/perlin" + str_frequency_changer + ".png" ;
//	const char *file_name_output = str.c_str();

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

//	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
//	if(a == 1)
//	{
//		std::cout<<"SUCCESS"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"FAILURE"<<std::endl;
//	}
}


//	This is the time-varying scaled perlin template being created with grid-size equal to the flag grid
//	It is created on January 9, 2013
PYTHON void create_perlin_template_varying_scaled(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real frequency_changer=1.0)
{

	frequency_changer = ( ( int(frequency_changer) + 150) % 300 ) * 0.1;
	std::cout << "frequency_changer = " << std::endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x / (double)width;
			hx = hx * frequency_changer ;
			double hy = (double)y / (double)height;
			hy = hy * frequency_changer ;
			noise = stb_perlin_noise3(hx,hy,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

//	char Result[8]; // string which will contain the number
//	sprintf ( Result, "%d", frequency_changer ); // %d makes the result be a decimal integer
//	string str_frequency_changer(Result);
//	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/7/templates/perlin" + str_frequency_changer + ".png" ;
//	const char *file_name_output = str.c_str();

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

//	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
//	if(a == 1)
//	{
//		std::cout<<"SUCCESS"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"FAILURE"<<std::endl;
//	}
}

//	This is the Perlin noise template being created
//	It has the same size as that of the grid.
//	The idea is to repeat it in some way
//	This is the first Perlin template being created with grid-size equal to the flag grid
PYTHON void create_perlin_template_varying(Grid<Real>& pressure_modification, int time, Real multiplier = 4.0, int res_x=128, int res_y=128)
{
	std::cout << " TIME STEP " << time << std::endl ;	
	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	int num_of_templates = 25 ;
	for(int x = 0; x<width; x++)
	{
		for(int y = 0; y<height; y++)
		{
			double hx = (double)x / (double)width;
			double hy = (double)y / (double)height;

			int period_changer = (time % num_of_templates) ;
			int p = period_changer ;

			noise = stb_perlin_noise3(hx*p,hy*p,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	std::cout << " POINT 1 " << std::endl ;

	for(int x = 0; x < res_x; x++)
	{
		for(int y = 0; y < res_y; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

	std::cout << " POINT 2 " << std::endl ;
// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 0; x < res_x; x++)
	{
		for(int y = 0; y < res_y; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/scenes/perlin_pictures/grid_size_1/perlin_%2d.png"  ;
	int t_duplicate_1 = time ;
	int t_duplicate_2 = time ;

	unsigned int number_of_digits = 0;
//	Find the number of digits in t with number_of_digits
	do 
	{
		++number_of_digits; 
		t_duplicate_1 /= 10;
	} while (t_duplicate_1);

	std::cout << " POINT 3 " << std::endl ;

//	std::cout << "Number of digits is " << number_of_digits << std::endl ;

	char *part_1 = "/home/tushar/manta_mikey_1/manta/build/pictures/december_17/" ;
	int len_1 = strlen(part_1);
	char *part_2 = ( char*) calloc (number_of_digits,sizeof( char)) ;
	int len_2 = number_of_digits;
	char *part_3 = "_dens.png" ;
	int len_3 = strlen(part_3);
	int total_length = len_1 + len_2 + len_3 ;
	char *file_name_output = ( char*) calloc (total_length,sizeof( char));


	std::cout << " POINT 8 " << std::endl ;

	for( unsigned int i = 0 ; i < number_of_digits ; i++)
	{
		int digit = t_duplicate_2/(pow(10,number_of_digits-i-1)) ;
		t_duplicate_2 = t_duplicate_2 % (int((pow(10,number_of_digits - i - 1))));
		char *num = ( char*) calloc (1,sizeof( char));
//		Warning : This is only to convert a single digit int to char
		char dig = (char)(((int)'0')+digit);
		num[0] = dig ;
		strcat (part_2,num);
	}
	std::cout << " POINT 9 " << std::endl ;

	strcat (file_name_output,part_1);
	strcat (file_name_output,part_2);
	strcat (file_name_output,part_3);

//	int a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);

//	free (part_2);
//	free (file_name_output);
//	if(a == 1)
//	{
//		std::cout<<"SUCCESS"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"FAILURE"<<std::endl;
//	}
	std::cout << " POINT 10 " << std::endl ;
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Getting frequencies from the audio
PYTHON void create_perlin_template_sine_warping(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time=1, Real x_factor_1=1.0, Real y_factor_1=1.0, Real x_factor_2=1.0, Real y_factor_2=1.0, Real x_factor_3=1.0, Real y_factor_3=1.0, int time_steps=30,Real start_x=5.0, Real start_y=5.0)
{

//	We move from x:y = start_x:start_y to x:y start_y:start_y
//	Real frequency_changer_x = start_x ;
//	Real frequency_changer_y = start_y ;

//	int time_depender = ((time - 1) % time_steps) ;
//	if(time_depender < (time_steps/2))
//	{
//		frequency_changer_x = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
//		frequency_changer_y = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
//	}
//	else
//	{
//		time_depender = time_depender - time_steps/2 ;
//		frequency_changer_x = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
//		frequency_changer_y = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
//	}

//	std::ofstream outfile;
//	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

//	outfile << " time = " << time << endl ;
//	outfile << " x_factor = " << x_factor << endl ;
//	outfile << " y_factor = " << y_factor << endl ;
//	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
//	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
//	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

//Since sin is taken in radians, 2*pi = 6.28
			hx = hx * x_factor_1 ; hy = hy * y_factor_1 ;
			Real frequency_x = hx + x_factor_2 * sin(x_factor_3 * hx + time/4.0) ;
			Real frequency_y = hy + y_factor_2 * sin(y_factor_3 * hy + time/4.0) ;
			noise = stb_perlin_noise3(frequency_x,frequency_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_27/templates/4/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template
//	Grid-size equal to the flag grid
//	x_y_xy
PYTHON void create_perlin_template_x_y_xy(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int res_z=128, int frequency_x=1, int frequency_y=1, int frequency_z=1, string folder_1="1", string folder_2="2")
{
	string dir_name_1 = "mkdir /home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+folder_1 ;
	const char *dir_name_char_1 = dir_name_1.c_str();
	int system_return_1 = system (dir_name_char_1);
	printf ("The value returned was: %d.\n",system_return_1);

	string dir_name_2 = "mkdir /home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+folder_2 ;
	const char *dir_name_char_2 = dir_name_2.c_str();
	int system_return_2 =system (dir_name_char_2);
	printf ("The value returned was: %d.\n",system_return_2);

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y , depth = res_z ;
	double noise_1 = 0.0, noise_2 = 0.0 ;
	unsigned char *noise_data_1 = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	unsigned char *noise_data_2 = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

	for(int z = 1 ; z < depth-1 ; z++)
	{
		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
			//	Changing angle from 0.0 to 360.0 degrees from time = 1 : 301
			//	In degrees
				Real theta = 45;
			//	In radians
				Real PI = 3.14159265 ;
				Real theta_r = -(PI/180.0)* theta;

				double a[3][3];
				int m,n;
				double b[3][3];
				double determinant=0;

			//	double aa = (((double)width/2.0)/width)*x_factor; double bb = (((double)height/2.0)/height)*y_factor;
				double aa = (((double)width/2.0));
				double bb = (((double)height/2.0));

				a[0][0] = cos(theta_r) ;
				a[0][1] = -sin(theta_r) ;
				a[0][2] =  -aa*cos(theta_r)+bb*sin(theta_r)+aa;
				a[1][0] =  sin(theta_r);
				a[1][1] =  cos(theta_r);
				a[1][2] =  -aa*sin(theta_r)-bb*cos(theta_r)+bb;
				a[2][0] =  0.0;
				a[2][1] =  0.0;
				a[2][2] =  1.0;

				for(m=0;m<3;m++)
				{
					determinant = determinant + (a[0][m]*(a[1][(m+1)%3]*a[2][(m+2)%3] - a[1][(m+2)%3]*a[2][(m+1)%3]));
				}

				for(m=0;m<3;m++)
				{
					for(n=0;n<3;n++)
					{
						b[n][m]=((a[(m+1)%3][(n+1)%3]*a[(m+2)%3][(n+2)%3])-(a[(m+1)%3][(n+2)%3]* a[(m+2)%3][(n+1)%3]))/determinant;
					}
				}

				double xdash = (b[0][0])*(x)+(b[0][1])*(y)+(b[0][2])*1 ;
				double ydash = (b[1][0])*(x)+(b[1][1])*(y)+(b[1][2])*1 ;

				double hx = xdash/width ;
				double hy = ydash/width ;
				double hz = (double)z/depth ;

//	Note the reverse in x and y
				noise_1 = stb_perlin_noise3(hx*frequency_x,hy*frequency_y,hz*frequency_z,xWrap=0,yWrap=0,zWrap=0);
				noise_2 = stb_perlin_noise3(hx*frequency_y,hy*frequency_x,hz*frequency_z,xWrap=0,yWrap=0,zWrap=0);

				noise_1 = (noise_1 + 1.0) / 2.0; // set range to 0 - 1
				noise_2 = (noise_2 + 1.0) / 2.0; // set range to 0 - 1

				int linear_index = y * width + x ;
				unsigned char col_1 = int(noise_1 * 255);
				unsigned char col_2 = int(noise_2 * 255);
				noise_data_1[linear_index] = col_1 ;
				noise_data_2[linear_index] = col_2 ;
			}
		}

		char Result[8]; // string which will contain the number
		sprintf ( Result, "%d", int(z) ); // %d makes the result be a decimal integer

		string str_frequency_changer(Result);
		if(z < 10)
		{ str_frequency_changer = "00" + str_frequency_changer ; }
		else if((z >= 10) && (z <=99))
		{str_frequency_changer = "0" + str_frequency_changer ;}

		string str_1 = "/home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+ folder_1 + "/" + str_frequency_changer + ".png" ;
		string str_2 = "/home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+ folder_2 + "/" + str_frequency_changer + ".png" ;

		const char *file_name_output_1 = str_1.c_str();
		std::cout << " file_name_output_1 = " << file_name_output_1 << std::endl ;
		const char *file_name_output_2 = str_2.c_str();
		std::cout << " file_name_output_2 = " << file_name_output_2 << std::endl ;

		int a = 1 ;
		a = stbi_write_png(file_name_output_1, width, height, 1, noise_data_1, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}

		a = 1 ;
		a = stbi_write_png(file_name_output_2, width, height, 1, noise_data_2, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}
	}
	free (noise_data_1);
	free (noise_data_2);
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_27 (Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int res_z=128, int frequency_x=1, int frequency_y=1, int frequency_z=1, string folder_1="1", string folder_2="2", string folder_3="3")
{
	string dir_name_1 = "mkdir /home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+folder_1 ;
	const char *dir_name_char_1 = dir_name_1.c_str();
	int system_return_1 = system (dir_name_char_1);
	printf ("The value returned was: %d.\n",system_return_1);

	string dir_name_2 = "mkdir /home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+folder_2 ;
	const char *dir_name_char_2 = dir_name_2.c_str();
	int system_return_2 =system (dir_name_char_2);
	printf ("The value returned was: %d.\n",system_return_2);

	string dir_name_3 = "mkdir /home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+folder_3 ;
	const char *dir_name_char_3 = dir_name_3.c_str();
	int system_return_3 =system (dir_name_char_3);
	printf ("The value returned was: %d.\n",system_return_3);

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y , depth = res_z ;
	double noise_1 = 0.0, noise_2 = 0.0, noise_3 = 0.0 ;
	unsigned char *noise_data_1 = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	unsigned char *noise_data_2 = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	unsigned char *noise_data_3 = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

	for(int z = 1 ; z < depth-1 ; z++)
	{
		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				double hx = (double)x/width ;
				double hy = (double)y/height ;
				double hz = (double)z/depth ;
				noise_1 = stb_perlin_noise3(hx*frequency_x,hy*frequency_y,hz*frequency_z,xWrap=0,yWrap=0,zWrap=0);
//Note the reverse in x and y
				noise_2 = stb_perlin_noise3(hx*frequency_y,hy*frequency_x,hz*frequency_z,xWrap=0,yWrap=0,zWrap=0);
				noise_3 = (noise_1+noise_2)/2.0;

				noise_1 = (noise_1 + 1.0) / 2.0; // set range to 0 - 1
				noise_2 = (noise_2 + 1.0) / 2.0; // set range to 0 - 1
				noise_3 = (noise_3 + 1.0) / 2.0; // set range to 0 - 1

				idx = pressure_modification.index(x , y , 1);
				pressure_modification[idx] = noise_1;

				int linear_index = y * width + x ;
				unsigned char col_1 = int(noise_1 * 255);
				unsigned char col_2 = int(noise_2 * 255);
				unsigned char col_3 = int(noise_3 * 255);
				noise_data_1[linear_index] = col_1 ;
				noise_data_2[linear_index] = col_2 ;
				noise_data_3[linear_index] = col_3 ;
			}
		}

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				idx = pressure_modification.index(x , y , 1);
				pressure_modification[idx] /= pressure_modification.getMaxAbsValue() ;
				pressure_modification[idx] *= multiplier;
			}
		}

		char Result[8]; // string which will contain the number
		sprintf ( Result, "%d", int(z) ); // %d makes the result be a decimal integer

		string str_frequency_changer(Result);
		if(z < 10)
		{ str_frequency_changer = "00" + str_frequency_changer ; }
		else if((z >= 10) && (z <=99))
		{str_frequency_changer = "0" + str_frequency_changer ;}

		string str_1 = "/home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+ folder_1 + "/" + str_frequency_changer + ".png" ;
		string str_2 = "/home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+ folder_2 + "/" + str_frequency_changer + ".png" ;
		string str_3 = "/home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+ folder_3 + "/" + str_frequency_changer + ".png" ;

		const char *file_name_output_1 = str_1.c_str();
		std::cout << " file_name_output_1 = " << file_name_output_1 << std::endl ;
		const char *file_name_output_2 = str_2.c_str();
		std::cout << " file_name_output_2 = " << file_name_output_2 << std::endl ;
		const char *file_name_output_3 = str_3.c_str();
		std::cout << " file_name_output_3 = " << file_name_output_3 << std::endl ;

		int a = 1 ;
		a = stbi_write_png(file_name_output_1, width, height, 1, noise_data_1, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}

		a = 1 ;
		a = stbi_write_png(file_name_output_2, width, height, 1, noise_data_2, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}

		a = 1 ;
		a = stbi_write_png(file_name_output_3, width, height, 1, noise_data_3, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}

	}
	free (noise_data_1);
	free (noise_data_2);
	free (noise_data_3);
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_26 (Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int res_z=128, int frequency_x=1, int frequency_y=1, int frequency_z=1, int z=1, string folder_templates_number="1" )
{
	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y , depth = res_z ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				double hx = (double)x/width ;
				double hy = (double)y/height ;
				double hz = (double)z/depth ;
				hz = 1.75 ;
				noise = stb_perlin_noise3(hx*frequency_x,hy*frequency_y,hz*frequency_z,xWrap=0,yWrap=0,zWrap=0);
				noise = (noise + 1.0) / 2.0; // set range to 0 - 1

				if(noise < 0.0){std::cout << " IMPOSSIBLE NEGATIVE VALUE IS POSSIBLE " << std::endl ;}

				idx = pressure_modification.index(x , y , 1);
				pressure_modification[idx] = noise;

				int linear_index = y * width + x ;
				unsigned char col = int(noise * 255);
				noise_data[linear_index] = col ;
			}
		}

		Real max_value = pressure_modification.getMaxValue() ;

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				idx = pressure_modification.index(x , y , 1);
				pressure_modification[idx] /= max_value ;
				pressure_modification[idx] *= multiplier;
			}
		}

		char Result[8]; // string which will contain the number
		sprintf ( Result, "%d", int(z) ); // %d makes the result be a decimal integer

		string str_frequency_changer(Result);
		if(z < 10)
		{ str_frequency_changer = "00" + str_frequency_changer ; }
		else if((z >= 10) && (z <=99))
		{str_frequency_changer = "0" + str_frequency_changer ;}

		string str = "/home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+ folder_templates_number + "/" + str_frequency_changer + ".png" ;
		const char *file_name_output = str.c_str();
//		std::cout << " file_name_output = " << file_name_output << std::endl ;

	//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

		int a = 1 ;
//		a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}
	free (noise_data);
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
PYTHON void create_perlin_template_25 (Grid<Real>& pressure_modification, Real multiplier = 4.0, int res_x=128, int res_y=128, int time = 1, Real frequency_x=10.0, Real frequency_y=10.0, Real sign_translate_x = 0.01, Real sign_translate_y = 0.01)
{
	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_x+sign_translate_x*time,hy*frequency_y + sign_translate_y *time,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/february_26/1/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_24 (Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time = 1, Real start_x=5.0,Real start_y=15.0,int time_steps=30)
{
//	We move from x:y = start_x:start_y to x:y start_y:start_y
	Real frequency_changer_x = start_x;
	Real frequency_changer_y = start_y;

//	Real factor_increment = ((start_y - start_x)/time_steps)*2.0;

	int time_depender = ((time - 1) % time_steps) ;
	if(time_depender < (time_steps/2))
	{
		frequency_changer_x = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
		frequency_changer_y = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
	}
	else
	{
		time_depender = time_depender - time_steps/2 ;
		frequency_changer_x = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
		frequency_changer_y = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
	}

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/february_26/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " time_depender = " << time_depender << endl ;
	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

//			noise = stb_perlin_noise3(hx*frequency_changer_x+0.01*time,hy*frequency_changer_y+0.01*time,1.75,xWrap=0,yWrap=0,zWrap=0);
			Real sign_translate_x = 0.005, sign_translate_y = 0.005 ;
//			int remainder = (time % 20) ;
//			if (remainder < 5)
//			{
//				sign_translate_x = 0.01 ; sign_translate_y = 0.01 ;
//			}
//			else if ((remainder > 4)&&(remainder < 10))
//			{
//				sign_translate_x = 0.01 ; sign_translate_y = -0.01 ;
//			}
//			else if ((remainder > 9)&&(remainder < 15))
//			{
//				sign_translate_x = -0.01 ; sign_translate_y = -0.01 ;
//			}
//			else
//			{
//				sign_translate_x = -0.01 ; sign_translate_y = 0.01 ;
//			}
			noise = stb_perlin_noise3(hx*frequency_changer_x+sign_translate_x*time,hy*frequency_changer_y + sign_translate_y *time,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

//// Just checking
//	double max_perlin_pressure_value = -2.0 ;
//	for(int x = 1; x<width-1; x++)
//	{
//		for(int y = 1; y<height-1; y++)
//                {
//			idx = pressure_modification.index(x , y , 1);
//			if(max_perlin_pressure_value < pressure_modification[idx])
//			{
//				max_perlin_pressure_value = pressure_modification[idx] ;
//			}
//                }
//	}

//	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
//	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
////	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/february_26/1/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Getting multiplier from the audio
PYTHON void create_perlin_template_23(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time=1, int ctr=1, Real num=1)
{
	Real frequency_changer_x = 0;
	Real frequency_changer_y = 0;

	int time_depender = ((ctr) % 2) ;
	if(time_depender == 1)
	{
		frequency_changer_x = 7 ;
		frequency_changer_y = 7 * 3 ;
	}
	else
	{
		frequency_changer_y = 7 ;
		frequency_changer_x = 7 * 3 ;
	}

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " ctr = " << ctr << endl ;
	outfile << " num = " << num << endl ;
	outfile << " time_depender = " << time_depender << endl ;
	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
	outfile << " mulitplier = " << multiplier << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x,hy*frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/february_3/templates/1/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template
//	Grid-size equal to the flag grid
//	Rotation with alternating x and y
PYTHON void create_perlin_template_22(Grid<Real>& pressure_modification, Real multiplier = 1.0, int res_x=128, int res_y=128, int time = 1, Real x_factor=1.0, Real y_factor=1.0)
{
	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

//	Changing angle from 0.0 to 360.0 degrees from time = 1 : 301
//	In degrees
	Real theta = (1.2)*time  - 1.2;
//	In radians
	Real PI = 3.14159265 ;
	Real theta_r = -(PI/180.0)* theta;

	outfile << " angle in degrees = " << theta << endl ;
	outfile << " angle in radians = " << theta_r << endl ;
	outfile << " time = " << time << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
	double a[3][3];
	int m,n;
	double b[3][3];
	double determinant=0;

//	double aa = (((double)width/2.0)/width)*x_factor; double bb = (((double)height/2.0)/height)*y_factor;
	double aa = (((double)width/2.0));
	double bb = (((double)height/2.0));

	a[0][0] = cos(theta_r) ;
	a[0][1] = -sin(theta_r) ;
	a[0][2] =  -aa*cos(theta_r)+bb*sin(theta_r)+aa;
	a[1][0] =  sin(theta_r);
	a[1][1] =  cos(theta_r);
	a[1][2] =  -aa*sin(theta_r)-bb*cos(theta_r)+bb;
	a[2][0] =  0.0;
	a[2][1] =  0.0;
	a[2][2] =  1.0;

	for(m=0;m<3;m++)
	{
		determinant = determinant + (a[0][m]*(a[1][(m+1)%3]*a[2][(m+2)%3] - a[1][(m+2)%3]*a[2][(m+1)%3]));
	}

	for(m=0;m<3;m++)
	{
		for(n=0;n<3;n++)
		{
			b[n][m]=((a[(m+1)%3][(n+1)%3]*a[(m+2)%3][(n+2)%3])-(a[(m+1)%3][(n+2)%3]* a[(m+2)%3][(n+1)%3]))/determinant;
		}
	}

			double xdash = (b[0][0])*(x)+(b[0][1])*(y)+(b[0][2])*1 ;
			double ydash = (b[1][0])*(x)+(b[1][1])*(y)+(b[1][2])*1 ;
			double hx  ;
			double hy  ;
//			double center_x ;
//			double center_y ;

			hx = xdash/width ;
			hy = ydash/width ;

			noise = stb_perlin_noise3(hx*x_factor,hy*y_factor,1.75,xWrap=0,yWrap=0,zWrap=0);
			double temp1 = noise;
			
			noise = (temp1 + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_30/templates/1/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Rotation with alternating x and y
PYTHON void create_perlin_template_21(Grid<Real>& pressure_modification, Real multiplier = 1.0, int res_x=128, int res_y=128, int time = 1, Real x_factor=1.0, Real y_factor=1.0)
{
	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

//	Changing angle from 0.0 to 360.0 degrees from time = 1 : 301
//	In degrees
	Real theta = (1.2)*time  - 1.2;
//	In radians
	Real PI = 3.14159265 ;
	Real theta_r = -(PI/180.0)* theta;

	outfile << " angle in degrees = " << theta << endl ;
	outfile << " angle in radians = " << theta_r << endl ;
	outfile << " time = " << time << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
	double a[3][3];
	int m,n;
	double b[3][3];
	double determinant=0;

//	double aa = (((double)width/2.0)/width)*x_factor; double bb = (((double)height/2.0)/height)*y_factor;
	double aa = (((double)width/2.0)/width);
	double bb = (((double)height/2.0)/height);

	a[0][0] = cos(theta_r) ;
	a[0][1] = -sin(theta_r) ;
	a[0][2] =  -aa*cos(theta_r)+bb*sin(theta_r)+aa;
	a[1][0] =  sin(theta_r);
	a[1][1] =  cos(theta_r);
	a[1][2] =  -aa*sin(theta_r)-bb*cos(theta_r)+bb;
	a[2][0] =  0.0;
	a[2][1] =  0.0;
	a[2][2] =  1.0;

	for(m=0;m<3;m++)
	{
		determinant = determinant + (a[0][m]*(a[1][(m+1)%3]*a[2][(m+2)%3] - a[1][(m+2)%3]*a[2][(m+1)%3]));
	}


	for(m=0;m<3;m++)
	{
		for(n=0;n<3;n++)
		{
			b[n][m]=((a[(m+1)%3][(n+1)%3]*a[(m+2)%3][(n+2)%3])-(a[(m+1)%3][(n+2)%3]* a[(m+2)%3][(n+1)%3]))/determinant;
		}
	}

			double xdash = ((double)x/width)*x_factor ;
			double ydash = ((double)y/height)*y_factor ;
			double hx  ;
			double hy  ;
//			double center_x ;
//			double center_y ;

			hx = (b[0][0])*(xdash)+(b[0][1])*(ydash)+(b[0][2])*1 ;
			hy = (b[1][0])*(xdash)+(b[1][1])*(ydash)+(b[1][2])*1 ;

			noise = stb_perlin_noise3(hx,hy,1.75,xWrap=0,yWrap=0,zWrap=0);
			double temp1 = noise;
			
			noise = (temp1 + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_27/templates/1/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Rotation with alternating x and y
PYTHON void create_perlin_template_20(Grid<Real>& pressure_modification, Real multiplier = 1.0, int res_x=128, int res_y=128, int time = 1, Real x_factor=1.0, Real y_factor=1.0)
{

	Real frequency_changer_x = 0.0 ;
	Real frequency_changer_y = 0.0 ;

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

//	Changing angle from 0.0 to 360.0 degrees from time = 1 : 301
//	In degrees
	Real theta = (1.2)*time  - 1.2;
//	In radians
	Real PI = 3.14159265 ;
	Real theta_r = -(PI/180.0)* theta;

	outfile << " angle in degrees = " << theta << endl ;
	outfile << " angle in radians = " << theta_r << endl ;
	outfile << " time = " << time << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
	double a[3][3];
	int m,n;
	double b[3][3];
	double determinant=0;

//	float xdash = x*x_factor, ydash = y*y_factor;
//	double xdash = (double)x/width ;
//	double ydash = (double)y/height ;
	double xdash = (double)x ;
	double ydash = (double)y ;

	xdash *= x_factor; ydash *= y_factor;

//	double aa = (((double)width/2.0)/width)*x_factor; double bb = (((double)height/2.0)/height)*y_factor;
	double aa = (((double)width/2.0))*x_factor; double bb = (((double)height/2.0))*y_factor;

	a[0][0] = cos(theta_r) ;
	a[0][1] = -sin(theta_r) ;
	a[0][2] =  -aa*cos(theta_r)+bb*sin(theta_r)+aa;
	a[1][0] =  sin(theta_r);
	a[1][1] =  cos(theta_r);
	a[1][2] =  -aa*sin(theta_r)+bb*cos(theta_r)+bb;
	a[2][0] =  0.0;
	a[2][1] =  0.0;
	a[2][2] =  1.0;

//	cout<<"\nThe entered matrix is:\n";
//	for(m=0;m<3;m++)
//	{
//		for(n=0;n<3;n++)
//		{
//			cout<<a[m][n]<<"  ";
//		}
//		cout<<"\n";
//	}
	for(m=0;m<3;m++)
	{
		determinant = determinant + (a[0][m]*(a[1][(m+1)%3]*a[2][(m+2)%3] - a[1][(m+2)%3]*a[2][(m+1)%3]));
	}


	for(m=0;m<3;m++)
	{
		for(n=0;n<3;n++)
		{
			b[m][n]=((a[(m+1)%3][(n+1)%3]*a[(m+2)%3][(n+2)%3])-(a[(m+1)%3][(n+2)%3]* a[(m+2)%3][(n+1)%3]))/determinant;
		}
//		cout<<"\n";
	}


//	std::cout << "hx = " << hx << std::endl ;
//	std::cout << "hy = " << hy << std::endl ;
//			double factor ;
			double hx  ;
			double hy  ;
//			double center_x ;
//			double center_y ;

	hx = (b[0][0])*(xdash)+(b[0][1])*(ydash)+(b[0][2])*1 ;
	hy = (b[1][0])*(xdash)+(b[1][1])*(ydash)+(b[1][2])*1 ;

//			if(time % 2 == 1)
//			{
//				factor = 9.0 ;
//				hx = factor*((double)x/width) ;
//				hy = 3*factor*((double)y/height) ;

//				center_x = (width/factor)/2.0 ;
//				center_y = (height/(factor*3.0))/2.0 ;
//			}
//			else
//			{
//				factor = 9.0 ;
//				hx = 3*factor*((double)x/width) ;
//				hy = factor*((double)y/height) ;

//				center_x = (width/(factor*3.0))/2.0 ;
//				center_y = (height/factor)/2.0 ;
//			}

//			frequency_changer_x = (hx-center_x)*cos(angle) + (hy-center_y)*sin(angle) ;
//			frequency_changer_y = -(hx-center_x)*sin(angle) + (hy-center_y)*cos(angle) ;

			frequency_changer_x = hx ;
			frequency_changer_y = hy ;

			noise = stb_perlin_noise3(frequency_changer_x,frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			double temp1 = noise;
			
			noise = (temp1 + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_27/templates/1/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Getting frequencies from the audio
//	Uncomment this later on
PYTHON void create_perlin_template_19(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time=1, Real x_factor=1.0, Real y_factor=1.0, int time_steps=30,Real start_x=5.0, Real start_y=5.0)
{

//	We move from x:y = start_x:start_y to x:y start_y:start_y
	Real frequency_changer_x = start_x;
	Real frequency_changer_y = start_y;

	int time_depender = ((time - 1) % time_steps) ;
	if(time_depender < (time_steps/2))
	{
		frequency_changer_x = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
		frequency_changer_y = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
	}
	else
	{
		time_depender = time_depender - time_steps/2 ;
		frequency_changer_x = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
		frequency_changer_y = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
	}


	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " x_factor = " << x_factor << endl ;
	outfile << " y_factor = " << y_factor << endl ;
	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

//Since sin is taken in radians, 2*pi = 6.28
			hx = hx * x_factor ; hy = hy * y_factor ;
			Real frequency_x = hx + frequency_changer_x * sin(hx + time/4.0) ;
			Real frequency_y = hy + frequency_changer_y * sin(hy + time/4.0) ;
//			frequency_changer_x = hx + 0.4 * sin(4*hx ) ;
//			frequency_changer_y = hy + 0.4 * sin(4*hy ) ;
//			frequency_changer_x = hx ;
//			frequency_changer_y = hy ;
			noise = stb_perlin_noise3(frequency_x,frequency_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_27/templates/4/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
PYTHON void create_perlin_template_18(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time=1, Real x_factor=1.0, Real y_factor=1.0)
{
	Real frequency_changer_x = 0;
	Real frequency_changer_y = 0;

//	int time_depender = ((time) % 2) ;

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " x_factor = " << x_factor << endl ;
	outfile << " y_factor = " << y_factor << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

//Since sin is taken in radians, 2*pi = 6.28
			hx = hx * x_factor ; hy = hy * y_factor ;
			frequency_changer_x = hx + 0.1 * sin(7*hx + time/4.0) ;
			frequency_changer_y = hy + 0.1 * sin(1*hy + time/4.0) ;
//			frequency_changer_x = hx + 0.4 * sin(4*hx ) ;
//			frequency_changer_y = hy + 0.4 * sin(4*hy ) ;
//			frequency_changer_x = hx ;
//			frequency_changer_y = hy ;
			noise = stb_perlin_noise3(frequency_changer_x,frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_27/templates/2/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_17 (Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time = 1, Real start_x=5.0,Real start_y=15.0,int time_steps=30)
{
//	We move from x:y = start_x:start_y to x:y start_y:start_y
	Real frequency_changer_x = start_x;
	Real frequency_changer_y = start_y;

//	Real factor_increment = ((start_y - start_x)/time_steps)*2.0;

	int time_depender = ((time - 1) % time_steps) ;
	if(time_depender < (time_steps/2))
	{
		frequency_changer_x = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
		frequency_changer_y = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
	}
	else
	{
		time_depender = time_depender - time_steps/2 ;
		frequency_changer_x = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
		frequency_changer_y = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
	}

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " time_depender = " << time_depender << endl ;
	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x-0.01*time,hy*frequency_changer_y-0.01*time,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_25/templates/4/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Getting frequencies from the audio
PYTHON void create_perlin_template_16(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time=1, Real x_factor=1.0, Real y_factor=1.0)
{
	Real frequency_changer_x = 0;
	Real frequency_changer_y = 0;

//	int time_depender = ((time) % 2) ;

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " x_factor = " << x_factor << endl ;
	outfile << " y_factor = " << y_factor << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

//Since sin is taken in radians, 2*pi = 6.28
			hx = hx * x_factor ; hy = hy * y_factor ;
//			frequency_changer_x = hx + 0.5 * sin(4*hx + time/4.0) ;
//			frequency_changer_y = hy + 0.5 * sin(4*hy + time/4.0) ;
			frequency_changer_x = hx + 0.9 * sin(3*hx ) ;
			frequency_changer_y = hy + 0.9 * sin(3*hy ) ;
			noise = stb_perlin_noise3(frequency_changer_x+0.05*time,frequency_changer_y+0.05*time,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_25/templates/9/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Getting frequencies from the audio
PYTHON void create_perlin_template_15(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real ffrequency = 1.0,int time=1)
{
	Real frequency_changer_x = 0;
	Real frequency_changer_y = 0;

	int time_depender = ((time) % 2) ;
	if(time_depender == 1)
	{
		frequency_changer_x = ffrequency ;
		frequency_changer_y = ffrequency * 3 ;
//		frequency_changer_x = 8.0 ;
//		frequency_changer_y = 8.0 * 3 ;
	}
	else
	{
		frequency_changer_y = ffrequency ;
		frequency_changer_x = ffrequency * 3 ;
//		frequency_changer_y = 8.0 ;
//		frequency_changer_x = 8.0 * 3 ;
	}

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " time_depender = " << time_depender << endl ;
	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x+0.03*time,hy*frequency_changer_y+0.03*time,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_24/templates/1/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Getting multiplier from the audio
PYTHON void create_perlin_template_14(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time=1)
{
	Real frequency_changer_x = 0;
	Real frequency_changer_y = 0;

	int time_depender = ((time) % 2) ;
	if(time_depender == 1)
	{
		frequency_changer_x = 6 ;
		frequency_changer_y = 6 * 3 ;
	}
	else
	{
		frequency_changer_y = 6 ;
		frequency_changer_x = 6 * 3 ;
	}

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " time_depender = " << time_depender << endl ;
	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
	outfile << " mulitplier = " << multiplier << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x+0.05*time,hy*frequency_changer_y+0.05*time,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_22/templates/3/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Getting frequencies from the audio
PYTHON void create_perlin_template_13(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real ffrequency = 1.0,int time=1)
{
	Real frequency_changer_x = 0;
	Real frequency_changer_y = 0;

	int time_depender = ((time) % 2) ;
	if(time_depender == 1)
	{
		frequency_changer_x = ffrequency ;
		frequency_changer_y = ffrequency * 3 ;
	}
	else
	{
		frequency_changer_y = ffrequency ;
		frequency_changer_x = ffrequency * 3 ;
	}

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " time_depender = " << time_depender << endl ;
	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x,hy*frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_23/templates/1/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Rotation with alternating x and y
PYTHON void create_perlin_template_12(Grid<Real>& pressure_modification, Real multiplier = 1.0, int res_x=128, int res_y=128, int time = 1)
{
	Real frequency_changer_x = 0.0 ;
	Real frequency_changer_y = 0.0 ;

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
//			double factor ;
			double hx  =  ((double)x/width) ;
			double hy  =  ((double)y/height);

			if(time % 2 == 1)
			{
				frequency_changer_x = 15.0 ;
				frequency_changer_y = 45.0 ;
			}
			else
			{
				frequency_changer_x = 45.0 ;
				frequency_changer_y = 15.0 ;
			}

			noise = stb_perlin_noise3(hx*frequency_changer_x+0.03*time,hy*frequency_changer_y+0.03*time,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_17/templates/9/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Rotation with alternating x and y
PYTHON void create_perlin_template_11(Grid<Real>& pressure_modification, Real multiplier = 1.0, int res_x=128, int res_y=128, int time = 1)
{

	Real frequency_changer_x = 0.0 ;
	Real frequency_changer_y = 0.0 ;

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

//	Changing angle from 0.0 to 360.0 degrees
//	In degrees
	Real angle = (1.2)*time  - 6.0;
	outfile << " angle in degrees = " << angle << endl ;
//	In radians
	Real PI = 3.14159265 ;
	angle = (PI/180.0)* angle;
	outfile << " angle in radians = " << angle << endl ;


	outfile << " time = " << time << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double factor ;
			double hx  ;
			double hy  ;
			double center_x ;
			double center_y ;

			if(time % 2 == 1)
			{
				factor = 9.0 ;
				hx = factor*((double)x/width) ;
				hy = 3*factor*((double)y/height) ;

				center_x = (width/factor)/2.0 ;
				center_y = (height/(factor*3.0))/2.0 ;
			}
			else
			{
				factor = 9.0 ;
				hx = 3*factor*((double)x/width) ;
				hy = factor*((double)y/height) ;

				center_x = (width/(factor*3.0))/2.0 ;
				center_y = (height/factor)/2.0 ;
			}

			frequency_changer_x = (hx-center_x)*cos(angle) + (hy-center_y)*sin(angle) ;
			frequency_changer_y = -(hx-center_x)*sin(angle) + (hy-center_y)*cos(angle) ;

			noise = stb_perlin_noise3(frequency_changer_x,frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_17/templates/8/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}



//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Rotation
PYTHON void create_perlin_template_10(Grid<Real>& pressure_modification, Real multiplier = 1.0, int res_x=128, int res_y=128, int time = 1)
{

	Real frequency_changer_x = 0.0 ;
	Real frequency_changer_y = 0.0 ;

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

//	Changing angle from 0.0 to 360.0 degrees
//	In degrees
	Real angle = (1.2)*time  - 6.0;
	outfile << " angle in degrees = " << angle << endl ;
//	In radians
	Real PI = 3.14159265 ;
	angle = (PI/180.0)* angle;
	outfile << " angle in radians = " << angle << endl ;


	outfile << " time = " << time << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double factor = 6.0 ;
			double hx = factor*((double)x/width) ;
			double hy = 3*factor*((double)y/height) ;
//			double hx = (double)x ;
//			double hy = (double)y ;

			double center_x = (width/factor)/2.0 ;
			double center_y = (height/(factor*3.0))/2.0 ;

//			if(time % 2 == 1)
//			{
//				hx = hx * 7.0;
//				hy = hy * 21.0 ;
//			}
//			else
//			{
//				hx = hx * 21.0;
//				hy = hy * 7.0 ;
//			}

			frequency_changer_x = (hx-center_x)*cos(angle) + (hy-center_y)*sin(angle) ;
			frequency_changer_y = -(hx-center_x)*sin(angle) + (hy-center_y)*cos(angle) ;

//			outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
//			outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
//			outfile << endl ;

			noise = stb_perlin_noise3(frequency_changer_x,frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_17/templates/7/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	The magnitude is increased linearly and translating
PYTHON void create_perlin_template_9(Grid<Real>& pressure_modification, int res_x=128, int res_y=128, int time = 1)
{
	Real multiplier = (0.05) * time + (-0.25) ;

	Real frequency_changer_x = 0;
	Real frequency_changer_y = 0;

	int time_depender = ((time) % 2) ;
	if(time_depender == 1)
	{
		frequency_changer_x = 8.0 ;
		frequency_changer_y = 24.0 ;
	}
	else
	{
		frequency_changer_y = 8.0 ;
		frequency_changer_x = 24.0 ;
	}

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " time_depender = " << time_depender << endl ;
	outfile << " multiplier = " << multiplier << endl ;
	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x + 0.025 * time,hy*frequency_changer_y+ 0.025*time,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_17/templates/6/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_8(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time = 1)
{
	Real frequency_changer_x = 0;
	Real frequency_changer_y = 0;

	int time_depender = ((time) % 2) ;
	if(time_depender == 1)
	{
		frequency_changer_x = (1.0/20) * time - (1.0/4) ;
		frequency_changer_y = frequency_changer_x * 3 ;
	}
	else
	{
		frequency_changer_y = (1.0/20) * (time-1) - (1.0/4) ;
		frequency_changer_x = frequency_changer_y * 3 ;
	}

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " time_depender = " << time_depender << endl ;
	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x,hy*frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_17/templates/4/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_7(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time = 1)
{
	Real frequency_changer_x = 15;
	Real frequency_changer_y = 45;

	int time_depender = ((time - 5) % 30) ;
	if(time_depender < 15)
	{
		frequency_changer_x = 15 + 2 * time_depender ;
		frequency_changer_y = 45 - 2 * time_depender ;
	}
	else
	{
		time_depender = time_depender - 15 ;
		frequency_changer_x = 45 - 2 * time_depender ;
		frequency_changer_y = 15 + 2 * time_depender ;
	}

	std::ofstream outfile;
	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

	outfile << " time = " << time << endl ;
	outfile << " time_depender = " << time_depender << endl ;
	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x,hy*frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_17/templates/2/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_6(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int levels = 1)
{

//	frequency_changer = 1.0;
//	std::cout << "frequency_changer = " << frequency_changer << std::endl ;
	std::cout << "levels = " << levels << std::endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{

			double hx = (double)x/width ;
			double hy = (double)y/height ;
			double noise_sum = 0.0 ;
			for(int level = 1 ; level <= levels ; level++ )
			{
				noise = stb_perlin_noise3(hx*pow(2,level),hy*pow(2,level),1.75,xWrap=0,yWrap=0,zWrap=0);
				noise = (noise + 1.0) / 2.0; // set range to 0 - 1
				noise_sum += noise;
			}

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise_sum;
			if(max_perlin_noise_value < noise_sum)
			{
				max_perlin_noise_value = noise_sum ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise_sum * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(levels) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_16/17/template/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Trials to be a done to check what happens if x and y are varied
PYTHON void create_perlin_template_5(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real frequency_changer_x=1.0, Real frequency_changer_y=1.0, int time = 1 )
{

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x,hy*frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);

	string str ;
	if(time < 10)
	{str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_25/templates/2/perlin00" + str_frequency_changer + ".png";}
	else if((time >= 10) && (time <= 99))
	{str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_25/templates/2/perlin0" + str_frequency_changer + ".png";}
	else if((time >= 100) && (time <= 999))
	{str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_25/templates/2/perlin" + str_frequency_changer + ".png";}

	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Trials to be a done to check what happens if x and y are varied
PYTHON void create_perlin_template_4(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real frequency_changer=1.0)
{

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*6,hy*frequency_changer,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(frequency_changer) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_16/7/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//	This is a perlin template
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_3(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int levels = 1,int time = 1)
{

    std::cout << "levels = " << levels << std::endl ;

    int xWrap,yWrap,zWrap;
    int idx ;
    int width = res_x , height = res_y ;
    double max_perlin_noise_value = -2.0 ;
    double noise = 0.0 ;
    unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
    for(int x = 1; x<width-1; x++)
    {
        for(int y = 1; y<height-1; y++)
        {

            double hx = (double)x/width ;
            double hy = (double)y/height ;
            double noise_sum = 0.0 ;
            for(int level = 1 ; level <= levels ; level++ )
            {
                noise = stb_perlin_noise3(hx*pow(2,level-1),hy*pow(2,level-1),1.75,xWrap=0,yWrap=0,zWrap=0);
                noise = (noise + 1.0) / 2.0; // set range to 0 - 1
                if(noise < 0.0){cout<<"ALERT ALERT"<<endl;}
                noise_sum += noise;
            }
            noise_sum /= levels;

            idx = pressure_modification.index(x , y , 1);
            pressure_modification[idx] = noise_sum;
            if(max_perlin_noise_value < noise_sum)
            {
                max_perlin_noise_value = noise_sum ;
            }

            int linear_index = y * width + x ;
            unsigned char col = int(noise_sum * 255);
            noise_data[linear_index] = col ;
        }
    }

    for(int x = 1; x<width-1; x++)
    {
        for(int y = 1; y<height-1; y++)
        {
            idx = pressure_modification.index(x , y , 1);
            pressure_modification[idx] /= max_perlin_noise_value;
            pressure_modification[idx] *= multiplier;
        }
    }

// Just checking
    double max_perlin_pressure_value = -2.0 ;
    for(int x = 1; x<width-1; x++)
    {
        for(int y = 1; y<height-1; y++)
                {
            idx = pressure_modification.index(x , y , 1);
            if(max_perlin_pressure_value < pressure_modification[idx])
            {
                max_perlin_pressure_value = pressure_modification[idx] ;
            }
                }
    }

    std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
    std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

    char Result[8]; // string which will contain the number
    sprintf ( Result, "%d", int(levels) ); // %d makes the result be a decimal integer
    string str_frequency_changer(Result);
    string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_25/templates/5/perlin" + str_frequency_changer + ".png" ;
    const char *file_name_output = str.c_str();
    std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

    int a = 1 ;
    a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
    free (noise_data);
    if(a == 1)
    {
        std::cout<<"SUCCESS"<<std::endl;
    }
    else
    {
        std::cout<<"FAILURE"<<std::endl;
    }
}

////! Kernel: Construct the right-hand side of the poisson equation
//KERNEL(bnd=1)
//void scale_values(Grid<Real>& pressure_modification,Real multiplier,int time,Real max_absolute,Real max_value,Real min_value)
//{
//            int idx = pressure_modification.index(i , j , k) ;
//            pressure_modification[idx] /= max_value ;
//            pressure_modification[idx] *= multiplier ;
//}

////! Kernel: Construct the right-hand side of the poisson equation
////KERNEL(bnd=1, reduce=+) returns(int cnt=0) returns(double sum=0)
//KERNEL(bnd=1)
//void step_1 (Grid<Real>& pressure_modification,int res_x,int res_y,int res_z,int levels,int time)
//{
//    int xWrap,yWrap,zWrap;
//    int idx ;
//    int width = res_x , height = res_y , depth = res_z ;
//    double noise = 0.0 ;

//            double hx = (double)i/width ;
//            double hy = (double)j/height ;
//            double hz = (double)k/depth ;
//            double noise_sum = 0.0 ;
//	    int level = 1 ;
////            for(int level = 1 ; level <= levels ; level++ )
////            {
//                noise = stb_perlin_noise3(hx*pow(2,level-1),hy*pow(2,level-1),hz*pow(2,level-1),xWrap=0,yWrap=0,zWrap=0);
//                noise = (noise + 1.0) / 2.0; // set range to 0 - 1
//                if(noise < 0.0){cout<<"ALERT ALERT"<<endl;}
////                noise_sum += noise;
//                noise_sum = noise;
////            }
////            noise_sum /= levels;

//            idx = pressure_modification.index(i , j , k);
//            pressure_modification[idx] = noise_sum;
////    sum += 1;
////    cnt++;
//}

////	This is a perlin template 
////	Grid-size equal to the flag grid
////	Multiple frequencies added
//PYTHON void create_perlin_template_3_3D_kernel(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int res_z=128, int levels = 1,int time = 1)
//{
//	std::cout << "levels = " << levels << std::endl ;

////Get the noise value for that particular index	and assign to the grid

////	step_1 Put_noise_value_2_cell(pressure_modification,multiplier,res_x,res_y,res_z,levels,time);
//	step_1(pressure_modification,res_x,res_y,res_z,levels,time);

////Get the maximum values
//	Real max_absolute = pressure_modification.getMaxAbsValue();
//	Real max_value = pressure_modification.getMaxValue();
//	Real min_value = pressure_modification.getMinValue();

////Scale the values
//	scale_values(pressure_modification,multiplier,time,max_absolute,max_value,min_value);

//    int xWrap=0,yWrap=0,zWrap=0 ;
//	int width = res_x , height = res_y ;
//    double noise = 0.0 ;
//    unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

//   for(int x = 1; x<width-1; x++)
//    {
//        for(int y = 1; y<height-1; y++)
//        {
//            double hx = (double)x/width ;
//            double hy = (double)y/height ;
//            double noise_sum = 0.0 ;
//	    int level = 1 ;
////            for(int level = 1 ; level <= levels ; level++ )
////            {
//                noise = stb_perlin_noise3(hx*pow(2,level-1),hy*pow(2,level-1),1.75,xWrap=0,yWrap=0,zWrap=0);
//                noise = (noise + 1.0) / 2.0; // set range to 0 - 1
//                if(noise < 0.0){cout<<"ALERT ALERT"<<endl;}
////                noise_sum += noise;
////            }
////            noise_sum /= levels;
//            noise_sum = noise;

//            int linear_index = y * width + x ;
//            unsigned char col = int(noise_sum * 255);
//            noise_data[linear_index] = col ;
//        }
//    }

////	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
////	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;

//	char Result[8]; // string which will contain the number
//	sprintf ( Result, "%d", int(levels) ); // %d makes the result be a decimal integer
//	string str_frequency_changer(Result);
//	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/february_23/templates/1/perlin" + str_frequency_changer + ".png" ;
//	const char *file_name_output = str.c_str();
//	std::cout << " file_name_output = " << file_name_output << std::endl ;

////	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

//	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
//	free (noise_data);
//	if(a == 1)
//	{
//		std::cout<<"SUCCESS"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"FAILURE"<<std::endl;
//	}
//}


//! Kernel: Construct the right-hand side of the poisson equation
KERNEL(bnd=1, reduce=+) returns(int cnt=0) returns(double sum=0)
void test_1 (Grid<Real>& test_grid)
{
    sum += 1;
    cnt++;
}

//! Perform pressure projection of the velocity grid
PYTHON void test(Grid<Real>& test_grid)
{
	test_1 testing (test_grid);
	std::cout << "testing.sum = " << testing.sum << std::endl ;
	std::cout << "testing.cnt = " << testing.cnt << std::endl ;
}


//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Trials to be a done to check what happens if x and y are varied
PYTHON void create_perlin_template_2(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real frequency_changer=1.0)
{

//	frequency_changer = 1.0;
	std::cout << "frequency_changer = " << frequency_changer << std::endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
//			double hx = (double)x / (double)width;
			double hx = (double)x/width ;
//			hx = hx * int(frequency_changer) ;
//			double hy = (double)y / (double)height;
			double hy = (double)y/height ;
//			hy = hy * int(frequency_changer) ;
//			if(((int(frequency_changer)) % 40) < 20)
//			{
//				noise = stb_perlin_noise3(hx*1,hy*64,1.75,xWrap=0,yWrap=0,zWrap=0);
//			}
//			else
//			{
//				noise = stb_perlin_noise3(hx*1,hy*1,1.75,xWrap=0,yWrap=0,zWrap=0);				
//			}	

			noise = stb_perlin_noise3(hx*1,hy*64,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(frequency_changer) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_13/templates/perlin" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();
	std::cout << " file_name_output = " << file_name_output << std::endl ;

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}


//        This is the second template being created 
PYTHON void create_perlin_template_1(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128)
{
	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	std::cout << " Hello World " << std::endl ;	
	for(int x = int(0.25*(width)); x<int(0.75*(width)); x++)
	{
		for(int y = int(0.25*(height)); y < int(0.75*(height)); y++)
		{
			double hx = (double)x / (double)width;
			double hy = (double)y / (double)height;
			noise = stb_perlin_noise3(hx*8.0,hy*8.0,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = int(0.25*(width)); x<int(0.75*(width)); x++)
	{
		for(int y = int(0.25*(height)); y < int(0.75*(height)); y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = int(0.25*(width)); x<int(0.75*(width)); x++)
	{
		for(int y = int(0.25*(height)); y < int(0.75*(height)); y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;
//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/scenes/perlin_pictures/grid_size_2/perlin6.png"  ;
//	int a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
//	if(a == 1)
//	{
//		std::cout<<"SUCCESS"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"FAILURE"<<std::endl;
//	}
}

//        This is the first perlin template being created with grid-size equal to the flag grid
PYTHON void create_perlin_template(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real frequency_changer=1.0)
{

	frequency_changer = 1.0;
	std::cout << "frequency_changer = " << std::endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x / (double)width;
			hx = hx * int(frequency_changer) ;
			double hy = (double)y / (double)height;
			hy = hy * int(frequency_changer) ;
			noise = stb_perlin_noise3(hx,hy,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;
			if(max_perlin_noise_value < noise)
			{
				max_perlin_noise_value = noise ;
			}

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

// Just checking
	double max_perlin_pressure_value = -2.0 ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
                {
			idx = pressure_modification.index(x , y , 1);
			if(max_perlin_pressure_value < pressure_modification[idx])
			{
				max_perlin_pressure_value = pressure_modification[idx] ;
			}
                }
	}

	std::cout << " max_perlin_noise_value = "<< max_perlin_noise_value << std::endl;
	std::cout << " max_perlin_pressure_value = "<< max_perlin_pressure_value << std::endl;
//	std::cout << " PHEW MADE IT MAN !! IT WAS CLOSE !! "<< std::endl;

//	char Result[8]; // string which will contain the number
//	sprintf ( Result, "%d", frequency_changer ); // %d makes the result be a decimal integer
//	string str_frequency_changer(Result);
//	string str = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/7/templates/perlin" + str_frequency_changer + ".png" ;
//	const char *file_name_output = str.c_str();

//	const char *file_name_output = "/home/tushar/manta_mikey_1/manta/build/pictures/january_9/6/template/perlin8.png"  ;

//	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
//	if(a == 1)
//	{
//		std::cout<<"SUCCESS"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"FAILURE"<<std::endl;
//	}
}

// Function Tushar
PYTHON void image_load_1()
{
	int x, y, comp;
	char *file_name = "/home/tushar/manta_mikey_1/manta/scenes/mine/november_24/templates_krueger/his/temp_gray.png" ;
	unsigned char *data = stbi_load(file_name, &x, &y, &comp, 1);

	if (data == NULL)
	{
		// error
		cout << "Error, data was null" << endl ;
        }
	else
	{
		// process
		cout << " data = " << data << endl ;
	}

	std::cout << " x = " << x << " y = " << y << " comp = " << comp << std::endl ;

	stbi_image_free(data);
}


//! Kernel: Construct the right-hand side of the poisson equation
KERNEL(bnd=1, reduce=+) returns(int cnt=0) returns(double sum=0)
void MakeRhs (FlagGrid& flags, Grid<Real>& rhs, MACGrid& vel, 
			  Grid<Real>* perCellCorr) 
{
	if (!flags.isFluid(i,j,k)) {
		rhs(i,j,k) = 0;
		return;
	}
	   
	// compute divergence 
	// no flag checks: assumes vel at obstacle interfaces is set to zero
	Real set =          vel(i,j,k).x - vel(i+1,j,k).x + 
						vel(i,j,k).y - vel(i,j+1,k).y; 
	if(vel.is3D()) set+=vel(i,j,k).z - vel(i,j,k+1).z;
	
	// per cell divergence correction
	if(perCellCorr) 
		set += perCellCorr->get(i,j,k);
	
	// obtain sum, cell count
	sum += set;
	cnt++;
	
	rhs(i,j,k) = set;
}

//! Kernel: Apply velocity update from poisson equation
KERNEL(bnd=1) 
void CorrectVelocity(FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure) 
{
//	std::cout<<"Entered CorrectVelocity"<<std::endl;
        int idx = flags.index(i,j,k);
	if (flags.isFluid(idx))
	{
		if (flags.isFluid(i-1,j,k)) vel[idx].x -= (pressure[idx] - pressure(i-1,j,k));
		if (flags.isFluid(i,j-1,k)) vel[idx].y -= (pressure[idx] - pressure(i,j-1,k));
		if (flags.is3D() && flags.isFluid(i,j,k-1)) vel[idx].z -= (pressure[idx] - pressure(i,j,k-1));
 
		if (flags.isEmpty(i-1,j,k)) vel[idx].x -= pressure[idx];
		if (flags.isEmpty(i,j-1,k)) vel[idx].y -= pressure[idx];
		if (flags.is3D() && flags.isEmpty(i,j,k-1)) vel[idx].z -= pressure[idx];
	}
	else if (flags.isEmpty(idx))
	{
		if (flags.isFluid(i-1,j,k)) vel[idx].x += pressure(i-1,j,k);
		else                        vel[idx].x  = 0.f;
		if (flags.isFluid(i,j-1,k)) vel[idx].y += pressure(i,j-1,k);
		else                        vel[idx].y  = 0.f;
		if (flags.is3D() ) {
		if (flags.isFluid(i,j,k-1)) vel[idx].z += pressure(i,j,k-1);
		else                        vel[idx].z  = 0.f;
		}
	}
}

//! Kernel: Apply velocity update from poisson equation
KERNEL(bnd=1) 
void CorrectVelocity_target(FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure, Grid<Real>& source_density_convolve) 
{
//	std::cout<<"Entered CorrectVelocity"<<std::endl;
        int idx = flags.index(i,j,k);
	if (flags.isFluid(idx))
	{
		if (flags.isFluid(i-1,j,k)) vel[idx].x -= (pressure[idx] - pressure(i-1,j,k)) * ((source_density_convolve[idx])/(pressure[idx]));
		if (flags.isFluid(i,j-1,k)) vel[idx].y -= (pressure[idx] - pressure(i,j-1,k)) * ((source_density_convolve[idx])/(pressure[idx]));
		if (flags.is3D() && flags.isFluid(i,j,k-1)) vel[idx].z -= (pressure[idx] - pressure(i,j,k-1)) * ((source_density_convolve[idx])/(pressure[idx]));
 
		if (flags.isEmpty(i-1,j,k)) vel[idx].x -= pressure[idx] ;
		if (flags.isEmpty(i,j-1,k)) vel[idx].y -= pressure[idx] ;
		if (flags.is3D() && flags.isEmpty(i,j,k-1)) vel[idx].z -= pressure[idx] ;
	}
	else if (flags.isEmpty(idx))
	{
		if (flags.isFluid(i-1,j,k)) vel[idx].x += pressure(i-1,j,k);
		else                        vel[idx].x  = 0.f;
		if (flags.isFluid(i,j-1,k)) vel[idx].y += pressure(i,j-1,k);
		else                        vel[idx].y  = 0.f;
		if (flags.is3D() ) {
		if (flags.isFluid(i,j,k-1)) vel[idx].z += pressure(i,j,k-1);
		else                        vel[idx].z  = 0.f;
		}
	}
}

//	This is printing the type of each cell in the flags grid if ob/em/fl
KERNEL
void print_celltype( FlagGrid& flags , MACGrid& vel)
{
        int idx = flags.index(i,j,k);
	if (flags.isFluid(idx))
	{
		std::cout << " I am index " << idx << " and I am fluid " << std::endl ;
	}
	else if (flags.isEmpty(idx))
	{
		std::cout << " I am index " << idx << " and I am Empty " << std::endl ;
	}
	else if (flags.isObstacle(idx))
	{
		std::cout << " I am index " << idx << " and I am Obstacle " << std::endl ;
	}
}

// TUSHAR : TEST FUNCTION .
//! Kernel: Apply velocity update from poisson equation with MANIPULATION
KERNEL(bnd=1) 
void CorrectVelocity1(FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure) 
{
//        std::cout << " Entered CorrectVelocity1 " << std::endl ;	
        int idx = flags.index(i,j,k);
	if (flags.isFluid(idx))
	{
		if (flags.isFluid(i-1,j,k)) vel[idx].x -= 1.5*(pressure[idx] - pressure(i-1,j,k));
		if (flags.isFluid(i,j-1,k)) vel[idx].y -= 1.5*(pressure[idx] - pressure(i,j-1,k));
		if (flags.is3D() && flags.isFluid(i,j,k-1)) vel[idx].z -= 1.5*(pressure[idx] - pressure(i,j,k-1));
 
		if (flags.isEmpty(i-1,j,k)) vel[idx].x -= 1.5*pressure[idx];
		if (flags.isEmpty(i,j-1,k)) vel[idx].y -= 1.5*pressure[idx];
		if (flags.is3D() && flags.isEmpty(i,j,k-1)) vel[idx].z -= 1.5*pressure[idx];
	}
	else if (flags.isEmpty(idx))
	{
		if (flags.isFluid(i-1,j,k)) vel[idx].x += 1.5*pressure(i-1,j,k);
		else                        vel[idx].x  = 0.f;
		if (flags.isFluid(i,j-1,k)) vel[idx].y += 1.5*pressure(i,j-1,k);
		else                        vel[idx].y  = 0.f;
		if (flags.is3D() ) {
		if (flags.isFluid(i,j,k-1)) vel[idx].z += 1.5*pressure(i,j,k-1);
		else                        vel[idx].z  = 0.f;
		}
	}
}

// ALERT : THIS IS FAKE !!!!
//! Kernel: Apply velocity update from poisson equation with MANIPULATION
KERNEL(bnd=1) 
void CorrectVelocity2(FlagGrid& flags, MACGrid& vel, Grid<Real>& pressure) 
{
//        std::cout << " Entered CorrectVelocity2 " << std::endl ;	
        int idx = flags.index(i,j,k);
	if (flags.isFluid(idx))
	{
		if (flags.isFluid(i-1,j,k)) vel[idx].x -= 0.5*(pressure[idx] - pressure(i-1,j,k));
		if (flags.isFluid(i,j-1,k)) vel[idx].y -= 0.5*(pressure[idx] - pressure(i,j-1,k));
		if (flags.is3D() && flags.isFluid(i,j,k-1)) vel[idx].z -= 0.5*(pressure[idx] - pressure(i,j,k-1));
 
		if (flags.isEmpty(i-1,j,k)) vel[idx].x -= 0.5*pressure[idx];
		if (flags.isEmpty(i,j-1,k)) vel[idx].y -= 0.5*pressure[idx];
		if (flags.is3D() && flags.isEmpty(i,j,k-1)) vel[idx].z -= 0.5*pressure[idx];
	}
	else if (flags.isEmpty(idx))
	{
		if (flags.isFluid(i-1,j,k)) vel[idx].x += 0.5*pressure(i-1,j,k);
		else                        vel[idx].x  = 0.f;
		if (flags.isFluid(i,j-1,k)) vel[idx].y += 0.5*pressure(i,j-1,k);
		else                        vel[idx].y  = 0.f;
		if (flags.is3D() ) {
		if (flags.isFluid(i,j,k-1)) vel[idx].z += 0.5*pressure(i,j,k-1);
		else                        vel[idx].z  = 0.f;
		}
	}
}

        
KERNEL void SetOpenBound(Grid<Real> &A0,Grid<Real> &Ai,Grid<Real> &Aj,Grid<Real> &Ak,FlagGrid& flags,MACGrid& vel, 
							Vector3D<bool> lo, Vector3D<bool> up) 
{

	if (!flags.isFluid(i,j,k))
		return;
	
	int b = flags.getBoundaryWidth();

	// set matrix stencil in and at boundary to empty
	if((lo.x && i <= b+1)||(up.x && i >= maxX-b-2)||(lo.y && j <= b+1)||(up.y && j >= maxY-b-2))
		{
//			std::cout << " In SetOpenBound 1 " << std::endl;
			A0(i,j,k) = (flags.is3D()) ? 6. : 4.;
//			std::cout<<"A0("<<i<<","<<j<<","<<k<<")="<<A0(i,j,k)<<std::endl;
		}

	if ((lo.x && i <= b)||(up.x && i >= maxX-b-2))					 
	{	
//		std::cout << " In SetOpenBound 2 " << std::endl;
		Ai(i,j,k) = .0;
//		std::cout<<"Ai("<<i<<","<<j<<","<<k<<")="<<Ai(i,j,k)<<std::endl;
	}

	if ((lo.y && j <= b)||(up.y && j >= maxY-b-2))					 
	{
//		std::cout << " In SetOpenBound 3 " << std::endl;
		Aj(i,j,k) = .0;
//		std::cout<<"Aj("<<i<<","<<j<<","<<k<<")="<<Aj(i,j,k)<<std::endl;
	}

	if (flags.is3D() && ((lo.z && k <= b)||(up.z && k >= maxZ-b-2))) Ak(i,j,k) = .0;

	// set velocity boundary conditions
	if (lo.x && i == b)				vel(b,j,k) = vel(b+1,j,k);
	if (lo.y && j == b)				vel(i,b,k) = vel(i,b+1,k);
	if (up.x && i == maxX-b-1)		vel(maxX-b-1,j,k) = vel(maxX-b-2,j,k);
	if (up.y && j == maxY-b-1)		vel(i,maxY-b-1,k) = vel(i,maxY-b-2,k);
	if(flags.is3D()) {
		if (lo.z && k == b)			vel(i,j,b) = vel(i,j,b+1);
		if (up.z && k == maxZ-b-1)	vel(i,j,maxZ-b-1) = vel(i,j,maxZ-b-2); 
	}
}


//! Kernel: Set matrix rhs for outflow
KERNEL void SetOutflow (Grid<Real>& rhs, Vector3D<bool> lowerBound, Vector3D<bool> upperBound, int height)
{
	if ((lowerBound.x && i < height) || (upperBound.x && i >= maxX-1-height) ||
		(lowerBound.y && j < height) || (upperBound.y && j >= maxY-1-height) ||
		(lowerBound.z && k < height) || (upperBound.z && k >= maxZ-1-height))
		rhs(i,j,k) = 0;
}


// *****************************************************************************
// Ghost fluid helpers

// calculate fraction filled with liquid (note, assumes inside value is < outside!)
inline static Real thetaHelper(Real inside, Real outside)
{
	Real denom = inside-outside;
	if (denom > -1e-04) return 0.5; // should always be neg, and large enough...
	return std::max(Real(0), std::min(Real(1), inside/denom));
}

// calculate ghost fluid factor, cell at idx should be a fluid cell
inline static Real ghostFluidHelper(int idx, int offset, const Grid<Real> &phi, Real gfClamp)
{
	Real alpha = thetaHelper(phi[idx], phi[idx+offset]);
	if (alpha < gfClamp) return alpha = gfClamp;
	return (1-(1/alpha)); 
}

//! Kernel: Adapt A0 for ghost fluid
KERNEL(bnd=1) 
void ApplyGhostFluidDiagonal(Grid<Real> &A0, const FlagGrid &flags, const Grid<Real> &phi, Real gfClamp)
{
	const int X = flags.getStrideX(), Y = flags.getStrideY(), Z = flags.getStrideZ();
	int idx = flags.index(i,j,k);
	if (!flags.isFluid(idx)) return;

	if (flags.isEmpty(i-1,j,k)) A0[idx] -= ghostFluidHelper(idx, -X, phi, gfClamp);
	if (flags.isEmpty(i+1,j,k)) A0[idx] -= ghostFluidHelper(idx, +X, phi, gfClamp);
	if (flags.isEmpty(i,j-1,k)) A0[idx] -= ghostFluidHelper(idx, -Y, phi, gfClamp);
	if (flags.isEmpty(i,j+1,k)) A0[idx] -= ghostFluidHelper(idx, +Y, phi, gfClamp);
	if (flags.is3D()) {
		if (flags.isEmpty(i,j,k-1)) A0[idx] -= ghostFluidHelper(idx, -Z, phi, gfClamp);
		if (flags.isEmpty(i,j,k+1)) A0[idx] -= ghostFluidHelper(idx, +Z, phi, gfClamp);
	}
}

//! Kernel: Apply velocity update: ghost fluid contribution
KERNEL(bnd=1)
void CorrectVelocityGhostFluid(MACGrid &vel, const FlagGrid &flags, const Grid<Real> &pressure, const Grid<Real> &phi, Real gfClamp)
{
	const int X = flags.getStrideX(), Y = flags.getStrideY(), Z = flags.getStrideZ();
	const int idx = flags.index(i,j,k);
	if (flags.isFluid(idx))
	{
		if (flags.isEmpty(i-1,j,k)) vel[idx][0] += pressure[idx] * ghostFluidHelper(idx, -X, phi, gfClamp);
		if (flags.isEmpty(i,j-1,k)) vel[idx][1] += pressure[idx] * ghostFluidHelper(idx, -Y, phi, gfClamp);
		if (flags.is3D() && flags.isEmpty(i,j,k-1)) vel[idx][2] += pressure[idx] * ghostFluidHelper(idx, -Z, phi, gfClamp);
	}
	else if (flags.isEmpty(idx))
	{
		if (flags.isFluid(i-1,j,k)) vel[idx][0] -= pressure(i-1,j,k) * ghostFluidHelper(idx-X, +X, phi, gfClamp);
		else                        vel[idx].x  = 0.f;
		if (flags.isFluid(i,j-1,k)) vel[idx][1] -= pressure(i,j-1,k) * ghostFluidHelper(idx-Y, +Y, phi, gfClamp);
		else                        vel[idx].y  = 0.f;
		if (flags.is3D() ) {
		if (flags.isFluid(i,j,k-1)) vel[idx][2] -= pressure(i,j,k-1) * ghostFluidHelper(idx-Z, +Z, phi, gfClamp);
		else                        vel[idx].z  = 0.f;
		}
	}
}


// improve behavior of clamping for large time steps:

inline static Real ghostFluidWasClamped(int idx, int offset, const Grid<Real> &phi, Real gfClamp)
{
	Real alpha = thetaHelper(phi[idx], phi[idx+offset]);
	if (alpha < gfClamp) return true;
	return false;
}

KERNEL(bnd=1)
void ReplaceClampedGhostFluidVels(MACGrid &vel, FlagGrid &flags, 
		const Grid<Real> &pressure, const Grid<Real> &phi, Real gfClamp )
{
	const int X = flags.getStrideX(), Y = flags.getStrideY(), Z = flags.getStrideZ();
	const int idx = flags.index(i,j,k);
	if (flags.isFluid(idx))
	{
		if( (flags.isEmpty(i-1,j,k)) && (ghostFluidWasClamped(idx, -X, phi, gfClamp)) )
			vel[idx-X][0] = vel[idx][0];
		if( (flags.isEmpty(i,j-1,k)) && (ghostFluidWasClamped(idx, -Y, phi, gfClamp)) )
			vel[idx-Y][1] = vel[idx][1];
		if( flags.is3D() && 
		   (flags.isEmpty(i,j,k-1)) && (ghostFluidWasClamped(idx, -Z, phi, gfClamp)) )
			vel[idx-Z][2] = vel[idx][2];
	}
	else if (flags.isEmpty(idx))
	{
		if( (i>-1) && (flags.isFluid(i-1,j,k)) && ( ghostFluidWasClamped(idx-X, +X, phi, gfClamp) ) )
			vel[idx][0] = vel[idx-X][0];
		if( (j>-1) && (flags.isFluid(i,j-1,k)) && ( ghostFluidWasClamped(idx-Y, +Y, phi, gfClamp) ) )
			vel[idx][1] = vel[idx-Y][1];
		if( flags.is3D() &&
		  ( (k>-1) && (flags.isFluid(i,j,k-1)) && ( ghostFluidWasClamped(idx-Z, +Z, phi, gfClamp) ) ))
			vel[idx][2] = vel[idx-Z][2];
	}
}


// *****************************************************************************
// Main pressure solve

inline void convertDescToVec(const string& desc, Vector3D<bool>& lo, Vector3D<bool>& up) {
	for(size_t i=0; i<desc.size(); i++) {
		if (desc[i] == 'x') lo.x = true;
		else if (desc[i] == 'y') lo.y = true;
		else if (desc[i] == 'z') lo.z = true;
		else if (desc[i] == 'X') {up.x = true;}
		else if (desc[i] == 'Y') {up.y = true;}
		else if (desc[i] == 'Z') up.z = true;
		else errMsg("invalid character in boundary description string. Only [xyzXYZ] allowed.");
	}
}

//        This is normal Gaussian
PYTHON void create_pressure_modifier_still_frame(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center = Vec3(40,32,0.5) , double sigma = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     double r , s = 2.0 * sigma * sigma ;

     // sum is for normalization
     double sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);

     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r = sqrt(x*x + y*y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp(-(r*r)/s))/(M_PI * s);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= central_value;
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is normal Gaussian
PYTHON void create_pressure_modifier_inflow(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center = Vec3(40,32,0.5) , double sigma = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     double r , s = 2.0 * sigma * sigma ;

     // sum is for normalization
     double sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);

     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r = sqrt(x*x + y*y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp(-(r*r)/s))/(M_PI * s);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= central_value;
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is normal Gaussian
PYTHON void create_pressure_modifier_gaussian(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center = Vec3(40,32,0.5) , double sigma = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     double r , s = 2.0 * sigma * sigma ;

     // sum is for normalization
     double sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);

     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r = sqrt(x*x + y*y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp(-(r*r)/s))/(M_PI * s);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= central_value;
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//	This is circular Mexican Hat
PYTHON void create_pressure_modifier_mexican_hat(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center = Vec3(40,32,0.5) , double sigma = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     Real constant_term = 2.0/(sqrt(3.0*sigma) * pow(M_PI , 0.25)) ;
     Real term_1 , term_2 , term_3;

     // sum is for normalization
     Real sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);

     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             term_1 = ((Real)(x*x + y*y))/(sigma*sigma);
             term_2 = 1.0 - term_1;
             term_3 = pow(M_E , -(term_1)/2.0);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = constant_term * term_2 * term_3;
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= central_value;
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is normal Gaussian
PYTHON void create_pressure_modifier_advection(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center = Vec3(40,32,0.5) , double sigma = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     double r , s = 2.0 * sigma * sigma ;
 
     // sum is for normalization
     double sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);
    
     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r = sqrt(x*x + y*y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp(-(r*r)/s))/(M_PI * s);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= central_value;
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is inverted stairs
PYTHON void create_pressure_modifier_spiral(Grid<Real>& pressure_modification , Vec3 origin = Vec3(38,31,0.5) )
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(origin)).x;
     idx_y = (toVec3i(origin)).y;
     idx_z = (toVec3i(origin)).z;

     int temp_idx_x ;
     int temp_idx_y ;

     int old_x ;
     int old_y ;

     temp_idx_x = idx_x ;
     temp_idx_y = idx_y ;
     old_x = idx_x ;
     old_y = idx_y ;

     idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
     pressure_modification[idx] = 1.0 ;


     int i,j,k,factor=1 ;
     for(i=1;i<=5;i++)
     {
     factor = 1 ;
     for (k=1;k<=i;k++)
     {
         factor *= -1 ;
     }

// left or right
     for (j=1;j<=3*i;j++)
     {
          temp_idx_x = old_x + factor*j ;
          temp_idx_y = old_y ;
          idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
          pressure_modification[idx] = 1.0 ;
     }
     old_x = temp_idx_x ;

// up or down
     for (j=1;j<=3*i;j++)
     {
          temp_idx_x = old_x ;
          temp_idx_y = old_y + (factor * (-1) * j) ;
          idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
          pressure_modification[idx] = 1.0 ;
     }
     old_y = temp_idx_y ;

     }
}

//        This is inverted stairs
PYTHON void create_pressure_modifier_smiling_face(Grid<Real>& pressure_modification , Vec3 origin = Vec3(38,31,0.5) )
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(origin)).x;
     idx_y = (toVec3i(origin)).y;
     idx_z = (toVec3i(origin)).z;

     int temp_idx_x ;
     int temp_idx_y ;

// nose
     temp_idx_x = idx_x ;
     temp_idx_y = idx_y ;
     idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
     pressure_modification[idx] = 1.0 ;

// eyes
     temp_idx_x = idx_x - 2 ;
     temp_idx_y = idx_y + 2 ;
     idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
     pressure_modification[idx] = 1.0 ;
     temp_idx_x = idx_x + 2 ;
     temp_idx_y = idx_y + 2 ;
     idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
     pressure_modification[idx] = 1.0 ;

// lips
     temp_idx_x = idx_x - 3 ;
     temp_idx_y = idx_y - 3 ;
     idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
     pressure_modification[idx] = 1.0 ;
     temp_idx_x = idx_x + 3 ;
     temp_idx_y = idx_y - 3 ;
     idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
     pressure_modification[idx] = 1.0 ;
     int j ;
     for(j=-3;j<=3;j++)
     {
          int x = j ;
          int y = -4 ;
          int temp_idx_x = idx_x + x ;
          int temp_idx_y = idx_y + y ;
          idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
          pressure_modification[idx] = 1.0 ;
     }
}

//        This is inverted stairs
PYTHON void create_pressure_modifier_inverted_stairs(Grid<Real>& pressure_modification , Vec3 origin = Vec3(38,31,0.5) , int depth=4)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(origin)).x;
     idx_y = (toVec3i(origin)).y;
     idx_z = (toVec3i(origin)).z;

    int i,j ;
// i is for the name of the layer 0,1 ,2 ,3 , 4 etc.
    for(i=0;i<depth;i++)
    {
// i+1 is also the number of elements in ith layer
        for(j=0;j<=i;j++)
        {
            int x = j ;
            int y = i ;
            int temp_idx_x = idx_x + x ;
            int temp_idx_y = idx_y + y ;
            idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
            
            pressure_modification[idx] = (i+j)*(i+j) ;
        }
    }
}

//        This is inverted Pascal
PYTHON void create_pressure_modifier_inverted_pascal(Grid<Real>& pressure_modification , Vec3 origin = Vec3(38,31,0.5) , int depth=4)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(origin)).x;
     idx_y = (toVec3i(origin)).y;
     idx_z = (toVec3i(origin)).z;

    int coef=1,i,j ;
// i is for the name of the layer 0,1 ,2 ,3 , 4 etc.
    for(i=0;i<depth;i++)
    {
// i+1 is also the number of elements in ith layer
        for(j=0;j<=i;j++)
        {
            int x = - ( i - 2 * j ) ;
            int y = i ;
            int temp_idx_x = idx_x + x ;
            int temp_idx_y = idx_y + y ;
            idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
            
            if (j==0||i==0)
            {
                coef = 1 ;
                pressure_modification[idx]=coef;
            }
            else
            {
                coef = coef*(i-j+1)/j ;
                pressure_modification[idx]=coef;
            }
        }
    }
}

//        This is elliptical Gaussian
PYTHON void create_pressure_modifier_elliptical_gaussian(Grid<Real>& pressure_modification , int radius_x=3 , int radius_y=3 , Vec3 splat_center = Vec3(40,32,0.5) , double sigma_x = 1.0 , double sigma_y = 2.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     double r_x , r_y ;
     double denominator = 2.0 * M_PI * sigma_x * sigma_y ;
 
     // sum is for normalization
     double sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);
    
     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius_x); x <= (radius_x); x++)
     {
         for (int y = -(radius_y); y <= (radius_y); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r_x = (x*x)/(2.0*sigma_x*sigma_x);
             r_y = (y*y)/(2.0*sigma_y*sigma_y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp((-r_x)+(-r_y)))/(denominator);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= (central_value);
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is elliptical Gaussian
PYTHON void create_pressure_modifier_elliptical_gaussian_with_multiplier(Grid<Real>& pressure_modification , int radius_x=3 , int radius_y=3 , Vec3 splat_center = Vec3(40,32,0.5) , Real sigma_x = 1.0 , Real sigma_y = 2.0, Real multiplier = 4.0,Real lower_factor = 1.0, Real higher_factor = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     Real r_x , r_y ;
     Real denominator = 2.0 * M_PI * sigma_x * sigma_y ;
 
     // sum is for normalization
     Real sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);
    
     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius_x); x <= (radius_x); x++)
     {
         for (int y = -(radius_y); y <= (radius_y); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r_x = (x*x)/(2.0*sigma_x*sigma_x);
             r_y = (y*y)/(2.0*sigma_y*sigma_y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp((-r_x)+(-r_y)))/(denominator);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= (central_value);
              pressure_modification[idx] *= (multiplier);
              Real j_temp = j + radius_y ;
              Real m = (higher_factor - lower_factor)/(2.0 * radius_y) ;
              Real c = lower_factor ;
              pressure_modification[idx] = (m * (j_temp) + c) * pressure_modification[idx] ;
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}


//	This is elliptical Gaussian
PYTHON void create_pressure_modifier_elliptical_gaussian_with_multiplier_3D(Grid<Real>& pressure_modification, int radius_x=3, int radius_y=3, int radius_z=3, Vec3 splat_center = Vec3(40,32,0.5), Real a = 1.0, Real b = 2.0, Real c = 1.0, Real m = 1.0, Real n = 0.0, Real p = 1.0, Real multiplier = 4.0, Real lower_factor = 1.0, Real higher_factor = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     Real r1_x, r1_y, r1_z, r2_x, r2_y, r2_z ;
//     Real denominator = 2.0 * M_PI * sigma_x * sigma_y ;
 
     // sum is for normalization
     Real sum = 0.0;
     idx = pressure_modification.index(idx_x, idx_y, idx_z);
    
     int temp_idx_x, temp_idx_y, temp_idx_z ;

//   generate 5x5 kernel
     for (int x = -(radius_x); x <= (radius_x); x++)
     {
         for (int y = -(radius_y); y <= (radius_y); y++)
         {
             for (int z = -(radius_z); z <= (radius_z); z++)
             {
                 temp_idx_x = idx_x + x ;
                 temp_idx_y = idx_y + y ;
                 temp_idx_z = idx_z + z ;
                 
                 r1_x = (x*x)*a;
                 r1_y = (y*y)*b;
                 r1_z = (z*z)*c;
                 
                 r2_x = 2.0 * m * x * y ; 
                 r2_y = 2.0 * n * x * z ;
                 r2_z = 2.0 * p * y * z ;
                 
                 idx = pressure_modification.index(temp_idx_x , temp_idx_y , temp_idx_z);
                 pressure_modification[idx] = (exp((-r1_x)+(-r1_y)+(-r1_z)+(-r2_x)+(-r2_y)+(-r2_z)));
                 sum += pressure_modification[idx];
             }
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              for(int k = -(radius_z); k <= (radius_z); ++k)
              {
                  temp_idx_x = idx_x + i ;
                  temp_idx_y = idx_y + j ;
                  temp_idx_z = idx_z + k ;
                  idx = pressure_modification.index(temp_idx_x , temp_idx_y , temp_idx_z);
                  pressure_modification[idx] /= sum;
//                pressure_modification[idx] *= 20.0;
              }
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              for(int k = -(radius_z); k <= (radius_z); ++k)
              {
                  temp_idx_x = idx_x + i ;
                  temp_idx_y = idx_y + j ;
                  temp_idx_z = idx_z + k ;

                  idx = pressure_modification.index(temp_idx_x , temp_idx_y , temp_idx_z);
                  
                  pressure_modification[idx] /= (central_value);
                  pressure_modification[idx] *= (multiplier);
                  
//                  Real j_temp = j + radius_y ;
//                  Real m = (higher_factor - lower_factor)/(2.0 * radius_y) ;
//                  Real c = lower_factor ;
//                  pressure_modification[idx] = (m * (j_temp) + c) * pressure_modification[idx] ;
              }
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is elliptical Gaussian
PYTHON void create_pressure_modifier_elliptical_gaussian_with_multiplier_and_quadratic_gradient(Grid<Real>& pressure_modification , int radius_x=3 , int radius_y=3 , Vec3 splat_center = Vec3(40,32,0.5) , Real sigma_x = 1.0 , Real sigma_y = 2.0, Real multiplier = 4.0,Real lower_factor = 0.0, Real higher_factor = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     Real r_x , r_y ;
     Real denominator = 2.0 * M_PI * sigma_x * sigma_y ;
 
     // sum is for normalization
     Real sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);
    
     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius_x); x <= (radius_x); x++)
     {
         for (int y = -(radius_y); y <= (radius_y); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r_x = (x*x)/(2.0*sigma_x*sigma_x);
             r_y = (y*y)/(2.0*sigma_y*sigma_y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp((-r_x)+(-r_y)))/(denominator);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= (central_value);
              pressure_modification[idx] *= (multiplier);
              Real j_temp = j + radius_y ;
              Real m = (higher_factor - lower_factor)/(2.0 * radius_y) ;
              Real c = lower_factor ;
              Real value = (m * (j_temp) + c);
              Real fraction = (value - lower_factor)/(higher_factor - lower_factor) ;
              fraction = fraction * fraction ;
              pressure_modification[idx] = (fraction * higher_factor) * pressure_modification[idx] ;
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is elliptical Gaussian
PYTHON void create_pressure_modifier_elliptical_gaussian_with_multiplier_and_cubic_gradient(Grid<Real>& pressure_modification , int radius_x=3 , int radius_y=3 , Vec3 splat_center = Vec3(40,32,0.5) , Real sigma_x = 1.0 , Real sigma_y = 2.0, Real multiplier = 4.0,Real lower_factor = 0.0, Real higher_factor = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     Real r_x , r_y ;
     Real denominator = 2.0 * M_PI * sigma_x * sigma_y ;
 
     // sum is for normalization
     Real sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);
    
     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius_x); x <= (radius_x); x++)
     {
         for (int y = -(radius_y); y <= (radius_y); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r_x = (x*x)/(2.0*sigma_x*sigma_x);
             r_y = (y*y)/(2.0*sigma_y*sigma_y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp((-r_x)+(-r_y)))/(denominator);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= (central_value);
              pressure_modification[idx] *= (multiplier);
              Real j_temp = j + radius_y ;
              Real m = (higher_factor - lower_factor)/(2.0 * radius_y) ;
              Real c = lower_factor ;
              Real value = (m * (j_temp) + c);
              Real fraction = (value - lower_factor)/(higher_factor - lower_factor) ;
              fraction = (fraction * fraction * fraction);
              pressure_modification[idx] = (fraction * higher_factor) * pressure_modification[idx] ;
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//PYTHON void create_pressure_modifier(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center , double sigma = 1.0)
//PYTHON void create_pressure_modifier(Grid<Real>& pressure_modification , int radius=3 , Vec3& splat_center , double sigma = 1.0)
//PYTHON void create_pressure_modifier(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center(40,32,0.5) , double sigma = 1.0)
//PYTHON void create_pressure_modifier(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center(40.0,32.0,0.5) , double sigma = 1.0)
//PYTHON void create_pressure_modifier(Grid<Real>& pressure_modification , int radius=3 , Vec3& splat_center(40.0,32.0,0.5) , double sigma = 1.0)

//        This is normal Gaussian
PYTHON void create_pressure_modifier_5(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center = Vec3(40,32,0.5) , double sigma = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     double r , s = 2.0 * sigma * sigma ;
 
     // sum is for normalization
     double sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);
    
     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r = sqrt(x*x + y*y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp(-(r*r)/s))/(M_PI * s);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= central_value;
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is Gaussian with alternate positive and negative
PYTHON void create_pressure_modifier_10(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center = Vec3(40,32,0.5) , double sigma = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     double r , s = 2.0 * sigma * sigma ;
 
     // sum is for normalization
     double sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);
    
     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r = sqrt(x*x + y*y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp(-(r*r)/s))/(M_PI * s);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values ; alternate signs 
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= central_value;
              if ((i%2 != 0)||(j%2 != 0))
              {
                   pressure_modification[idx] *= (-1.0);
              }
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is Gaussian with alternate positive and zero
PYTHON void create_pressure_modifier_11(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center = Vec3(40,32,0.5) , double sigma = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     double r , s = 2.0 * sigma * sigma ;
 
     // sum is for normalization
     double sum = 0.0;
     idx = pressure_modification.index(idx_x , idx_y , idx_z);
    
     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r = sqrt(x*x + y*y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp(-(r*r)/s))/(M_PI * s);
             sum += pressure_modification[idx];
         }
     }
 
//   normalize the Kernel
     for(int i = -(radius ); i <= (radius); ++i)
     {
         for(int j = -(radius ); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= sum;
//             pressure_modification[idx] *= 20;
         }
     }

     idx = pressure_modification.index(idx_x , idx_y , idx_z);
     Real central_value = pressure_modification[idx] ;

//   Make the central value 1 and accordingly adjust other values ; alternate signs 
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= central_value;
              if ((i%2 != 0)||(j%2 != 0))
              {
                   pressure_modification[idx] *= (0.0);
              }
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is a constant alternating function 1 and -1 , with radius as Gaussian
PYTHON void create_pressure_modifier_13(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center = Vec3(40,32,0.5) , double sigma = 1.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = 1.0;
         }
     }

//   Make the central value 1 and accordingly adjust other values ; alternate signs 
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              if ((i%2 != 0)||(j%2 != 0))
              {
                   pressure_modification[idx] *= (-1.0);
              }
         }
     }

//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}


PYTHON void modify_pressure_constant(Grid<Real>& pressure,Grid<Real>& pressure_modification,Real multiplier = 4.0)
{
//   pressure = pressure + pressure_modification*pressure;
//   gridMultScalar<Real,Real>(pressure_modification,10.0);

//   gridMult<Real,Real>(pressure_modification,pressure);
//   gridAdd<Real,Real>(pressure,pressure_modification);

     std::cout<<"minimum value of pressure = " << pressure.getMinValue()<<std::endl;
     std::cout<<"maximum value of pressure = " << pressure.getMaxValue()<<std::endl;
     gridMultScalar<Real,Real>(pressure_modification , multiplier);
     gridAdd<Real,Real>(pressure , pressure_modification);

//   KERNEL(idx) template<class T, class S> void gridAdd  (Grid<T>& me, const Grid<S>& other) { me[idx] += other[idx]; }
//   KERNEL(idx) template<class T, class S> void gridSub  (Grid<T>& me, const Grid<S>& other) { me[idx] -= other[idx]; }
//   KERNEL(idx) template<class T, class S> void gridMult (Grid<T>& me, const Grid<S>& other) { me[idx] *= other[idx]; }
//   KERNEL(idx) template<class T, class S> void gridDiv  (Grid<T>& me, const Grid<S>& other) { me[idx] /= other[idx]; }
//   KERNEL(idx) template<class T, class S> void gridAddScalar (Grid<T>& me, const S& other)  { me[idx] += other; }
//   KERNEL(idx) template<class T, class S> void gridMultScalar(Grid<T>& me, const S& other)  { me[idx] *= other; }
//   KERNEL(idx) template<class T, class S> void gridScaledAdd (Grid<T>& me, const Grid<T>& other, const S& factor) { me[idx] += factor * other[idx]; }     
     std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp modify_pressure_1"<<std::endl;
}

PYTHON void modify_pressure_variable(Grid<Real>& pressure,Grid<Real>& pressure_modification,int multiplier = 25)
{
     std::cout<<"minimum value of pressure = " << pressure.getMinValue()<<std::endl;
     std::cout<<"maximum value of pressure = " << pressure.getMaxValue()<<std::endl;
     gridMultScalar<Real,Real>(pressure_modification , multiplier);
     gridMult<Real,Real>(pressure_modification , pressure);
     gridAdd<Real,Real>(pressure , pressure_modification);

     std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp modify_pressure_2"<<std::endl;
}

//! Perform pressure projection of the velocity grid
PYTHON void solvePressure(MACGrid& vel, Grid<Real>& pressure, FlagGrid& flags, string openBound="",
                     Grid<Real>* phi = 0, 
                     Grid<Real>* perCellCorr = 0, 
                     Real gfClamp = 1e-04,
                     Real cgMaxIterFac = 1.5,
                     Real cgAccuracy = 1e-10,
                     string outflow = "",
                     int outflowHeight = 1,
                     bool precondition = true,
                     bool enforceCompatibility = false,
                     bool useResNorm = true )
{
        // parse strings
	Vector3D<bool> loOpenBound, upOpenBound, loOutflow, upOutflow;
	convertDescToVec(openBound, loOpenBound, upOpenBound);
	convertDescToVec(outflow, loOutflow, upOutflow);
	if (vel.is2D() && (loOpenBound.z || upOpenBound.z))
		errMsg("open boundaries for z specified for 2D grid");

	// reserve temp grids
	FluidSolver* parent = flags.getParent();
	Grid<Real> rhs(parent);
	Grid<Real> residual(parent);
	Grid<Real> search(parent);
	Grid<Real> A0(parent);
	Grid<Real> Ai(parent);
	Grid<Real> Aj(parent);
	Grid<Real> Ak(parent);
	Grid<Real> tmp(parent);
	Grid<Real> pca0(parent);
	Grid<Real> pca1(parent);
	Grid<Real> pca2(parent);
	Grid<Real> pca3(parent);
		
	// setup matrix and boundaries
	MakeLaplaceMatrix (flags, A0, Ai, Aj, Ak);
	SetOpenBound (A0, Ai, Aj, Ak, flags, vel, loOpenBound, upOpenBound);
	
	if (phi) {
	        std::cout<<"Applying GhostFluidDiagonal"<<std::endl;
        	ApplyGhostFluidDiagonal(A0, flags, *phi, gfClamp);
	}
	
	// compute divergence and init right hand side
	MakeRhs kernMakeRhs (flags, rhs, vel, perCellCorr);
	
	if (!outflow.empty())
		SetOutflow (rhs, loOutflow, upOutflow, outflowHeight);
	
	if (enforceCompatibility)
		rhs += (Real)(-kernMakeRhs.sum / (Real)kernMakeRhs.cnt);
	
	// CG setup
	// note: the last factor increases the max iterations for 2d, which right now can't use a preconditioner 
	const int maxIter = (int)(cgMaxIterFac * flags.getSize().max()) * (flags.is3D() ? 1 : 4);
	GridCgInterface *gcg;
	if (vel.is3D())
		gcg = new GridCg<ApplyMatrix>  (pressure, rhs, residual, search, flags, tmp, &A0, &Ai, &Aj, &Ak );
	else
		gcg = new GridCg<ApplyMatrix2D>(pressure, rhs, residual, search, flags, tmp, &A0, &Ai, &Aj, &Ak );
	
	gcg->setAccuracy( cgAccuracy ); 
	gcg->setUseResNorm( useResNorm );

	// optional preconditioning
	gcg->setPreconditioner( precondition ? GridCgInterface::PC_mICP : GridCgInterface::PC_None, &pca0, &pca1, &pca2, &pca3);

	for (int iter=0; iter<maxIter; iter++) {
		if (!gcg->iterate()) {iter=maxIter;}
	} 
	std::cout<<"maxIter = " << maxIter << std::endl ;
	debMsg("FluidSolver::solvePressure iterations:"<<gcg->getIterations()<<", res:"<<gcg->getSigma(), 1);
	delete gcg;
	
	CorrectVelocity(flags, vel, pressure ); 
        if (phi) {
//                std::cout<<"Correcting VelocityGhostFluid and Replacing ClampedGhostFluidVels"<<std::endl;
		CorrectVelocityGhostFluid (vel, flags, pressure, *phi, gfClamp);
		// improve behavior of clamping for large time steps:
		ReplaceClampedGhostFluidVels (vel, flags, pressure, *phi, gfClamp);
	}
}

//! Perform pressure projection of the velocity grid
PYTHON void solvePressure_modifiedCG_part1(MACGrid& vel, Grid<Real>& pressure, FlagGrid& flags, string openBound="",
                     Grid<Real>* phi = 0, 
                     Grid<Real>* perCellCorr = 0, 
                     Real gfClamp = 1e-04,
                     Real cgMaxIterFac = 1.5,
                     Real cgAccuracy = 1e-10,
                     string outflow = "",
                     int outflowHeight = 1,
                     bool precondition = true,
                     bool enforceCompatibility = false,
                     bool useResNorm = true )
{
        // parse strings
	Vector3D<bool> loOpenBound, upOpenBound, loOutflow, upOutflow;
	convertDescToVec(openBound, loOpenBound, upOpenBound);
	convertDescToVec(outflow, loOutflow, upOutflow);
	if (vel.is2D() && (loOpenBound.z || upOpenBound.z))
		errMsg("open boundaries for z specified for 2D grid");
	
	// reserve temp grids
	FluidSolver* parent = flags.getParent();
	Grid<Real> rhs(parent);
	Grid<Real> residual(parent);

//	Vec3 splat_center = Vec3(31,31,0.5) ; int splat_radius = 3 ;

//	modify_residual_to_target(residual , splat_center , splat_radius);
//	std::cout << "Let us start printing" << std::endl ;
//	residual.printGrid(0);

//	modify_pressure_to_target(pressure , splat_center , splat_radius);
//	std::cout << "Let us start printing again !!" << std::endl ;
//	pressure.printGrid(0);

	Grid<Real> search(parent);
	Grid<Real> A0(parent);
	Grid<Real> Ai(parent);
	Grid<Real> Aj(parent);
	Grid<Real> Ak(parent);
	Grid<Real> tmp(parent);
	Grid<Real> pca0(parent);
	Grid<Real> pca1(parent);
	Grid<Real> pca2(parent);
	Grid<Real> pca3(parent);

	// setup matrix and boundaries
	MakeLaplaceMatrix (flags, A0, Ai, Aj, Ak);
	SetOpenBound (A0, Ai, Aj, Ak, flags, vel, loOpenBound, upOpenBound);
	
	if (phi) {
	        std::cout<<"Applying GhostFluidDiagonal"<<std::endl;
        	ApplyGhostFluidDiagonal(A0, flags, *phi, gfClamp);
	}
	
	// compute divergence and init right hand side
	MakeRhs kernMakeRhs (flags, rhs, vel, perCellCorr);
	
	if (!outflow.empty())
		SetOutflow (rhs, loOutflow, upOutflow, outflowHeight);
	
	if (enforceCompatibility)
		rhs += (Real)(-kernMakeRhs.sum / (Real)kernMakeRhs.cnt);
	
	// CG setup
	// note: the last factor increases the max iterations for 2d, which right now can't use a preconditioner 
	const int maxIter = (int)(cgMaxIterFac * flags.getSize().max()) * (flags.is3D() ? 1 : 4);
//	const int maxIter = 4500;

	GridCgInterface *gcg;
	if (vel.is3D())
		gcg = new GridCg<ApplyMatrix>  (pressure, rhs, residual, search, flags, tmp, &A0, &Ai, &Aj, &Ak );
	else
		gcg = new GridCg<ApplyMatrix2D>(pressure, rhs, residual, search, flags, tmp, &A0, &Ai, &Aj, &Ak );
	
	gcg->setAccuracy( cgAccuracy ); 
	gcg->setUseResNorm( useResNorm );

	// optional preconditioning
	gcg->setPreconditioner( precondition ? GridCgInterface::PC_mICP : GridCgInterface::PC_None, &pca0, &pca1, &pca2, &pca3);

	for (int iter=0; iter<maxIter; iter++) {
			if (!gcg->iterate()) {
				std::cout << "It returned false and the value of iter is "<<iter<<std::endl;
				iter=maxIter;
			}
	} 

//	pressure.printGrid(0);

	std::cout << "maxIter = " << maxIter << std::endl ;

	Real sum = 0.0 ;
	int norm_iter = 0 ;
	for (norm_iter=0; norm_iter < (residual.getSizeX()*residual.getSizeY()); ++norm_iter) {
		sum+=((residual[norm_iter])*(residual[norm_iter]));
	}

//	sum = sqrt(sum) ;
	std::cout << "norm of the residual = " << sum << std::endl ;
	std::cout << "square root of norm of the residual = " << sqrt(sum) << std::endl ;
	std::cout << "gcg->mResNorm = " << gcg->getResNorm() << std::endl ;

	debMsg("FluidSolver::solvePressure iterations:"<<gcg->getIterations()<<", res:"<<gcg->getSigma(), 1);
	delete gcg;
	
//	CorrectVelocity(flags, vel, pressure ); 
//	std::cout<<"Just checking!!"<<std::endl;
//        if (phi) {
//                std::cout<<"Correcting VelocityGhostFluid and Replacing ClampedGhostFluidVels"<<std::endl;
//		CorrectVelocityGhostFluid (vel, flags, pressure, *phi, gfClamp);
//		// improve behavior of clamping for large time steps:
//		ReplaceClampedGhostFluidVels (vel, flags, pressure, *phi, gfClamp);
//	}
}

//! Perform pressure projection of the velocity grid
PYTHON void solvePressure_modifiedCG_part2(MACGrid& vel, Grid<Real>& pressure, FlagGrid& flags,
                     Grid<Real>* phi = 0, 
                     Real gfClamp = 1e-04)
{	
	std::cout<<"minimum value of pressure = " << pressure.getMinValue()<<std::endl;
        std::cout<<"maximum value of pressure = " << pressure.getMaxValue()<<std::endl;
        CorrectVelocity(flags, vel, pressure ); 
        if (phi) {
                std::cout<<"Correcting VelocityGhostFluid and Replacing ClampedGhostFluidVels"<<std::endl;
		CorrectVelocityGhostFluid (vel, flags, pressure, *phi, gfClamp);
		// improve behavior of clamping for large time steps:
		ReplaceClampedGhostFluidVels (vel, flags, pressure, *phi, gfClamp);
	}
}

//! Perform pressure projection of the velocity grid
PYTHON void solvePressure3_part2_target(MACGrid& vel, Grid<Real>& pressure, Grid<Real>& source_density_convolve, FlagGrid& flags,
                     Grid<Real>* phi = 0, 
                     Real gfClamp = 1e-04)
{	

	
        CorrectVelocity_target(flags, vel, pressure, source_density_convolve); 

//        if (phi) {
////                std::cout<<"Correcting VelocityGhostFluid and Replacing ClampedGhostFluidVels"<<std::endl;
//		CorrectVelocityGhostFluid (vel, flags, pressure, *phi, gfClamp);
//		// improve behavior of clamping for large time steps:
//		ReplaceClampedGhostFluidVels (vel, flags, pressure, *phi, gfClamp);
//	}
}

//! Kernel: Apply velocity update from poisson equation
KERNEL(bnd=1) 
void attenuate_target(FlagGrid& flags, MACGrid& vel, Real attenuate_coefficient) 
{
//	std::cout<<"Entered CorrectVelocity"<<std::endl;
        int idx = flags.index(i,j,k);
	if (flags.isFluid(idx))
	{
		if (flags.isFluid(i-1,j,k)) vel[idx].x -= attenuate_coefficient * vel[idx].x ;
		if (flags.isFluid(i,j-1,k)) vel[idx].y -= attenuate_coefficient * vel[idx].y ;
		if (flags.is3D() && flags.isFluid(i,j,k-1)) vel[idx].z -= attenuate_coefficient * vel[idx].z ;

//		if (flags.isEmpty(i-1,j,k)) vel[idx].x -= pressure[idx] ;
//		if (flags.isEmpty(i,j-1,k)) vel[idx].y -= pressure[idx] ;
//		if (flags.is3D() && flags.isEmpty(i,j,k-1)) vel[idx].z -= pressure[idx] ;
	}
//	else if (flags.isEmpty(idx))
//	{
//		if (flags.isFluid(i-1,j,k)) vel[idx].x += pressure(i-1,j,k);
//		else                        vel[idx].x  = 0.f;
//		if (flags.isFluid(i,j-1,k)) vel[idx].y += pressure(i,j-1,k);
//		else                        vel[idx].y  = 0.f;
//		if (flags.is3D() ) {
//		if (flags.isFluid(i,j,k-1)) vel[idx].z += pressure(i,j,k-1);
//		else                        vel[idx].z  = 0.f;
//		}
//	}
}


//! Perform pressure projection of the velocity grid
PYTHON void attenuate(MACGrid& vel, FlagGrid& flags,
                     Real attenuate_coefficient=0.0)
{	

	attenuate_target(flags, vel, attenuate_coefficient); 

}

PYTHON void solvePressure1(MACGrid& vel, Grid<Real>& pressure, FlagGrid& flags, string openBound="",
                     Grid<Real>* phi = 0, 
                     Grid<Real>* perCellCorr = 0, 
                     Real gfClamp = 1e-04,
                     Real cgMaxIterFac = 1.5,
                     Real cgAccuracy = 1e-3,
                     string outflow = "",
                     int outflowHeight = 1,
                     bool precondition = true,
                     bool enforceCompatibility = false,
                     bool useResNorm = true )
{
	std::cout << "Entered solvePressure1 !!" << std::endl ;
        // parse strings
	Vector3D<bool> loOpenBound, upOpenBound, loOutflow, upOutflow;
	convertDescToVec(openBound, loOpenBound, upOpenBound);
	convertDescToVec(outflow, loOutflow, upOutflow);
	if (vel.is2D() && (loOpenBound.z || upOpenBound.z))
		errMsg("open boundaries for z specified for 2D grid");
	
	// reserve temp grids
	FluidSolver* parent = flags.getParent();
	Grid<Real> rhs(parent);
	Grid<Real> residual(parent);
	Grid<Real> search(parent);
	Grid<Real> A0(parent);
	Grid<Real> Ai(parent);
	Grid<Real> Aj(parent);
	Grid<Real> Ak(parent);
	Grid<Real> tmp(parent);
	Grid<Real> pca0(parent);
	Grid<Real> pca1(parent);
	Grid<Real> pca2(parent);
	Grid<Real> pca3(parent);
		
	// setup matrix and boundaries
	MakeLaplaceMatrix (flags, A0, Ai, Aj, Ak);
	SetOpenBound (A0, Ai, Aj, Ak, flags, vel, loOpenBound, upOpenBound);
	
	if (phi) {
		ApplyGhostFluidDiagonal(A0, flags, *phi, gfClamp);
	}
	
	// compute divergence and init right hand side
	MakeRhs kernMakeRhs (flags, rhs, vel, perCellCorr);
	
	if (!outflow.empty())
		SetOutflow (rhs, loOutflow, upOutflow, outflowHeight);
	
	if (enforceCompatibility)
		rhs += (Real)(-kernMakeRhs.sum / (Real)kernMakeRhs.cnt);
	
	// CG setup
	// note: the last factor increases the max iterations for 2d, which right now can't use a preconditioner 
	const int maxIter = (int)(cgMaxIterFac * flags.getSize().max()) * (flags.is3D() ? 1 : 4);
	GridCgInterface *gcg;
	if (vel.is3D())
		gcg = new GridCg<ApplyMatrix>  (pressure, rhs, residual, search, flags, tmp, &A0, &Ai, &Aj, &Ak );
	else
		gcg = new GridCg<ApplyMatrix2D>(pressure, rhs, residual, search, flags, tmp, &A0, &Ai, &Aj, &Ak );
	
	gcg->setAccuracy( cgAccuracy ); 
	gcg->setUseResNorm( useResNorm );

	// optional preconditioning
	gcg->setPreconditioner( precondition ? GridCgInterface::PC_mICP : GridCgInterface::PC_None, &pca0, &pca1, &pca2, &pca3);

	for (int iter=0; iter<maxIter; iter++) {
		if (!gcg->iterate()) iter=maxIter;
	} 
	debMsg("FluidSolver::solvePressure1 iterations:"<<gcg->getIterations()<<", res:"<<gcg->getSigma(), 1);
	delete gcg;
	
	CorrectVelocity1(flags, vel, pressure ); 
	if (phi) {
		CorrectVelocityGhostFluid (vel, flags, pressure, *phi, gfClamp);
		// improve behavior of clamping for large time steps:
		ReplaceClampedGhostFluidVels (vel, flags, pressure, *phi, gfClamp);
	}
}

PYTHON void solvePressure2(MACGrid& vel, Grid<Real>& pressure, FlagGrid& flags, string openBound="",
                     Grid<Real>* phi = 0, 
                     Grid<Real>* perCellCorr = 0, 
                     Real gfClamp = 1e-04,
                     Real cgMaxIterFac = 1.5,
                     Real cgAccuracy = 1e-3,
                     string outflow = "",
                     int outflowHeight = 1,
                     bool precondition = true,
                     bool enforceCompatibility = false,
                     bool useResNorm = true )
{
	std::cout << "Entered solvePressure2 !!" << std::endl ;
        // parse strings
	Vector3D<bool> loOpenBound, upOpenBound, loOutflow, upOutflow;
	convertDescToVec(openBound, loOpenBound, upOpenBound);
	convertDescToVec(outflow, loOutflow, upOutflow);
	if (vel.is2D() && (loOpenBound.z || upOpenBound.z))
		errMsg("open boundaries for z specified for 2D grid");
	
	// reserve temp grids
	FluidSolver* parent = flags.getParent();
	Grid<Real> rhs(parent);
	Grid<Real> residual(parent);
	Grid<Real> search(parent);
	Grid<Real> A0(parent);
	Grid<Real> Ai(parent);
	Grid<Real> Aj(parent);
	Grid<Real> Ak(parent);
	Grid<Real> tmp(parent);
	Grid<Real> pca0(parent);
	Grid<Real> pca1(parent);
	Grid<Real> pca2(parent);
	Grid<Real> pca3(parent);
		
	// setup matrix and boundaries
	MakeLaplaceMatrix (flags, A0, Ai, Aj, Ak);
	SetOpenBound (A0, Ai, Aj, Ak, flags, vel, loOpenBound, upOpenBound);
	
	if (phi) {
		ApplyGhostFluidDiagonal(A0, flags, *phi, gfClamp);
	}
	
	// compute divergence and init right hand side
	MakeRhs kernMakeRhs (flags, rhs, vel, perCellCorr);
	
	if (!outflow.empty())
		SetOutflow (rhs, loOutflow, upOutflow, outflowHeight);
	
	if (enforceCompatibility)
		rhs += (Real)(-kernMakeRhs.sum / (Real)kernMakeRhs.cnt);
	
	// CG setup
	// note: the last factor increases the max iterations for 2d, which right now can't use a preconditioner 
	const int maxIter = (int)(cgMaxIterFac * flags.getSize().max()) * (flags.is3D() ? 1 : 4);
	GridCgInterface *gcg;
	if (vel.is3D())
		gcg = new GridCg<ApplyMatrix>  (pressure, rhs, residual, search, flags, tmp, &A0, &Ai, &Aj, &Ak );
	else
		gcg = new GridCg<ApplyMatrix2D>(pressure, rhs, residual, search, flags, tmp, &A0, &Ai, &Aj, &Ak );
	
	gcg->setAccuracy( cgAccuracy ); 
	gcg->setUseResNorm( useResNorm );

	// optional preconditioning
	gcg->setPreconditioner( precondition ? GridCgInterface::PC_mICP : GridCgInterface::PC_None, &pca0, &pca1, &pca2, &pca3);

	for (int iter=0; iter<maxIter; iter++) {
		if (!gcg->iterate()) iter=maxIter;
	} 
	debMsg("FluidSolver::solvePressure1 iterations:"<<gcg->getIterations()<<", res:"<<gcg->getSigma(), 1);
	delete gcg;
	
	CorrectVelocity2(flags, vel, pressure ); 
	if (phi) {
		CorrectVelocityGhostFluid (vel, flags, pressure, *phi, gfClamp);
		// improve behavior of clamping for large time steps:
		ReplaceClampedGhostFluidVels (vel, flags, pressure, *phi, gfClamp);
	}
}

PYTHON void write_to_file_c(Grid<Real>& target_density_convolve )
{

   // open a file in write mode.
   ofstream outfile;
   outfile.open("/home/tushar/manta_mikey_1/manta/scenes/mine/february_18/foo.txt");

	for (int i = 1 ; i < 127 ; i++)
	for (int j = 1 ; j < 127 ; j++)
	{
		Real data = target_density_convolve(i,j,1);

		// again write inputted data into the file.
		outfile << data << endl;

	}
	// close the opened file.
	outfile.close();
}

KERNEL(bnd=1)
void Gradient_X(Grid<Real>& Grad_x, const Grid<Real>& grid)
{
	Real grad = 0.5 * (grid(i+1,j,k)-grid(i-1,j,k));
//	if(grid.is3D()) grad[2]= 0.5*( grid(i,j,k+1)-grid(i,j,k-1) );
	Grad_x(i,j,k) = grad;
}

PYTHON void calculate_grad_x(Grid<Real>& Grad_x, Grid<Real>& grid)
{
	Gradient_X(Grad_x, grid) ;
}

KERNEL(bnd=1)
void Gradient_Y(Grid<Real>& Grad_y, const Grid<Real>& grid) {
	Real grad = 0.5 * (grid(i,j+1,k)-grid(i,j-1,k));
//	if(grid.is3D()) grad[2]= 0.5*( grid(i,j,k+1)-grid(i,j,k-1) );
	Grad_y(i,j,k) = grad;
}

PYTHON void calculate_grad_y(Grid<Real>& Grad_y, Grid<Real>& grid)
{
	Gradient_Y(Grad_y, grid) ;
}

//! Kernel: divergence operator (from MAC grid)
KERNEL(bnd=1) 
void Divergence_Op(Grid<Real>& divergence_grid, Grid<Real>& Grad_x, Grid<Real>& Grad_y)
{
	Real del_1 = 0.5 * (Grad_x(i+1,j,k) - Grad_x(i-1,j,k));
	Real del_2 = 0.5 * (Grad_y(i,j+1,k) - Grad_y(i,j-1,k));

//	if(grid.is3D()) del[2] += grid(i,j,k+1).z;
//	else            del[2]  = 0.;
	divergence_grid(i,j,k) = del_1 + del_2 ;
}

PYTHON void calculate_divergence(Grid<Real>& divergence_grid, Grid<Real>& Grad_x, Grid<Real>& Grad_y)
{
	Divergence_Op(divergence_grid, Grad_x, Grad_y) ;
}

//! Kernel:
KERNEL(bnd=1)
void assign_value_to_weight_kernelized(Grid<Real>& grid, Real value)
{
	grid(i,j,k) = value ;
}

PYTHON void assign_value_to_weight(Grid<Real>& grid1, Real value)
{
	assign_value_to_weight_kernelized (grid1,value) ;
}

//! Kernel:
KERNEL(bnd=1)
void approximate_weight_from_density_kernel(Grid<Real>& target, Grid<Real>& source)
{
//	int idx = target.index(i, j, k);
//	int idx_1 = target.index(i, j+1, k);
//	int idx_2 = target.index(i-1, j+1, k);
//	int idx_3 = target.index(i-1, j, k);
//	int idx_4 = target.index(i-1, j-1, k);
//	int idx_5 = target.index(i, j-1, k);
//	int idx_6 = target.index(i+1, j-1, k);
//	int idx_7 = target.index(i+1, j, k);
//	int idx_8 = target.index(i+1, j+1, k);
//	target(i,j,k) = (source[idx]+source[idx_1]+source[idx_2]+source[idx_3]+source[idx_4]+source[idx_5]+source[idx_6]+source[idx_7]+source[idx_8])/9.0 ;

	int idx = target.index(i, j, k);
	int idx_1 = target.index(i, j+1, k);
	int idx_2 = target.index(i, j+1, k);
	int idx_3 = target.index(i-1, j, k);
	int idx_4 = target.index(i+1, j, k);
	int idx_5 = target.index(i, j, k-1);
	int idx_6 = target.index(i, j, k+1);
	target(i,j,k) = (source[idx]+source[idx_1]+source[idx_2]+source[idx_3]+source[idx_4]+source[idx_5]+source[idx_6])/7.0 ;
}

PYTHON void approximate_weight_from_density(Grid<Real>& target, Grid<Real>& source)
{
	approximate_weight_from_density_kernel ( target, source) ;
}

//PYTHON void gather(Grid<Real>& density, Grid<Real>& Grad_x, Grid<Real>& divergence_grid)
//{
//	Divergence_Op(divergence_grid, Grad_x, Grad_y) ;
//}

//The new ones for the LaTeX report
//---------------------------------------------------------
//---------------------------------------------------------
//The new ones for the LaTeX report

//! Perform pressure projection of the velocity grid
PYTHON void solvePressure3_part1(MACGrid& vel, Grid<Real>& pressure, FlagGrid& flags, string openBound="",
                     Grid<Real>* phi = 0, 
                     Grid<Real>* perCellCorr = 0, 
                     Real gfClamp = 1e-04,
                     Real cgMaxIterFac = 1.5,
                     Real cgAccuracy = 1e-3,
                     string outflow = "",
                     int outflowHeight = 1,
                     bool precondition = true,
                     bool enforceCompatibility = false,
                     bool useResNorm = true,
                     int max_iters = 3 )
{
        // parse strings
	Vector3D<bool> loOpenBound, upOpenBound, loOutflow, upOutflow;

	convertDescToVec(openBound, loOpenBound, upOpenBound);

	convertDescToVec(outflow, loOutflow, upOutflow);

	if (vel.is2D() && (loOpenBound.z || upOpenBound.z))
		errMsg("open boundaries for z specified for 2D grid");

	// reserve temp grids
	FluidSolver* parent = flags.getParent();
	Grid<Real> rhs(parent);
	Grid<Real> residual(parent);
	Grid<Real> search(parent);
	Grid<Real> A0(parent);
	Grid<Real> Ai(parent);
	Grid<Real> Aj(parent);
	Grid<Real> Ak(parent);
	Grid<Real> tmp(parent);
	Grid<Real> pca0(parent);
	Grid<Real> pca1(parent);
	Grid<Real> pca2(parent);
	Grid<Real> pca3(parent);
		
	// setup matrix and boundaries
	MakeLaplaceMatrix (flags, A0, Ai, Aj, Ak);


	SetOpenBound (A0, Ai, Aj, Ak, flags, vel, loOpenBound, upOpenBound);

	if (phi) {
        	ApplyGhostFluidDiagonal(A0, flags, *phi, gfClamp);
	}

	// compute divergence and init right hand side
	MakeRhs kernMakeRhs (flags, rhs, vel, perCellCorr);

	if (!outflow.empty())
		SetOutflow (rhs, loOutflow, upOutflow, outflowHeight);

	if (enforceCompatibility)
		rhs += (Real)(-kernMakeRhs.sum / (Real)kernMakeRhs.cnt);

	// CG setup
	// note: the last factor increases the max iterations for 2d, which right now can't use a preconditioner 
	const int maxIter = (int)(cgMaxIterFac * flags.getSize().max()) * (flags.is3D() ? 1 : 4);
	GridCgInterface *gcg;
	if (vel.is3D())
		gcg = new GridCg<ApplyMatrix>  (pressure, rhs, residual, search, flags, tmp, &A0, &Ai, &Aj, &Ak );
	else
		gcg = new GridCg<ApplyMatrix2D>(pressure, rhs, residual, search, flags, tmp, &A0, &Ai, &Aj, &Ak );
	
	gcg->setAccuracy( cgAccuracy ); 
	gcg->setUseResNorm( useResNorm );

	// optional preconditioning
	gcg->setPreconditioner( precondition ? GridCgInterface::PC_mICP : GridCgInterface::PC_None, &pca0, &pca1, &pca2, &pca3);

	for (int iter=0; iter<maxIter; iter++) {
		if (!gcg->iterate()) iter=maxIter;
	} 
	debMsg("FluidSolver::solvePressure iterations:"<<gcg->getIterations()<<", res:"<<gcg->getSigma(), 1);
	delete gcg;
}

//! Perform pressure projection of the velocity grid
PYTHON void solvePressure3_part2(MACGrid& vel, Grid<Real>& pressure, FlagGrid& flags,
                     Grid<Real>* phi = 0, 
                     Real gfClamp = 1e-04)
{	
        CorrectVelocity(flags, vel, pressure ); 
        if (phi) {
		CorrectVelocityGhostFluid (vel, flags, pressure, *phi, gfClamp);
		// improve behavior of clamping for large time steps:
		ReplaceClampedGhostFluidVels (vel, flags, pressure, *phi, gfClamp);
	}
}

//        This is a constant function with radius as Gaussian
PYTHON void create_pressure_modifier_12(Grid<Real>& pressure_modification , Grid<Real>& pressure , int radius=3 , Vec3 splat_center = Vec3(32,24,0.5))
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = ( pressure.getMaxAbsValue() )/2;
         }
     }
//   std::cout<<"In ~/manta_mikey_1/manta/source/plugin/pressure.cpp create_pressure_modifier"<<std::endl;
}

//        This is the circular Gaussian
PYTHON void create_circular_gaussian(Grid<Real>& pressure_modification , int radius=3 , Vec3 splat_center = Vec3(40,32,0.5) , Real sigma = 1.0, int multiplier = 4.0)
{
     int idx , idx_x , idx_y , idx_z;
     idx_x = (toVec3i(splat_center)).x;
     idx_y = (toVec3i(splat_center)).y;
     idx_z = (toVec3i(splat_center)).z;

     double r , s = 2.0 * sigma * sigma ;

     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius); x <= (radius); x++)
     {
         for (int y = -(radius); y <= (radius); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r = sqrt(x*x + y*y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp(-(r*r)/s))/(M_PI * s);
         }
     }
 
     Real max_value = pressure_modification.getMaxAbsValue() ;

//   Make the central value 1 and then scale by multiplier
     for(int i = -(radius); i <= (radius); ++i)
     {
         for(int j = -(radius); j <= (radius); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= max_value;
              pressure_modification[idx] *= multiplier;
         }
     }
}

//	This is elliptical Gaussian with multiplier
PYTHON void create_elliptical_gaussian ( Grid<Real>& pressure_modification, int radius_x = 3, int radius_y = 3, Vec3 splat_center = Vec3(40,32,0.5), Real sigma_x = 1.0, Real sigma_y = 2.0, Real multiplier = 4.0 )
{
     int idx , idx_x , idx_y , idx_z ;
     idx_x = (toVec3i(splat_center)).x ;
     idx_y = (toVec3i(splat_center)).y ;
     idx_z = (toVec3i(splat_center)).z ;

     Real r_x , r_y ;
     Real denominator = 2.0 * M_PI * sigma_x * sigma_y ;
    
     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius_x); x <= (radius_x); x++)
     {
         for (int y = -(radius_y); y <= (radius_y); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r_x = (x*x)/(2.0*sigma_x*sigma_x) ;
             r_y = (y*y)/(2.0*sigma_y*sigma_y) ;
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z) ;
             pressure_modification[idx] = (exp((-r_x)+(-r_y)))/(denominator) ;
         }
     }
 
     Real max_value = pressure_modification.getMaxAbsValue() ;

//   normalize the Kernel
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;
              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
              pressure_modification[idx] /= max_value ;
              pressure_modification[idx] *= multiplier ;
         }
     }
}

//	This is elliptical Gaussian with a spatial gradient
PYTHON void create_elliptical_gaussian_cubic_gradient( Grid<Real>& pressure_modification , int radius_x=3 , int radius_y=3 , Vec3 splat_center = Vec3(40,32,0.5) , Real sigma_x = 1.0 , Real sigma_y = 2.0, Real multiplier = 4.0, Real lower_factor = 0.0, Real higher_factor = 1.0 )
{
     int idx , idx_x , idx_y , idx_z ;
     idx_x = (toVec3i(splat_center)).x ;
     idx_y = (toVec3i(splat_center)).y ;
     idx_z = (toVec3i(splat_center)).z ;

     Real r_x , r_y ;
     Real denominator = 2.0 * M_PI * sigma_x * sigma_y ;
 
     int temp_idx_x , temp_idx_y ;

//   generate 5x5 kernel
     for (int x = -(radius_x); x <= (radius_x); x++)
     {
         for (int y = -(radius_y); y <= (radius_y); y++)
         {
             temp_idx_x = idx_x + x ;
             temp_idx_y = idx_y + y ;
             r_x = (x*x)/(2.0*sigma_x*sigma_x);
             r_y = (y*y)/(2.0*sigma_y*sigma_y);
             idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);
             pressure_modification[idx] = (exp((-r_x)+(-r_y)))/(denominator);
         }
     }
 
	Real max_value = pressure_modification.getMaxAbsValue() ;

//   normalize the Kernel
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              temp_idx_x = idx_x + i ;
              temp_idx_y = idx_y + j ;

              idx = pressure_modification.index(temp_idx_x , temp_idx_y , idx_z);

              pressure_modification[idx] /= max_value ;
              pressure_modification[idx] *= multiplier ;

              Real j_temp = j + radius_y ;
              Real m = (higher_factor - lower_factor)/(2.0 * radius_y) ;
              Real c = lower_factor ;
              Real value = (m * (j_temp) + c);
// Linear
              Real fraction = (value - lower_factor)/(higher_factor - lower_factor) ;
//// Quadratic
//              fraction = fraction * fraction ;
// Cubic
              fraction = fraction * fraction * fraction ;

              pressure_modification[idx] = (fraction * higher_factor) * pressure_modification[idx] ;
         }
     }
}

//	This is a perlin template for the report
PYTHON void create_static_perlin_template_general(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real frequency_changer_x=1.0, Real frequency_changer_y=1.0, int time = 1 )
{
	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x,hy*frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	max_perlin_noise_value = pressure_modification.getMaxAbsValue();

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);

	string str ;
	if(time < 10)
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/2_elliptical_perlin/template/perlin00" + str_frequency_changer + ".png";}
	else if((time >= 10) && (time <= 99))
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/2_elliptical_perlin/template/perlin0" + str_frequency_changer + ".png";}
	else if((time >= 100) && (time <= 999))
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/2_elliptical_perlin/template/perlin" + str_frequency_changer + ".png";}

	const char *file_name_output = str.c_str();

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}

//	This is a time-varying perlin template with X and Y alternating
PYTHON void create_perlin_template_x_y_abrupt_translation_3D(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int res_z = 128, Real frequency_changer_1 = 7, Real frequency_changer_2 = 21, int time = 1)
{

	Real frequency_changer_x = 0 ;
	Real frequency_changer_y = 0 ;

	int time_depender = (time % 2) ;
	if(time_depender == 0)
	{
		frequency_changer_x = frequency_changer_1 ;
		frequency_changer_y = frequency_changer_2 ;
	}
	else
	{
		frequency_changer_x = frequency_changer_2 ;
		frequency_changer_y = frequency_changer_1 ;
	}

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y, depth = res_z ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			for(int z = 1; z< depth-1; z++)
			{
				double hx = (double)x/width ;
				double hy = (double)y/height ;

				noise = stb_perlin_noise3(hx * frequency_changer_x + 0.025 * time, hy * frequency_changer_y + 0.025 * time, 1.75,xWrap=0,yWrap=0,zWrap=0);
				noise = (noise + 1.0) / 2.0; // set range to 0 - 1

				idx = pressure_modification.index(x , y , z);
				pressure_modification[idx] = noise;

//				int linear_index = y * width + x ;
//				unsigned char col = int(noise * 255);
//				noise_data[linear_index] = col ;
			}
		}
	}

	max_perlin_noise_value = pressure_modification.getMaxAbsValue() ;

//	Scaling
	for(int x = 1; x < width-1; x++)
	{
		for(int y = 1; y < height-1; y++)
		{
			for(int z = 1; z < depth-1; z++)
			{
				idx = pressure_modification.index(x , y , z);
				pressure_modification[idx] /= max_perlin_noise_value;
				pressure_modification[idx] *= multiplier;
			}
		}
	}

//	char Result[8]; // string which will contain the number
//	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
//	string str_frequency_changer(Result);

//	string str ;
//	if(time < 10)
//	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/3_x_y_abrupt/template/perlin00" + str_frequency_changer + ".png";}
//	else if((time >= 10) && (time <= 99))
//	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/3_x_y_abrupt/template/perlin0" + str_frequency_changer + ".png";}
//	else if((time >= 100) && (time <= 999))
//	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/3_x_y_abrupt/template/perlin" + str_frequency_changer + ".png";}

//	const char *file_name_output = str.c_str();

//	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
//	free (noise_data);
//	if(a == 1)
//	{
//		std::cout<<"SUCCESS"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"FAILURE"<<std::endl;
//	}
}

//	This is a time-varying perlin template with X and Y alternating
PYTHON void create_perlin_template_x_y_abrupt(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real frequency_changer_1 = 7, Real frequency_changer_2 = 21, int time = 1)
{

	Real frequency_changer_x = 0 ;
	Real frequency_changer_y = 0 ;

	int time_depender = (time % 2) ;
	if(time_depender == 0)
	{
		frequency_changer_x = frequency_changer_1 ;
		frequency_changer_y = frequency_changer_2 ;
	}
	else
	{
		frequency_changer_x = frequency_changer_2 ;
		frequency_changer_y = frequency_changer_1 ;
	}

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x,hy*frequency_changer_y,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	max_perlin_noise_value = pressure_modification.getMaxAbsValue() ;

//	Scaling
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);

	string str ;
	if(time < 10)
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/3_x_y_abrupt/template/perlin00" + str_frequency_changer + ".png";}
	else if((time >= 10) && (time <= 99))
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/3_x_y_abrupt/template/perlin0" + str_frequency_changer + ".png";}
	else if((time >= 100) && (time <= 999))
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/3_x_y_abrupt/template/perlin" + str_frequency_changer + ".png";}

	const char *file_name_output = str.c_str();

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}

//	This is a time-varying perlin template with X and Y alternating 
PYTHON void create_perlin_template_translation(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, Real frequency_changer_1 = 7, Real frequency_changer_2 = 21, int time = 1)
{

	Real frequency_changer_x = 0;
	Real frequency_changer_y = 0;

	int time_depender = ((time) % 2) ;
	if(time_depender == 1)
	{
		frequency_changer_x = frequency_changer_1 ;
		frequency_changer_y = frequency_changer_2 ;
	}
	else
	{
		frequency_changer_x = frequency_changer_2 ;
		frequency_changer_y = frequency_changer_1 ;
	}

	int xWrap,yWrap,zWrap ;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx*frequency_changer_x + 0.025 * time,hy*frequency_changer_y+ 0.025*time,1.75,xWrap=0,yWrap=0,zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	max_perlin_noise_value = pressure_modification.getMaxAbsValue() ;

	for(int x = 1 ; x < (width - 1) ; x++)
	{
		for(int y = 1; y < (height - 1) ; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);

	string str ;
	if(time < 10)
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/4_translation/template/perlin00" + str_frequency_changer + ".png";}
	else if((time >= 10) && (time <= 99))
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/4_translation/template/perlin0" + str_frequency_changer + ".png";}
	else if((time >= 100) && (time <= 999))
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/4_translation/template/perlin" + str_frequency_changer + ".png";}

	const char *file_name_output = str.c_str();

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}

//	This is a time-varying perlin template with interpolating from X to Y to X
PYTHON void create_perlin_template_global_interpolation_x_y ( Grid<Real>& pressure_modification, Real multiplier = 4.0, int res_x=128, int res_y=128, int time = 1, Real start_x = 5.0, Real start_y = 15.0, int time_steps = 30 )
{

//	We move from x:y = start_x:start_y to x:y start_y:start_y
	Real frequency_changer_x = 0 ;
	Real frequency_changer_y = 0 ;

	int time_depender = ((time - 1) % time_steps) ;
	if(time_depender < (time_steps/2))
	{
		frequency_changer_x = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
		frequency_changer_y = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
	}
	else
	{
		time_depender = time_depender - time_steps/2 ;
		frequency_changer_x = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
		frequency_changer_y = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
	}

//	std::ofstream outfile;
//	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

//	outfile << " time = " << time << endl ;
//	outfile << " time_depender = " << time_depender << endl ;
//	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
//	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
//	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

			noise = stb_perlin_noise3(hx * frequency_changer_x + 0.025 * time, hy * frequency_changer_y + 0.025 * time, 1.75, xWrap=0, yWrap=0, zWrap=0) ;
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

	max_perlin_noise_value = pressure_modification.getMaxAbsValue() ;

	for(int x = 1; x < width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);

	string str ;
	if(time < 10)
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/5_global_interpolation/template/perlin00" + str_frequency_changer + ".png";}
	else if((time >= 10) && (time <= 99))
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/5_global_interpolation/template/perlin0" + str_frequency_changer + ".png";}
	else if((time >= 100) && (time <= 999))
	{str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/5_global_interpolation/template/perlin" + str_frequency_changer + ".png";}

	const char *file_name_output = str.c_str();

	int a = 1 ;
	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width)) ;
	free (noise_data) ;
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl ;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl ;
	}
}

//	This is a time-varying perlin template with interpolating from X to Y to X
PYTHON void create_perlin_template_local_interpolation_x_y ( Grid<Real>& pressure_modification, Real multiplier = 4.0, int res_x=128, int res_y=128, int frequency_x=1, int frequency_y=1, string folder_templates_number="1", int time=1 )
{
	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				double hx = (double)x/width ;
				double hy = (double)y/height ;
				noise = stb_perlin_noise3(hx*frequency_x + 0.025 * time ,hy*frequency_y + 0.025 * time ,1.75,xWrap=0,yWrap=0,zWrap=0);
				noise = (noise + 1.0) / 2.0; // set range to 0 - 1

				if(noise < 0.0){std::cout << " IMPOSSIBLE NEGATIVE VALUE IS POSSIBLE " << std::endl ;}

				idx = pressure_modification.index(x , y , 1);
				pressure_modification[idx] = noise;

				int linear_index = y * width + x ;
				unsigned char col = int(noise * 255);
				noise_data[linear_index] = col ;
			}
		}

		Real max_value = pressure_modification.getMaxValue() ;

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				idx = pressure_modification.index(x , y , 1);
				pressure_modification[idx] /= max_value ;
				pressure_modification[idx] *= multiplier;
			}
		}

		char Result[8]; // string which will contain the number
		sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer

		string str_frequency_changer(Result);
		if(time < 10)
		{ str_frequency_changer = "00" + str_frequency_changer ; }
		else if((time >= 10) && (time <=99))
		{str_frequency_changer = "0" + str_frequency_changer ;}

		string str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/6_local_interpolation/template/"+ folder_templates_number + "/" + str_frequency_changer + ".png" ;
		const char *file_name_output = str.c_str();

		int a = 1 ;
//		a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}
	free (noise_data);
}

//	This is a time-varying perlin template with interpolating from X to Y to X
PYTHON void print_perlin_template_local_interpolation_x_y ( Real multiplier = 4.0, int res_x=128, int res_y=128, int frequency_x_1=1, int frequency_y_1=1, int frequency_x_2=1, int frequency_y_2=1, string folder_templates_number="1", int time=1, int time_steps = 20 )
{
	int xWrap,yWrap,zWrap;
	int width = res_x , height = res_y ;
	double noise = 0.0, noise1 = 0.0, noise2 = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				double hx = (double)x/width ;
				double hy = (double)y/height ;

				noise1 = stb_perlin_noise3(hx*frequency_x_1 + 0.025 * time ,hy*frequency_y_1 + 0.025 * time ,1.75,xWrap=0,yWrap=0,zWrap=0);
				noise1 = (noise1 + 1.0) / 2.0; // set range to 0 - 1

				noise2 = stb_perlin_noise3(hx*frequency_x_2 + 0.025 * time ,hy*frequency_y_2 + 0.025 * time ,1.75,xWrap=0,yWrap=0,zWrap=0);
				noise2 = (noise2 + 1.0) / 2.0; // set range to 0 - 1

				Real linear_interpolate ;
				Real start_x = 0.0 ;
				Real start_y = 1.0 ;

				int time_depender = ((time - 1) % time_steps) ;
				if(time_depender < (time_steps/2))
				{
					linear_interpolate = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
				}
				else
				{
					time_depender = time_depender - time_steps/2 ;
					linear_interpolate = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
				}

				noise1 *= (linear_interpolate) ;
				noise2 *= (1.0 - linear_interpolate) ;
				noise = noise1 + noise2 ;

				if(noise < 0.0){std::cout << " IMPOSSIBLE NEGATIVE VALUE IS POSSIBLE " << std::endl ;}

				int linear_index = y * width + x ;
				unsigned char col = int(noise * 255);
				noise_data[linear_index] = col ;
			}
		}

		char Result[8]; // string which will contain the number
		sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer

		string str_frequency_changer(Result);
		if(time < 10)
		{ str_frequency_changer = "00" + str_frequency_changer ; }
		else if((time >= 10) && (time <=99))
		{str_frequency_changer = "0" + str_frequency_changer ;}

		string str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/6_local_interpolation/template/"+ folder_templates_number + "/" + str_frequency_changer + ".png" ;
		const char *file_name_output = str.c_str();

		int a = 1 ;
		a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}
	free (noise_data);
}

//	This is a time-varying perlin template with global rotation
PYTHON void create_perlin_template_global_rotation ( Grid<Real>& pressure_modification, Real multiplier = 1.0, int res_x=128, int res_y=128, int time = 1, Real x_factor=1.0, Real y_factor=1.0 )
{
//	Changing angle from 0.0 to 360.0 degrees from time = 1 : 301
//	In degrees
	Real theta = (2.0)*time  - 2.0;
//	In radians
	Real PI = 3.14159265 ;
	Real theta_r = -(PI/180.0)* theta;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double a[3][3];
			int m,n;
			double b[3][3];
			double determinant=0;

		//	double aa = (((double)width/2.0)/width)*x_factor; double bb = (((double)height/2.0)/height)*y_factor;
			double aa = (((double)width/2.0));
			double bb = (((double)height/2.0));

			a[0][0] = cos(theta_r) ;
			a[0][1] = -sin(theta_r) ;
			a[0][2] =  -aa*cos(theta_r)+bb*sin(theta_r)+aa;
			a[1][0] =  sin(theta_r);
			a[1][1] =  cos(theta_r);
			a[1][2] =  -aa*sin(theta_r)-bb*cos(theta_r)+bb;
			a[2][0] =  0.0;
			a[2][1] =  0.0;
			a[2][2] =  1.0;

		for(m=0;m<3;m++)
		{
			determinant = determinant + (a[0][m]*(a[1][(m+1)%3]*a[2][(m+2)%3] - a[1][(m+2)%3]*a[2][(m+1)%3]));
		}

		for(m=0;m<3;m++)
		{
			for(n=0;n<3;n++)
			{
				b[n][m]=((a[(m+1)%3][(n+1)%3]*a[(m+2)%3][(n+2)%3])-(a[(m+1)%3][(n+2)%3]* a[(m+2)%3][(n+1)%3]))/determinant;
			}
		}

			double xdash = (b[0][0])*(x)+(b[0][1])*(y)+(b[0][2])*1 ;
			double ydash = (b[1][0])*(x)+(b[1][1])*(y)+(b[1][2])*1 ;
			double hx  ;
			double hy  ;
//			double center_x ;
//			double center_y ;

			hx = xdash/width ;
			hy = ydash/width ;

			noise = stb_perlin_noise3(hx*x_factor,hy*y_factor,1.75,xWrap=0,yWrap=0,zWrap=0);
			double temp1 = noise;
			
			noise = (temp1 + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

		max_perlin_noise_value = pressure_modification.getMaxValue() ;

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/7_global_rotation/template/" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}

PYTHON void create_perlin_template_sinusoidal_interpolation(Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int time=1, Real frequency_x = 1.0, Real frequency_y = 1.0)
{

//	std::ofstream outfile;
//	outfile.open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", std::ios_base::app);

//	outfile << " time = " << time << endl ;
//	outfile << " x_factor = " << x_factor << endl ;
//	outfile << " y_factor = " << y_factor << endl ;
//	outfile << " frequency_changer_x = " << frequency_changer_x << endl ;
//	outfile << " frequency_changer_y = " << frequency_changer_y << endl ;
//	outfile << endl ;

	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;
	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			double hx = (double)x/width ;
			double hy = (double)y/height ;

//Since sin is taken in radians, 2*pi = 6.28

			noise = stb_perlin_noise3(hx * frequency_x + 0.025 * time, hy * frequency_y  + 0.025 * time, 1.75, xWrap=0, yWrap=0, zWrap=0);
			noise = (noise + 1.0) / 2.0; // set range to 0 - 1

			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] = noise;

			int linear_index = y * width + x ;
			unsigned char col = int(noise * 255);
			noise_data[linear_index] = col ;
		}
	}

		max_perlin_noise_value = pressure_modification.getMaxValue() ;

	for(int x = 1; x<width-1; x++)
	{
		for(int y = 1; y<height-1; y++)
		{
			idx = pressure_modification.index(x , y , 1);
			pressure_modification[idx] /= max_perlin_noise_value;
			pressure_modification[idx] *= multiplier;
		}
	}

	char Result[8]; // string which will contain the number
	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
	string str_frequency_changer(Result);
	if(time < 10)
	{ str_frequency_changer = "00" + str_frequency_changer ; }
	else if((time >= 10) && (time <=99))
	{str_frequency_changer = "0" + str_frequency_changer ;}

	string str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/8_sinusoidal_warping/template/1/" + str_frequency_changer + ".png" ;
	const char *file_name_output = str.c_str();

	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
	free (noise_data);
	if(a == 1)
	{
		std::cout<<"SUCCESS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILURE"<<std::endl;
	}
}

//	This is a time-varying perlin template with interpolating from X to Y to X
PYTHON void print_perlin_template_sinusoidal_warping ( Real multiplier = 4.0, int res_x=128, int res_y=128, int frequency_x_1=1, int frequency_y_1=1, int frequency_x_2=1, int frequency_y_2=1, string folder_templates_number="1", int time=1, int time_steps = 20 )
{
	int xWrap,yWrap,zWrap;
	int width = res_x , height = res_y ;
	double noise = 0.0, noise1 = 0.0, noise2 = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				double hx = (double)x/width ;
				double hy = (double)y/height ;

				noise1 = stb_perlin_noise3(hx*frequency_x_1 + 0.025 * time ,hy*frequency_y_1 + 0.025 * time ,1.75,xWrap=0,yWrap=0,zWrap=0);
				noise1 = (noise1 + 1.0) / 2.0; // set range to 0 - 1

				noise2 = stb_perlin_noise3(hx*frequency_x_2 + 0.025 * time ,hy*frequency_y_2 + 0.025 * time ,1.75,xWrap=0,yWrap=0,zWrap=0);
				noise2 = (noise2 + 1.0) / 2.0; // set range to 0 - 1

				Real linear_interpolate ;
				Real start_x = 0.0 ;
				Real start_y = 1.0 ;

				int time_depender = ((2 * 3.14159)/(time_steps)) * ( time - 1 ) ;
				linear_interpolate = ( sin(time_depender) + 1 )/2.0 ;

				noise1 *= (linear_interpolate) ;
				noise2 *= (1.0 - linear_interpolate) ;
				noise = noise1 + noise2 ;

				if(noise < 0.0){std::cout << " IMPOSSIBLE NEGATIVE VALUE IS POSSIBLE " << std::endl ;}

				int linear_index = y * width + x ;
				unsigned char col = int(noise * 255);
				noise_data[linear_index] = col ;
			}
		}

		char Result[8]; // string which will contain the number
		sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer

		string str_frequency_changer(Result);
		if(time < 10)
		{ str_frequency_changer = "00" + str_frequency_changer ; }
		else if((time >= 10) && (time <=99))
		{str_frequency_changer = "0" + str_frequency_changer ;}

		string str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/8_sinusoidal_warping/template/"+ folder_templates_number + "/" + str_frequency_changer + ".png" ;
		const char *file_name_output = str.c_str();

		int a = 1 ;
		a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}
	free (noise_data);
}

//	This is for comparing 2 frequencies, lower and higher
//	This is a time-varying perlin template with interpolating from X to Y to X
PYTHON void create_perlin_template_higher_frequencies_local_interpolation_x_y ( Grid<Real>& pressure_modification, Real multiplier = 4.0, int res_x=128, int res_y=128, int frequency_x=1, int frequency_y=1, string folder_templates_number="1", int time=1 )
{
	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				double hx = (double)x/width ;
				double hy = (double)y/height ;
				noise = stb_perlin_noise3 ( hx * frequency_x + 0.025 * time , hy * frequency_y + 0.025 * time , 1.75, xWrap=0, yWrap=0, zWrap=0 ) ;
				noise = (noise + 1.0) / 2.0; // set range to 0 - 1

				if(noise < 0.0){std::cout << " IMPOSSIBLE NEGATIVE VALUE IS POSSIBLE " << std::endl ;}

				idx = pressure_modification.index(x , y , 1);
				pressure_modification[idx] = noise;

				int linear_index = y * width + x ;
				unsigned char col = int(noise * 255);
				noise_data[linear_index] = col ;
			}
		}

		Real max_value = pressure_modification.getMaxValue() ;

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				idx = pressure_modification.index(x , y , 1);
				pressure_modification[idx] /= max_value ;
				pressure_modification[idx] *= multiplier;
			}
		}

		char Result[8]; // string which will contain the number
		sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer

		string str_frequency_changer(Result);
		if(time < 10)
		{ str_frequency_changer = "00" + str_frequency_changer ; }
		else if((time >= 10) && (time <=99))
		{str_frequency_changer = "0" + str_frequency_changer ;}

		string str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/9_higher_frequencies/template/"+ folder_templates_number + "/" + str_frequency_changer + ".png" ;
		const char *file_name_output = str.c_str();

		int a = 1 ;
//		a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}
	free (noise_data);
}

//	This is a time-varying perlin template with interpolating from X to Y to X
PYTHON void print_perlin_template_higher_frequencies_local_interpolation_x_y ( int res_x=128, int res_y=128, int frequency_x_1=1, int frequency_y_1=1, int frequency_x_2=1, int frequency_y_2=1, string folder_templates_number="1", int time=1, int time_steps = 20 )
{
	int xWrap,yWrap,zWrap;
	int width = res_x , height = res_y ;
	double noise = 0.0, noise1 = 0.0, noise2 = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				double hx = (double)x/width ;
				double hy = (double)y/height ;

				noise1 = stb_perlin_noise3 ( hx * frequency_x_1 + 0.025 * time , hy * frequency_y_1 + 0.025 * time, 1.75, xWrap=0, yWrap=0, zWrap=0 ) ;
				noise1 = (noise1 + 1.0) / 2.0; // set range to 0 - 1

				noise2 = stb_perlin_noise3 ( hx * frequency_x_2 + 0.025 * time ,hy * frequency_y_2 + 0.025 * time , 1.75, xWrap=0, yWrap=0, zWrap=0 ) ;
				noise2 = (noise2 + 1.0) / 2.0; // set range to 0 - 1

				Real linear_interpolate ;
				Real start_x = 0.0 ;
				Real start_y = 1.0 ;

				int time_depender = ((time - 1) % time_steps) ;
				if(time_depender < (time_steps/2))
				{
					linear_interpolate = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender ;
				}
				else
				{
					time_depender = time_depender - time_steps/2 ;
					linear_interpolate = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender ;
				}

				noise1 *= (linear_interpolate) ;
				noise2 *= (1.0 - linear_interpolate) ;
				noise = noise1 + noise2 ;

				if(noise < 0.0){std::cout << " IMPOSSIBLE NEGATIVE VALUE IS POSSIBLE " << std::endl ;}

				int linear_index = y * width + x ;
				unsigned char col = int(noise * 255);
				noise_data[linear_index] = col ;
			}
		}

		char Result[8]; // string which will contain the number
		sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer

		string str_frequency_changer(Result);
		if(time < 10)
		{ str_frequency_changer = "00" + str_frequency_changer ; }
		else if((time >= 10) && (time <=99))
		{str_frequency_changer = "0" + str_frequency_changer ;}

		string str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/9_higher_frequencies/template/"+ folder_templates_number + "/" + str_frequency_changer + ".png" ;
		const char *file_name_output = str.c_str();

		int a = 1 ;
		a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}
	free (noise_data);
}

//	This is a perlin template
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_template_multiple_frequencies ( Grid<Real>& pressure_modification, Real multiplier = 4.0, int res_x=128, int res_y=128, int levels = 1, int time = 1, Real x_factor = 8.0, Real y_factor = 32.0, string folder_templates_number="1", Real persistence = 1 )
{
	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y ;
	double max_perlin_noise_value = -2.0 ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

    for(int x = 1; x<width-1; x++)
    {
        for(int y = 1; y<height-1; y++)
        {

            double hx = (double)x/width ;
            double hy = (double)y/height ;
            Real noise_sum = 0.0 ;
            for(int level = 1 ; level <= levels ; level++ )
            {
		noise = stb_perlin_noise3(hx*pow(2,level-1) * x_factor, hy*pow(2,level-1) * y_factor, 1.75, xWrap=0, yWrap=0, zWrap=0);
                noise = (noise + 1.0)/2 ; // set range to 0 - 1
                if(noise < 0.0){cout<<"ALERT ALERT"<<endl;}
                noise_sum += noise/(pow(persistence,level-1));
            }

            noise_sum /= (levels/persistence);

            idx = pressure_modification.index(x , y , 1);
            pressure_modification[idx] = noise_sum;

            int linear_index = y * width + x ;
            unsigned char col = int(noise_sum * 255);
            noise_data[linear_index] = col ;
        }
    }

	max_perlin_noise_value = pressure_modification.getMaxValue() ;

	for(int x = 1; x < width-1; x++)
	{
		for(int y = 1; y < height-1; y++)
		{
		    idx = pressure_modification.index(x , y , 1);
		    pressure_modification[idx] /= max_perlin_noise_value;
		    pressure_modification[idx] *= multiplier;
		}
	}

		char Result[8]; // string which will contain the number
		sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer

		string str_frequency_changer(Result);
		if(time < 10)
		{ str_frequency_changer = "00" + str_frequency_changer ; }
		else if((time >= 10) && (time <=99))
		{str_frequency_changer = "0" + str_frequency_changer ;}

		string str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/9_multiple_frequencies/template/"+ folder_templates_number + "/" + str_frequency_changer + ".png" ;
		const char *file_name_output = str.c_str();

		int a = 1 ;
		a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}
	free (noise_data);
}

//	This is elliptical Gaussian
PYTHON void create_ellipsoidal_gaussian_3D(Grid<Real>& pressure_modification, int radius_x=3, int radius_y=3, int radius_z=3, Vec3 splat_center = Vec3(40,32,0.5), Real a = 1.0, Real b = 2.0, Real c = 1.0, Real m = 1.0, Real n = 0.0, Real p = 1.0, Real multiplier = 4.0)
{
	int idx , idx_x , idx_y , idx_z;
	idx_x = (toVec3i(splat_center)).x;
	idx_y = (toVec3i(splat_center)).y;
	idx_z = (toVec3i(splat_center)).z;

	Real r1_x, r1_y, r1_z, r2_x, r2_y, r2_z ;
//	Real denominator = 2.0 * M_PI * sigma_x * sigma_y ;

//	sum is for normalization
	idx = pressure_modification.index(idx_x, idx_y, idx_z);

	int temp_idx_x, temp_idx_y, temp_idx_z ;

//	generate 5x5 kernel
     for (int x = -(radius_x); x <= (radius_x); x++)
     {
         for (int y = -(radius_y); y <= (radius_y); y++)
         {
             for (int z = -(radius_z); z <= (radius_z); z++)
             {
                 temp_idx_x = idx_x + x ;
                 temp_idx_y = idx_y + y ;
                 temp_idx_z = idx_z + z ;
                 
                 r1_x = (x*x)*a;
                 r1_y = (y*y)*b;
                 r1_z = (z*z)*c;
                 
                 r2_x = 2.0 * m * x * y ; 
                 r2_y = 2.0 * n * x * z ;
                 r2_z = 2.0 * p * y * z ;
                 
                 idx = pressure_modification.index(temp_idx_x , temp_idx_y , temp_idx_z);
                 pressure_modification[idx] = (exp((-r1_x)+(-r1_y)+(-r1_z)+(-r2_x)+(-r2_y)+(-r2_z)));
             }
         }
     }

	idx = pressure_modification.index(idx_x , idx_y , idx_z);
	Real max_value = pressure_modification.getMaxValue() ;

//	Make the central value 1 and accordingly adjust other values
     for(int i = -(radius_x); i <= (radius_x); ++i)
     {
         for(int j = -(radius_y); j <= (radius_y); ++j)
         {        
              for(int k = -(radius_z); k <= (radius_z); ++k)
              {
                  temp_idx_x = idx_x + i ;
                  temp_idx_y = idx_y + j ;
                  temp_idx_z = idx_z + k ;

                  idx = pressure_modification.index(temp_idx_x , temp_idx_y , temp_idx_z);
                  
                  pressure_modification[idx] /= (max_value);
                  pressure_modification[idx] *= (multiplier);
                  
              }
         }
     }
}

//	This is a perlin template 
//	Grid-size equal to the flag grid
//	Multiple frequencies added
PYTHON void create_perlin_test_template (int res_x=128, int res_y=128, int res_z=128, int frequency_x=1, int frequency_y=1, int frequency_z=1, int z=1)
{
	int xWrap,yWrap,zWrap;
//	int idx ;
	int width = res_x , height = res_y , depth = res_z ;
	double noise = 0.0 ;
	unsigned char *noise_data = (unsigned char *) calloc (width*height,sizeof(unsigned char)) ;

		for(int x = 1; x<width-1; x++)
		{
			for(int y = 1; y<height-1; y++)
			{
				double hx = (double)x/width ;
				double hy = (double)y/height ;
				double hz = (double)z/depth ;
				hz = 1.75 ;
				hy = 1.75 ;
				noise = stb_perlin_noise3(hx*frequency_x,hy*frequency_y,hz*frequency_z,xWrap=0,yWrap=0,zWrap=0);
				noise = (noise + 1.0) / 2.0; // set range to 0 - 1

				if(noise < 0.0){std::cout << " IMPOSSIBLE NEGATIVE VALUE IS POSSIBLE " << std::endl ;}

				int linear_index = y * width + x ;
				unsigned char col = int(noise * 255);
				noise_data[linear_index] = col ;
			}
		}

		char Result[8]; // string which will contain the number
		sprintf ( Result, "%d", int(z) ); // %d makes the result be a decimal integer

		string str_frequency_changer(Result);
		if(z < 10)
		{ str_frequency_changer = "00" + str_frequency_changer ; }
		else if((z >= 10) && (z <=99))
		{str_frequency_changer = "0" + str_frequency_changer ;}

		string str = "/media/tushar/B19D-ABF3/latex1/6_3D_april_11/9_test_perlin/" + str_frequency_changer + ".png" ;
		const char *file_name_output = str.c_str();

		int a = 1 ;
		a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
		if(a == 1)
		{
			std::cout<<"SUCCESS"<<std::endl;
		}
		else
		{
			std::cout<<"FAILURE"<<std::endl;
		}
	free (noise_data);
}

//! Kernel: Construct the right-hand side of the poisson equation
KERNEL(bnd=1)
void scale_values_create_perlin_template_3D_kernel (Grid<Real>& pressure_modification,Real multiplier,int time,Real max_absolute,Real max_value,Real min_value)
{
	int idx = pressure_modification.index(i , j , k) ;
	pressure_modification[idx] /= max_value ;
	pressure_modification[idx] *= multiplier ;
}

//! Kernel: Construct the right-hand side of the poisson equation
//KERNEL(bnd=1, reduce=+) returns(int cnt=0) returns(double sum=0)
KERNEL(bnd=1)
void step_1_create_perlin_template_3D_kernel (Grid<Real>& pressure_modification,int res_x,int res_y,int res_z,int levels,int time,Real frequency_x,Real frequency_y,Real frequency_z,Real translation_x=0.1, Real translation_y=0.1, Real translation_z=0.1)
{
	int xWrap,yWrap,zWrap;
	int idx ;
	int width = res_x , height = res_y , depth = res_z ;
	double noise = 0.0 ;

	double hx = (double)i/width ;
	hx*=frequency_x ;

	double hy = (double)j/height ;
	hy*=frequency_y ;

	double hz = (double)k/depth ;
	hz*=frequency_z ;

	hz = 1.75 ;
	noise = stb_perlin_noise3(hx, hy, hz, xWrap=0, yWrap=0, zWrap=0);
	noise = (noise + 1.0) / 2.0; // set range to 0 - 1
	if(noise < 0.0){cout<<"ALERT ALERT"<<endl;}

	idx = pressure_modification.index(i , j , k);
	pressure_modification[idx] = noise;
}

//	This is a perlin template in 3D
PYTHON void create_perlin_template_3D_kernel (Grid<Real>& pressure_modification,Real multiplier = 4.0, int res_x=128, int res_y=128, int res_z=128, int levels = 1,int time = 1, Real frequency_x=1, Real frequency_y=1, Real frequency_z=1, Real translation_x=0.1, Real translation_y=0.1, Real translation_z=0.1)
{

//	Get the noise value for that particular index	and assign to the grid

//	step_1 Put_noise_value_2_cell(pressure_modification,multiplier,res_x,res_y,res_z,levels,time);
	step_1_create_perlin_template_3D_kernel(pressure_modification,res_x,res_y,res_z,levels,time,frequency_x,frequency_y,frequency_z,translation_x, translation_y, translation_z);

//	Get the maximum values
	Real max_absolute = pressure_modification.getMaxAbsValue();
	Real max_value = pressure_modification.getMaxValue();
	Real min_value = pressure_modification.getMinValue();

//	Scale the values
	scale_values_create_perlin_template_3D_kernel(pressure_modification,multiplier,time,max_absolute,max_value,min_value);
}

//! Kernel: Construct the right-hand side of the poisson equation
//KERNEL(bnd=1, reduce=+) returns(int cnt=0) returns(double sum=0)
KERNEL(bnd=1)
void step_1_create_perlin_template_rotate_3D_kernel (Grid<Real>& pressure_modification,int res_x,int res_y,int res_z,int time,Real frequency_x,Real frequency_y,Real frequency_z, int every_time_step = 1, Real angle = 0)
{
	int xWrap, yWrap, zWrap;
	int idx ;
	int width = res_x , height = res_y  ;
	double noise = 0.0 ;

//	In degrees
	Real theta = angle ;
//	In radians
	Real PI = 3.14159265 ;
	Real theta_r = -(PI/180.0)* theta;

	double a[3][3];
	int m,n;
	double b[3][3];
	double determinant=0;

//	double aa = (((double)width/2.0)/width)*x_factor; double bb = (((double)height/2.0)/height)*y_factor;
	double aa = (((double)width/2.0));
	double bb = (((double)height/2.0));

	a[0][0] = cos(theta_r) ;
	a[0][1] = -sin(theta_r) ;
	a[0][2] =  -aa*cos(theta_r)+bb*sin(theta_r)+aa ;
	a[1][0] =  sin(theta_r) ;
	a[1][1] =  cos(theta_r) ;
	a[1][2] =  -aa*sin(theta_r)-bb*cos(theta_r)+bb ;
	a[2][0] =  0.0 ;
	a[2][1] =  0.0 ;
	a[2][2] =  1.0 ;

	for(m=0;m<3;m++)
	{
		determinant = determinant + (a[0][m]*(a[1][(m+1)%3]*a[2][(m+2)%3] - a[1][(m+2)%3]*a[2][(m+1)%3])) ;
	}

	for(m=0;m<3;m++)
	{
		for(n=0;n<3;n++)
		{
			b[n][m]=((a[(m+1)%3][(n+1)%3]*a[(m+2)%3][(n+2)%3])-(a[(m+1)%3][(n+2)%3]* a[(m+2)%3][(n+1)%3]))/determinant ;
		}
	}

	double xdash = (b[0][0])*(i)+(b[0][1])*(j)+(b[0][2])*1 ;
	double ydash = (b[1][0])*(i)+(b[1][1])*(j)+(b[1][2])*1 ;
	double hx  ;
	double hy  ;
	Real hz ;

	hx = xdash/width ;
	hy = ydash/width ;

	hz = 1.75 ;

	noise = stb_perlin_noise3 ( hx * frequency_x, hy * frequency_y, hz, xWrap=0, yWrap=0, zWrap=0 ) ;
	double temp1 = noise;
	
	noise = (temp1 + 1.0) / 2.0; // set range to 0 - 1

	if(noise < 0.0){cout<<"ALERT ALERT"<<endl;}

	idx = pressure_modification.index(i , j , k);
	pressure_modification[idx] = noise;
}

//	This is a time-varying perlin template with global rotation
PYTHON void create_perlin_template_rotate_3D_kernel ( Grid<Real>& pressure_modification, Real multiplier = 1.0, int res_x = 128, int res_y = 128, int res_z = 128, int time = 1, Real frequency_x = 1.0, Real frequency_y = 1.0, Real frequency_z = 1.0, int every_time_step = 1 , Real angle = 0, int sign = 1)
{
	step_1_create_perlin_template_rotate_3D_kernel ( pressure_modification, res_x, res_y, res_z, time, frequency_x, frequency_y, frequency_z, angle);

//	Get the maximum values
	Real max_absolute = pressure_modification.getMaxAbsValue();
	Real max_value = pressure_modification.getMaxValue();
	Real min_value = pressure_modification.getMinValue();

//	Scale the values
	multiplier *= sign ;
	scale_values_create_perlin_template_3D_kernel ( pressure_modification, multiplier, time, max_absolute, max_value, min_value) ;

//	char Result[8]; // string which will contain the number
//	sprintf ( Result, "%d", int(time) ); // %d makes the result be a decimal integer
//	string str_frequency_changer(Result);
//	if(time < 10)
//	{ str_frequency_changer = "00" + str_frequency_changer ; }
//	else if((time >= 10) && (time <=99))
//	{str_frequency_changer = "0" + str_frequency_changer ;}

//	string str = "/media/tushar/TOSHIBA EXT/latex/4_Perlin/7_global_rotation/template/" + str_frequency_changer + ".png" ;
//	const char *file_name_output = str.c_str();

//	int a = 1 ;
//	a = stbi_write_png(file_name_output, width, height, 1, noise_data, (sizeof(unsigned char)*width));
//	free (noise_data);
//	if(a == 1)
//	{
//		std::cout<<"SUCCESS"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"FAILURE"<<std::endl;
//	}
}

} // end namespace
