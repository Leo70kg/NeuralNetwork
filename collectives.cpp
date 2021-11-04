#include <vector>
#include <stdexcept>
#include <cassert> 
#include <cstring>
#include <iostream>
#include "collectives.h"


void InitCollectives() {  
	int mpi_error = MPI_Init(NULL, NULL);   
	if(mpi_error != MPI_SUCCESS) {  
		throw std::runtime_error("MPI_Init failed with an error");  
	}
}

// Allocate a new memory buffer. 
float* alloc(size_t size) 
{ 
	return new float[size];  
}

void dealloc(float* buffer) 
{
	delete[] buffer;
}

// Copy data from one memory buffer to another.
// Both buffers must resize on the same device. 
void copy(float* dst, float* src, size_t size)
{
	std::memcpy((void*) dst, (void*) src, size * sizeof(float));
}

// Copy data from one memory buffer to another.
// Both buffers must resize on the same device.
void reduce(float* dst, float* src, size_t size) 
{
	for(size_t i = 0; i < size; i++)
	{            
		dst[i] += src[i];  
	}
}

std::vector<size_t> AllgatherInputLengths(int size, size_t this_rank_length) 
{   
	std::vector<size_t> lengths(size);
	MPI_Allgather(&this_rank_length, 1, MPI_UNSIGNED_LONG, &lengths[0], 1, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);    
	
	return lengths; 
}

void RingAllreduce(float* data, size_t length, float* output) 
{
	// Get MPI size and rank.    
	int rank;
	int mpi_error = MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	if(mpi_error != MPI_SUCCESS) 
		throw std::runtime_error("MPI_Comm_rank failed with an error"); 

	int size; 
	mpi_error = MPI_Comm_size(MPI_COMM_WORLD, &size); 
	if(mpi_error != MPI_SUCCESS)    
		throw std::runtime_error("MPI_Comm_size failed with an error");    
	
	// Check that the lengths given to every process are the same.  
	std::vector<size_t> lengths = AllgatherInputLengths(size, length);  
	
	for(size_t other_length : lengths)
	{
		if (length != other_length) 
		{
			throw std::runtime_error("RingAllreduce received different lengths");  
		}
	}

	// Partition the elements of the array into N approximately equal-sized chunks, where N is the MPI size.     
	const size_t segment_size = length / size; 
	std::vector<size_t> segment_sizes(size, segment_size);

	// Load balance
	const size_t residual = length % size;   
	for (size_t i = 0; i < residual; ++i) 
	{
		segment_sizes[i]++;
	}

	// Compute where each chunk ends. 
	std::vector<size_t> segment_ends(size);  
	segment_ends[0] = segment_sizes[0];  
	for (size_t i = 1; i < segment_ends.size(); ++i)
	{
		segment_ends[i] = segment_sizes[i] + segment_ends[i - 1]; 
	}
	
	// The last segment should end at the very end of the buffer.  
	assert(segment_ends[size - 1] == length);    
	
	// Allocate the output buffer.
	//float* output = alloc(length);  
	//*output_ptr =  output;   
	
	// Copy your data to the output buffer to avoid modifying the input buffer. 
	copy(output, data, length); 
	
	// Allocate a temporary buffer to store incoming data.  
	// We know that segment_sizes[0] is going to be the largest buffer size, because if there are any overflow elements at least one will be added to the first segment.   
	float* buffer = alloc(segment_sizes[0]);   
	
	// Receive from your left neighbor with wrap-around.    
	const size_t recv_from = (rank - 1 + size) % size;   
	
	// Send to your right neighbor with wrap-around.  
	const size_t send_to = (rank + 1) % size; 
	
	MPI_Status recv_status; 
	MPI_Request recv_req; 
	MPI_Datatype datatype = MPI_FLOAT;   
	
	// Now start ring. At every step, for every rank, we iterate through segments with wraparound and send and recv from our neighbors and reduce locally. At the i'th iteration, sends segment (rank - i) and receives segment (rank - i - 1). 
	for (int i = 0; i < size - 1; i++) 
	{
		int recv_chunk = (rank - i - 1 + size) % size; 
		int send_chunk = (rank - i + size) % size; 
		float* segment_send = &(output[segment_ends[send_chunk] - segment_sizes[send_chunk]]);       
		MPI_Irecv(buffer, segment_sizes[recv_chunk], datatype, recv_from, 0, MPI_COMM_WORLD, &recv_req);    
		MPI_Send(segment_send, segment_sizes[send_chunk], MPI_FLOAT, send_to, 0, MPI_COMM_WORLD); 
		
		float *segment_update = &(output[segment_ends[recv_chunk] - segment_sizes[recv_chunk]]);
		
		// Wait for recv to complete before reduction  
		MPI_Wait(&recv_req, &recv_status); 
		reduce(segment_update, buffer, segment_sizes[recv_chunk]);  
	}
	
	// Now start pipelined ring allgather. At every step, for every rank, we iterate through segments with wraparound and send and recv from our neighbors. At the i'th iteration, rank r, sends segment (rank + 1 - i) and receives segment (rank - i).
	for (size_t i = 0; i < size_t(size - 1); ++i) 
	{
		int send_chunk = (rank - i + 1 + size) % size;   
		int recv_chunk = (rank - i + size) % size;   
		
		// Segment to send - at every iteration we send segment (r+1-i)  
		float* segment_send = &(output[segment_ends[send_chunk] - segment_sizes[send_chunk]]);  
		
		// Segment to recv - at every iteration we receive segment (r-i)   
		float* segment_recv = &(output[segment_ends[recv_chunk] - segment_sizes[recv_chunk]]);  
		
		MPI_Sendrecv(segment_send, segment_sizes[send_chunk], datatype, send_to, 0, segment_recv, segment_sizes[recv_chunk], datatype, recv_from, 0, MPI_COMM_WORLD, &recv_status);  
	}  
	
	// Free temporary memory.
	dealloc(buffer); 
}


void RingAllgather(float* data, size_t length, float** output_ptr) 
{
	// Get MPI size and rank. 
	int rank; 
	int mpi_error = MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	
	if(mpi_error != MPI_SUCCESS)  
		throw std::runtime_error("MPI_Comm_rank failed with an error"); 
	
	int size;
	mpi_error = MPI_Comm_size(MPI_COMM_WORLD, &size);  
	if (mpi_error != MPI_SUCCESS)   
		throw std::runtime_error("MPI_Comm_size failed with an error");   
	
	// Get the lengths of data provided to every process, so that we know how much memory to allocate for the output buffer.     
	std::vector<size_t> segment_sizes = AllgatherInputLengths(size, length);    
	size_t total_length = 0; 
	
	for(size_t other_length : segment_sizes) 
	{
		total_length += other_length;  
	}
	
	// Compute where each chunk ends.  
	std::vector<size_t> segment_ends(size);   
	segment_ends[0] = segment_sizes[0];  
	for (size_t i = 1; i < segment_ends.size(); ++i) 
	{
		segment_ends[i] = segment_sizes[i] + segment_ends[i - 1];   
	}
	
	assert(segment_sizes[rank] == length); 
	assert(segment_ends[size - 1] == total_length);  
	
	// Allocate the output buffer and copy the input buffer to the right place in the output buffer. 
	float* output = alloc(total_length);  
	*output_ptr = output; 
	copy(output + segment_ends[rank] - segment_sizes[rank], data, segment_sizes[rank]);    
	
	// Receive from your left neighbor with wrap-around.  
	const size_t recv_from = (rank - 1 + size) % size;   
	
	// Send to your right neighbor with wrap-around.  
	const size_t send_to = (rank + 1) % size; 
	
	// What type of data is being sent  
	MPI_Datatype datatype = MPI_FLOAT; 
	MPI_Status recv_status;  
	
	// Now start pipelined ring allgather. At every step, for every rank, we iterate through segments with wraparound and send and recv from our neighbors. At the i'th iteration, rank r, sends segment (rank + 1 - i) and receives segment (rank - i).     
	for (size_t i = 0; i < size_t(size - 1); ++i) 
	{
		int send_chunk = (rank - i + size) % size; 
		int recv_chunk = (rank - i - 1 + size) % size;  
		
		// Segment to send - at every iteration we send segment (r+1-i) 
		float* segment_send = &(output[segment_ends[send_chunk] - segment_sizes[send_chunk]]);    
		
		// Segment to recv - at every iteration we receive segment (r-i)   
		float* segment_recv = &(output[segment_ends[recv_chunk] - segment_sizes[recv_chunk]]);  
		
		MPI_Sendrecv(segment_send, segment_sizes[send_chunk], datatype, send_to, 0, segment_recv, segment_sizes[recv_chunk], datatype, recv_from, 0, MPI_COMM_WORLD, &recv_status); 
	}
}
