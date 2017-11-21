#include<cuda.h>
#include<nvvm.h>
#include<unistd.h>
#include<malloc.h>

#include<iostream>
#include<iomanip>

#include"SimpleSW.h"
#include"Data.h"
#include"Cost.h"


// Lenght of each data
__constant__ int gcT_size;
__constant__ int gcP_size;

// Threshold of the SW algorithm
__constant__ int gcThre;

// Data of the query
__constant__ char gcP_seq[1024];

// Cost and Gain
__constant__ int gcMatch;
__constant__ int gcMiss;
__constant__ int gcExtend;
__constant__ int gcBegin;

enum{
	Zero,
	Diagonal,
	Vertical,
	Horizon,
};

using namespace std;

SimpleSW::SimpleSW(const Data& db, const Data& q, int threshold, int block_num) : mBlock{ block_num }
{
	cout << "Simple SW algorithm start" << endl;
	// Sieze check
	if(q.size() > 1024 || db.size() > 1024 * 1024 * 1024){
		cout << "Too large size" << endl;
		return;
	}
	mThreshold = threshold;
	// Set value in constant memory
	mSize = db.size();
	int psize = q.size();
	cudaMemcpyToSymbol(gcT_size, &mSize, sizeof(int));
	cudaMemcpyToSymbol(gcP_size, &psize, sizeof(int));
	cudaMemcpyToSymbol(gcThre, &threshold, sizeof(int));
	cudaMemcpyToSymbol(gcP_seq, q.data(), sizeof(char) * q.size());
	// Cost and gain
	int gain = cost.Match;
	cudaMemcpyToSymbol(gcMatch, &gain, sizeof(int));
	gain = cost.Miss;
	cudaMemcpyToSymbol(gcMiss, &gain, sizeof(int));
	gain = cost.Extend;
	cudaMemcpyToSymbol(gcExtend, &gain, sizeof(int));
	gain = cost.Begin;
	cudaMemcpyToSymbol(gcBegin, &gain, sizeof(int));
	// Dynamic Programing part by call_DP
	call_DP(db, q);
	cout << "Max score is " << max_score() << ", max position is " << max_position() << endl; 
	cout << "Simple SW algorithm end" << endl;
}

SimpleSW::~SimpleSW(){
	if(mScore){
		delete[] mScore;
		mScore = nullptr;
	}
}

// Implementation 
// No traceback
__global__ void DP(char* dT_seq, int* dScore, int block_size){
	// ThreadId = q point
	int id = threadIdx.x;
	// The acid in this thread
	char p = gcP_seq[id];
	// p-1 row line's value
	__shared__ int Hp_1[1024];
	__shared__ int Ep_1[1024];	
	// Temporary
	int Hp_1_buf = 0;
	int Ep_1_buf = 0;
	// t-1 element value
	int Ht_1 = 0;
	int Ft_1 = 0;
	// p-1 t-1 element value
	int Ht_1p_1 = 0;
	// Initialize
	Hp_1[id] = 0;
	Ep_1[id] = 0;
	// Similar score
	int sim = 0;
	// Set start point
	int start = (block_size - gcP_size) * blockIdx.x;
	if(blockIdx.x != 0){ start -= gcP_size; }	// Take margin
	// Culcurate elements
	for(int t = start - id; t < start + block_size; ++t){
		// Control culcurate order
		if(t<0 || t < start){}
		// Get similar score
		else{
			// Compare acids
			if(dT_seq[t] == p){sim = gcMatch;}
			else{sim = gcMiss;}
		}
		// SW algorithm
		// Culcurate each elements
		Ht_1p_1 += sim;	// Diagonal
		Ht_1 += gcBegin;	// Horizon (Start)
		Ft_1 += gcExtend;	// Horizon (Extend)
		Hp_1_buf = Hp_1[id] + gcBegin;	// Vertical (Start)
		Ep_1_buf = Ep_1[id] + gcExtend;	// Vertical (Extend)
		// Choose the gap score
		if(Ht_1 > Ft_1){Ft_1 = Ht_1;}	// Horizon
		if(Hp_1_buf > Ft_1){Ep_1_buf = Hp_1_buf;}	// Vertical
		// Choose the max score
		// Ht_1 is stored the max score
		if(Ht_1p_1 > Ep_1_buf){
			// Diagonal
			if(Ht_1p_1 > Ft_1){
				Ht_1 = Ht_1p_1;
			}
			// Horizon
			else{
				Ht_1 = Ft_1;
			}
		}
		else {
			// Vertical
			if(Ep_1_buf > Ft_1){
				Ht_1 = Ep_1_buf;
			}
			// Horizon
			else{
				Ht_1 = Ft_1;
			}
		}
		// The case 0 is max
		if(Ht_1 <= 0){
			Ht_1 = 0;
			// Set 0 other value
			Ft_1 = 0;
			Ep_1_buf = 0;
		}
		// Hp-1 is next Ht-1p-1 
		Ht_1p_1 = Hp_1[id];
		__syncthreads();
		// Set value need next culcurate
		// p+1 row line
		if(t >= 0){
			Hp_1[id + 1] = Ht_1;
			Ep_1[id + 1] = Ep_1_buf;
		}
		if(Ht_1 >= gcThre){
			if(Ht_1 >= dScore[t]){
			// Set score
				dScore[t] = Ht_1;
			}
		} 
		__syncthreads();
		// for end
	}
}

// With traceback
__global__ void DPwith(char* dT_seq, char* dTrace, int start, int length){
	// ThreadId = q point
	int id = threadIdx.x;
	// The acid in this thread
	char p = gcP_seq[id];
	// p-1 row line's value
	__shared__ int Hp_1[1024];
	__shared__ int Ep_1[1024];	
	// Temporary
	int Hp_1_buf = 0;
	int Ep_1_buf = 0;
	// t-1 element value
	int Ht_1 = 0;
	int Ft_1 = 0;
	// p-1 t-1 element value
	int Ht_1p_1 = 0;
	// Initialize
	Hp_1[id] = 0;
	Ep_1[id] = 0;
	// Similar score
	int sim = 0;
	int point = id * length - id;
	// Culcurate elements
	for(int t = -id + start; t < start + length; ++t){
		// Control culcurate order
		if(t<start){}
		// Get similar score
		else{
			// Compare acids
			if(dT_seq[t] == p){sim = gcMatch;}
			else{sim = gcMiss;}
		}
		// SW algorithm
		// Culcurate each elements
		Ht_1p_1 += sim;	// Diagonal
		Ht_1 += gcBegin;	// Horizon (Start)
		Ft_1 += gcExtend;	// Horizon (Extend)
		Hp_1_buf = Hp_1[id] + gcBegin;	// Vertical (Start)
		Ep_1_buf = Ep_1[id] + gcExtend;	// Vertical (Extend)
		// Choose the gap score
		if(Ht_1 > Ft_1){Ft_1 = Ht_1;}	// Horizon
		if(Hp_1_buf > Ft_1){Ep_1_buf = Hp_1_buf;}	// Vertical
		// Choose the max score
		// Ht_1 is stored the max score
		if(Ht_1p_1 > Ep_1_buf){
			// Diagonal
			if(Ht_1p_1 > Ft_1){
				Ht_1 = Ht_1p_1;
				dTrace[point] = Diagonal;
			}
			// Horizon
			else{
				Ht_1 = Ft_1;
				dTrace[point] = Horizon;
			}
		}
		else {
			// Vertical
			if(Ep_1_buf > Ft_1){
				Ht_1 = Ep_1_buf;
				dTrace[point] = Vertical;
			}
			// Horizon
			else{
				Ht_1 = Ft_1;
				dTrace[point] = Horizon;
			}
		}
		// The case 0 is max
		if(Ht_1 <= 0){
			Ht_1 = 0;
			// Set 0 other value
			Ft_1 = 0;
			Ep_1_buf = 0;
			dTrace[point] = Zero;
		}
		// Hp-1 is next Ht-1p-1 
		Ht_1p_1 = Hp_1[id];
		__syncthreads();
		// Set value need next culcurate
		// p+1 row line
		if(t >= start){
			Hp_1[id + 1] = Ht_1;
			Ep_1[id + 1] = Ep_1_buf;
			// DEBUG, score check
	//		dTrace[point] = (char)(Ht_1);
		} 
		++point;
		__syncthreads();
		// for end
	}
}

// Provisional
void SimpleSW::call_DP(const Data& db, const Data& q){
	// Set db
	char* dT_seq;
	cudaMalloc((void**)&dT_seq, sizeof(char)*db.size());
	cudaMemcpy(dT_seq, db.data(), sizeof(char)*db.size(), cudaMemcpyHostToDevice);
	// Set block size
	int block_size = (db.size()/mBlock) + q.size();
	// Set Score and point
	int* dScore;
	cudaMalloc((void**)&dScore, sizeof(int)*db.size());
	int* init0 = new int[db.size()];
	for(int i=0;i<db.size();++i){init0[i]=0;}
	cudaMemcpy(dScore, init0, sizeof(int)*db.size(), cudaMemcpyHostToDevice);
	// Main process
	DP<<<mBlock,q.size()>>>(dT_seq, dScore, block_size);	
	// Score and point copy
	mScore = new int[db.size()];
	cudaMemcpy(mScore, dScore, sizeof(int)*db.size(), cudaMemcpyDeviceToHost);
	// traceback if db has homelogy
//	checkScore(db, q);
	delete[] init0;
	cudaFree(dT_seq);
	cudaFree(dScore);
}

// score -> 0~16 : 17~31 = score : point of q
void SimpleSW::checkScore(const Data& db, const Data& q){
	// get the max score
	int max = max_score();
	int x = max_position();
	if(max != 0){
		// Call DP in limit range
		// Set db
		char* dT_seq;
		cudaMalloc((void**)&dT_seq, sizeof(char)*db.size());	// TODO Too much range
		cudaMemcpy(dT_seq, db.data(), sizeof(char)*db.size(), cudaMemcpyHostToDevice);
		// Set Traceback
		int length = q.size() * 2;	// Enough
		char* dTrace;
		cudaMalloc((void**)&dTrace, sizeof(char)*length*q.size());
		DPwith<<<1,q.size()>>>(dT_seq, dTrace, x - length + 1, length);
		// Direction copy
		char* direction = new char[length*q.size()];
		cudaMemcpy(direction, dTrace, sizeof(char)*length*q.size(), cudaMemcpyDeviceToHost);	
//		show(direction, db, q, x - length + 1, length);
//		traceback(direction, db, x, y, length);
		delete[] direction;
		cudaFree(dT_seq);
		cudaFree(dTrace);
	}
}

void SimpleSW::traceback(const char* direction, const Data& db, int x, int y, int length) const{
	// Store the result, get enough size
	char *ans = new char[1024 * 2];
	// Point of result array
	int p = 0;
	int db_point = x;
	// trace point must be most right
	int trace =  length - 1 + y * length;
	// Traceback
	while(trace >= 0){
		switch(direction[trace]){
		case Diagonal:
			ans[p++] = db[db_point--];
			trace -= length + 1;
			break;
		case Vertical:
			ans[p++] = '+';
			trace -= length;
			break;
		case Horizon:
			ans[p++] = '-';
			--trace;
			--db_point;
			break;
		case Zero:	// End
			trace = -1;
		default:
			trace = -1;
		}
	}
	// This array has reverse answer
	for(int i=p-1;i>=0;--i){ cout << ans[i]; }
	printf("  %d ~ %d \n", db_point+1, x);
	delete[] ans;
}

