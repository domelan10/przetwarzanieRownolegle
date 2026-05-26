#include <stdio.h>
#include <stdlib.h>

int main(void) {
	cudaDeviceProp  prop;
	int count;
	printf("{\n");
	cudaGetDeviceCount(&count);
	printf("\"Devices\":[");
	for (int i=0;i<count;i++) {
		printf("\n\t{\n");
		cudaGetDeviceProperties(&prop,i);
		printf("\t\"name\":\"%s\",\n",prop.name);
		printf("\t\"major\":\"%d\",\n",prop.major);
		printf("\t\"minor\":\"%d\",\n",prop.minor);
		printf("\t\"integrated\":\"%d\",\n",prop.integrated);
		printf("\t\"tccDriver\":\"%d\",\n",prop.tccDriver);
		printf("\t\"ECCEnabled\":\"%d\",\n",prop.ECCEnabled);
		printf("\t\"concurrentKernels\":\"%d\",\n",prop.concurrentKernels);
		printf("\t\"canMapHostMemory\":\"%d\",\n",prop.canMapHostMemory);
		printf("\t\"multiProcessorCount\":\"%d\",\n",prop.multiProcessorCount);
		printf("\t\"warpSize\":\"%d\",\n",prop.warpSize);
		printf("\t\"maxThreadsPerBlock\":\"%d\",\n",prop.maxThreadsPerBlock);
		printf("\t\"maxThreadsDim\":[%d,%d,%d],\n",prop.maxThreadsDim[0], prop.maxThreadsDim[1],prop.maxThreadsDim[2]);
		printf("\t\"maxGridSize\":[%d,%d,%d],\n",prop.maxGridSize[0],prop.maxGridSize[1],prop.maxGridSize[2]);
		printf("\t\"maxTexture1D\":\"%d\",\n",prop.maxTexture1D);
		printf("\t\"maxTexture2D\":[%d,%d],\n",prop.maxTexture2D[0],prop.maxTexture2D[1]);
		printf("\t\"maxTexture3D\":[%d,%d,%d],\n",prop.maxTexture3D[0],prop.maxTexture3D[1],prop.maxTexture3D[2]);
		printf("\t\"pciBusID\":\"%d\",\n",prop.pciBusID);
		printf("\t\"pciDeviceID\":\"%d\",\n",prop.pciDeviceID);
		printf("\t\"regsPerBlock\":\"%d\",\n",prop.regsPerBlock);
		printf("\t\"memPitch\":\"%llu\",\n",prop.memPitch);
		printf("\t\"surfaceAlignment\":\"%llu\",\n",prop.surfaceAlignment);
		printf("\t\"textureAlignment\":\"%llu\",\n",prop.textureAlignment);
		printf("\t\"totalConstMem\":\"%llu\",\n",prop.totalConstMem);
		printf("\t\"totalGlobalMem\":\"%llu\",\n",prop.totalGlobalMem);	
		printf("\t\"sharedMemPerBlock\":\"%llu\",\n",prop.sharedMemPerBlock);
		printf("\t\"canMapHostMemory\":\"%d\",\n",prop.canMapHostMemory);
		printf("\t\"regsPerBlock\":\"%d\"\n",prop.regsPerBlock);
		if (i<count-1) printf("\t},\n"); else printf("\t}\n");
	}
	printf("]\n}\n");
	return EXIT_SUCCESS;
}