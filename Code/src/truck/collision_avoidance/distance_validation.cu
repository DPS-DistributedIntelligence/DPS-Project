#include<iostream>
#include<stdio.h>
#include<cuda.h>
#include <stdbool.h>
#include<cuda_runtime_api.h>

using namespace std;
#define Number_of_sensor 4

// Kernel
__global__ void distance_validation(double *sensor_input, double *treshold, double *output)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    output[i] =    sensor_input[i] - treshold[i];
}


int main(){


    //Vector Declaration and Definition
    double sensor_input[Number_of_sensor] =   { 9.0  , 8.0 , 0.5 , 6.0   };
    double treshold[Number_of_sensor]     =   { 3.0  , 3.0 , 0.8 , 0.8  };
    double output[Number_of_sensor]       =   { 0.0  , 0.0 , 0.0 , 0.0  };
    double *d_sensor_input, *d_treshold, *d_output;


    //Allocation of Device variables
    cudaMalloc((void **)&d_sensor_input, sizeof(double)*Number_of_sensor);
    cudaMalloc((void **)&d_treshold, sizeof(double)*Number_of_sensor);
    cudaMalloc((void **)&d_output, sizeof(double)*Number_of_sensor);

    //Copy Host Memory to Device Memory
    cudaMemcpy(d_sensor_input, &sensor_input, sizeof(double)*Number_of_sensor, cudaMemcpyHostToDevice);
    cudaMemcpy(d_treshold, &treshold, sizeof(double)*Number_of_sensor, cudaMemcpyHostToDevice);
    cudaMemcpy(d_output, &output, sizeof(double)*Number_of_sensor, cudaMemcpyHostToDevice);

    int thr_per_blk = 3;
    int blk_in_grid = ceil( float(Number_of_sensor) / thr_per_blk );


    // Launch kernel
    distance_validation<<< blk_in_grid, thr_per_blk >>>(d_sensor_input, d_treshold, d_output);

     //Copy Device Memory to Host Memory
    cudaMemcpy(&output, d_output, sizeof(double)*Number_of_sensor, cudaMemcpyDeviceToHost);


    // print result



    printf("Sensor position: front  \t  back  \t left  \t\t  right  \t\n\n");

    printf("Sensor input:  \t %f \t %f \t %f \t %f \t\n\n", sensor_input[0], sensor_input[1], sensor_input[2], sensor_input[3]);

    printf("treshold:  \t %f \t %f \t %f \t %f \t\n\n", treshold[0], treshold[1], treshold[2], treshold[3]);

    printf("output:  \t %f \t %f \t %f \t %f \t\n\n", output[0], output[1], output[2], output[3]);

    std::string v_1 = (output[0]>0)?"true":"false";
    std::string v_2 = (output[1]>0)?"true":"false";
    std::string v_3 = (output[2]>0)?"true":"false";
    std::string v_4 = (output[3]>0)?"true":"false";

    std::cout << "validation: "<< "  \t " << v_1 << "\t\t  " << v_2 << "\t\t " << v_3 << "\t\t " << v_4 << "\n\n" << std::endl;



    //front sensor
    if(output[0]<0){
      printf("front side is to close with obstacle !!!!\n");
    }

    //back sensor
    if(output[1]<0){
      printf("back side is to close with obstacle !!!!\n");
    }

    //left sensor
    if(output[2]<0){
      printf("left side is to close with obstacle !!!!\n");
    }

    //right sensor
    if(output[3]<0){
      printf("right side is to close with obstacle !!!!\n");
    }


    // Free GPU memory
    cudaFree(d_sensor_input);
    cudaFree(d_treshold);
    cudaFree(d_output);

    return 0;
}
