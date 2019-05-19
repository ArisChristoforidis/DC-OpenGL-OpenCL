typedef /*__attribute__ ((packed))*/ struct{
  //Initializing with a value causes problems.
  unsigned int index;
  uchar cornerValues;
  char used;
  float x,y,z;
} WorkData;


/*
//Use to test Host/Device communication.If this doesn't work,you have serious problems.
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
*/

float TorusFunction(float x,float y,float z) {
	const float c = 50.0f;
	const float a = 6.0f;
  float sqrt = pow(x*x + y*y,0.5f);
	return (c - sqrt)*(c - sqrt) + z * z - a * a;
}

float Adapt(float v0Val, float v1Val) {
	return -v0Val /(v1Val - v0Val);
}


float3 CalculateNormal(float x,float y,float z,float d){
  float3 derivatives;
	derivatives.x = (TorusFunction(x + d, y,  z) - TorusFunction(x - d,  y,  z)) / 2.0f / d;
	derivatives.y = (TorusFunction(x, y + d,  z) - TorusFunction(x,  y - d,  z)) / 2.0f / d;
	derivatives.z = (TorusFunction(x, y,  z + d) - TorusFunction(x,  y,  z - d)) / 2.0f / d;
  return derivatives;
}

float3 GetAveragePoint(float points[15][3],int count){
  float3 averagePoint = 0;

  for(int i=0;i<count;i++){
    averagePoint.x += points[i][0];
    averagePoint.y += points[i][1];
    averagePoint.z += points[i][2];
  }

  averagePoint /= count;
  return averagePoint;
}


__constant int cornerOffset[8][3] = {
  {0,0,0},
  {0,0,1},
  {0,1,0},
  {0,1,1},
  {1,0,0},
  {1,0,1},
  {1,1,0},
  {1,1,1}
};

__constant int edgeMap[12][2] = {
  {0,4},{1,5},{2,6},{3,7},	// x-axis
  {0,2},{1,3},{4,6},{5,7},	// y-axis
  {0,1},{2,3},{4,5},{6,7}		// z-axis
};

/*
//Returns the vector length.
float VectorLength(float v[],unsigned int start,unsigned int stop){
  float sum = 0;

  //Calculate v[0]^2 + v[1]^2 + ... + v[n]^2
  for(int i=start;i<stop;i++){
    sum += v[i]*v[i];
  }
  return pow(sum,0.5f);
}
*/
/*
__constant int MAXROWS = 15;
__constant int N = 3;
__constant int NINC = 4;
*/
/*
void QrFactorization(float A[15][4]){

  //for k = 0 to N
  for(int k=0;k<N-1;k++){

    //Define y = A(k:N,k)
    float y[MAXROWS] = {0};

    for(int i=k;i<MAXROWS;i++){
      y[i] = A[i][k];
    }

    float yLength = 1.0f;//VectorLength(y,k,MAXROWS);
    //Sign can be -1 or 1.
    float sign = 1.0f;//y[k]/fabs(y[k]);
    //w vector is defined as : w = y + sign(y0) * ||y|| * e1
    //e1 vector is defined as : e1 = [1,0,0,...,0]
    //y is now w(to save space).
    y[k] += sign * yLength;

    //v vector is defined as : v = 1/||w|| * w
    //y is now v(to save space).
    float wLength = 1.0f;//VectorLength(y,k,MAXROWS);
    for (int i = k; i < MAXROWS; i++) {
      y[i] = 1.0f/wLength * y[i];
    }

    float vMatrix[MAXROWS][MAXROWS];
    for(int i=k;i<MAXROWS;i++){
      for(int j=k;j<MAXROWS;j++){
        vMatrix[i][j] = 2 * y[i] * y[j];
      }
    }

    float newA[MAXROWS][NINC] = {0};
    for(int i=k;i<MAXROWS;i++){
      for(int j=k;j<NINC;j++){
        for(int l=k;l<MAXROWS;l++){
          newA[i][j] += vMatrix[i][l] * A[l][j];
        }
      }
    }

    for(int i=k;i<MAXROWS;i++){
      for(int j=k;j<NINC;j++){
        A[i][j] -= newA[i][j];
      }
    }
    */
  }
  /*
  for(int i=0;i<N;i++){
    float diagValue = A[i][i];
    for(int j=i;j<NINC;j++){
      A[i][j] /= diagValue;
    }
  }

}
*/
/*
__constant float BIAS_STRENGTH = 0.01f;
void SolveQEF(float edgePositions[15][3],float edgeNormals[15][3],uint edgeCount){
  float extendedA[15][4],x[3];

  float3 averagePoint = GetAveragePoint(edgePositions,edgeCount);

  for(int i=edgeCount;i<edgeCount+3;i++){
    edgePositions[i][0] = averagePoint.x;
    edgePositions[i][0] = averagePoint.y;
    edgePositions[i][0] = averagePoint.z;
  }


  edgeNormals[edgeCount][0] = BIAS_STRENGTH;
  edgeNormals[edgeCount][1] = 0;
  edgeNormals[edgeCount][2] = 0;

  edgeNormals[edgeCount+1][0] = 0;
  edgeNormals[edgeCount+1][1] = BIAS_STRENGTH;
  edgeNormals[edgeCount+1][2] = 0;

  edgeNormals[edgeCount+2][0] = 0;
  edgeNormals[edgeCount+2][1] = 0;
  edgeNormals[edgeCount+2][2] = BIAS_STRENGTH;

  edgeCount += 3;
  for(int i=0;i<edgeCount;i++){
    extendedA[i][0] = edgeNormals[i][0];
    extendedA[i][1] = edgeNormals[i][1];
    extendedA[i][2] = edgeNormals[i][2];

    float b = edgePositions[i][0] * edgeNormals[i][0] + edgePositions[i][1] * edgeNormals[i][1] + edgePositions[i][2] * edgeNormals[i][2];
    extendedA[i][3] = b;
  }

}
*/
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

  //Function cbrt() doesn't work so I use this workaround to calculate the cube root.
  int dimMax = round(pow(nodeCount,1.0/3.0));
  //The [x,y,z] coordinates of the node.
  float3 nodePos;
  nodePos.z = id % dimMax - dimMax/2;
  nodePos.y = (id / dimMax) % dimMax - dimMax/2;
  nodePos.x = id / (dimMax * dimMax) - dimMax/2;

  //float nodeZ = id % dimMax - dimMax/2;
  //float nodeY = (id / dimMax) % dimMax - dimMax/2;
  //float nodeX = id / (dimMax * dimMax) - dimMax/2;

  float cornerDensity[8];
  uchar cornerHasSurface = 0;

  for(int i=0;i<8;i++){
    float3 cornerPos;
    cornerPos.x = nodePos.x + cornerOffset[i][0];
    cornerPos.y = nodePos.y + cornerOffset[i][1];
    cornerPos.z = nodePos.z + cornerOffset[i][2];
    //float x = nodeX + cornerOffset[i][0];
    //float y = nodeY + cornerOffset[i][1];
    //float z = nodeZ + cornerOffset[i][2];


    cornerDensity[i] = TorusFunction(cornerPos.x,cornerPos.y,cornerPos.z);
    int hasSurface = cornerDensity[i] > 0 ? 1 : 0;
    cornerHasSurface |= (hasSurface << i);
  }
  nodes[id].cornerValues = cornerHasSurface;

  if(cornerHasSurface == 0 || cornerHasSurface == 255){
    nodes[id].used = 0;
    return;
  }

  nodes[id].used = 1;

  float edgelist[15][3];
  uint edgeCount = 0;
  for(int i=0;i<12;i++){
    int v0 = (cornerHasSurface >> edgeMap[i][0]) & 1;
    int v1 = (cornerHasSurface >> edgeMap[i][1]) & 1;

    if(v0 != v1){
      int cornerIndex = edgeMap[i][0];

      float3 edgePos;
      edgePos.x = nodePos.x + cornerOffset[cornerIndex][0];
      edgePos.y = nodePos.y + cornerOffset[cornerIndex][2];
      edgePos.z = nodePos.z + cornerOffset[cornerIndex][2];

      //float edgeX = nodeX + cornerOffset[cornerIndex][0];
      //float edgeY = nodeY + cornerOffset[cornerIndex][1];
      //float edgeZ = nodeZ + cornerOffset[cornerIndex][2];

      float c0Value = cornerDensity[edgeMap[i][0]];
      float c1Value = cornerDensity[edgeMap[i][1]];

      float adaptedVal = Adapt(c0Value,c1Value);
      if(i<4){
        edgePos.x = nodePos.x + adaptedVal;
      }else if(i<8){
        edgePos.y = nodePos.y + adaptedVal;
      }else{
        edgePos.z = nodePos.z + adaptedVal;
      }

      edgelist[edgeCount][0] = edgePos.x;
      edgelist[edgeCount][1] = edgePos.y;
      edgelist[edgeCount][2] = edgePos.z;
      edgeCount++;

    }


  }

  if(edgeCount <= 1){
    return;
  }

  float edgeNormals[15][3];
  for(int i=0;i<edgeCount;i++){
    float3 normal = CalculateNormal(edgelist[i][0],edgelist[i][1],edgelist[i][2],0.01f);
    edgeNormals[i][0] = normal.x;
    edgeNormals[i][1] = normal.y;
    edgeNormals[i][2] = normal.z;
  }

  /*float3 solution =*/ //SolveQEF(edgelist,edgeNormals,edgeCount);

}
