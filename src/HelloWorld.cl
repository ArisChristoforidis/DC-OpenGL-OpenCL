
typedef /*__attribute__ ((packed))*/ struct{
  //Initializing with a value causes problems.
  unsigned int index;
  char edgeHasSignChange;
  char used;
  float x,y,z;
} WorkData;

__kernel void HelloWorld(__global char* data){

data[0] = 'H';
data[1] = 'e';
data[2] = 'l';
data[3] = 'l';
data[4] = 'o';
data[5] = ' ';
data[6] = 'A';
data[7] = 'r';
data[8] = 'i';
data[9] = 's';
data[10] = '.';
data[11] = '\n';
}


__global int[][3] cornerOffset = {
  {0,0,0},
  {0,0,1},
  {0,1,0},
  {0,1,1},
  {1,0,0},
  {1,0,1},
  {1,1,0},
  {1,1,1}
};

__global int edgeMap[12][2] = {
  {0,4},{1,5},{2,6},{3,7},	// x-axis
  {0,2},{1,3},{4,6},{5,7},	// y-axis
  {0,1},{2,3},{4,5},{6,7}		// z-axis
};

__kernel void EvaluateWorldSpace(__global WorkData* nodes,int nodeCount){

  int id = get_global_id(0);
  if(id >= nodeCount){
    return;
  }
  if(id < 100){
    nodes[id].used = 0;
  }else{
    nodes[id].used = 1;
  }

  /*
  float cornerDensity[8];
  char cornerHasSurface = 0;
  for(int i=0;i<8;i++){
    float x = nodes[id] + cornerDensity[i][0];
    float y = nodes[id] + cornerDensity[i][1];
    float z = nodes[id] + cornerDensity[i][2];

    cornerDensity[i] = TorusFunction(x,y,z);
    int hasSurface = cornerDensity > 0 ? 1 : 0;
    cornerHasSurface |= (hasSurface << i);
  }

  if(cornerHasSurface == 0 || cornerHasSurface == 255){
    return;
  }

  //Check if this works.
  float[15][3] edgelist;
  for(int i=0;i<12;i++){
    int v0 = (cornerHasSurface >> edgeMap[i][0]) & 1;
    int v1 = (cornerHasSurface >> edgeMap[i][1]) & 1;

    if(v0 != v1){
      return;
    }

  }
  */


}

float TorusFunction(float x, float y, float z) {
	const float c = 50.0f;
	const float a = 6.0f;
	return (c - sqrt(x*x + y * y))*(c - sqrt(x*x + y * y)) + z * z - a * a;
}
